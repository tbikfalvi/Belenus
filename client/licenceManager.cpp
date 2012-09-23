
#include <QSettings>
#include <QDateTime>

#include "licenceManager.h"
#include "preferences.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"

extern cPreferences *g_poPrefs;
extern cQTMySQLConnection *g_poDB;
extern cQTLogger g_obLogger;

cLicenceManager::cLicenceManager()
{
    m_LicenceType = LTYPE_DEMO;

    m_qslCode = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J"
                              << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T"
                              << "U" << "V" << "O" << "X" << "Y" << "Z" << "J" << "T" << "K" << "A"
                              << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J"
                              << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T"
                              << "U" << "V" << "O" << "X" << "Y" << "Z" << "J" << "T" << "K" << "A";

    m_qslLicenceKeys = QStringList() << "TO[@I_R"  // BLNS01_DPKBTY <- BLNS01_358194
                                     << "TL[SHGD"  // BLNS02_GPXCLO <- BLNS02_653212
                                     << "TIZ_L[D"  // BLNS03_BQTGPO <- BLNS03_167652
                                     << "TBXDL@D"  // BLNS04_ISOGKO <- BLNS04_882602
                                     << "TCYSNZ^"  // BLNS05_HRXEQU <- BLNS05_773460
                                     << "TB__AX@"  // BLNS06_ITTJSK <- BLNS06_897988
                                     << "TMXJJY^"  // BLNS07_FSAARU <- BLNS07_589070
                                     << "TAEQIE_"  // BLNS08_JNZBNT <- BLNS08_935137
                                     << "TOFRO__"  // BLNS09_DMYDTT <- BLNS09_324397
                                     << "TAE]B@S"  // BLNS10_JNVIKX <- BLNS10_931803
                                     << "TH[AM[_"  // BLNS11_CPJFPT <- BLNS11_256557
                                     << "TCXJBYA"  // BLNS12_HSAIRJ <- BLNS12_789876
                                     << "TA[DJ_Q"  // BLNS13_JPOATZ <- BLNS13_952095
                                     << "TIERJ[]"  // BLNS14_BNYAPV <- BLNS14_134051
                                     << "TID]JF@"  // BLNS15_BOVAMK <- BLNS15_141028
                                     << "TNDQJYD"  // BLNS16_EOZARO <- BLNS16_445072
                                     << "TNXAHES"  // BLNS17_ESJCNX <- BLNS17_486233
                                     << "TC@^IYA"  // BLNS18_HKUBRJ <- BLNS18_700176
                                     << "TCY@N[_"  // BLNS19_HRKEPT <- BLNS19_778457
                                     << "TN__MX]"  // BLNS20_ETTFSV <- BLNS20_497581
                                     << "TM[@O@Q"  // BLNS21_FPKDKZ <- BLNS21_558305
                                     << "TH[JB_]"  // BLNS22_CPAITV <- BLNS22_259891
                                     << "TNFJHX^"  // BLNS23_EMACSU <- BLNS23_429280
                                     << "TL_DJ[J"  // BLNS24_GTOAPA <- BLNS24_692059
                                     << "TAEDH_Q"  // BLNS25_JNOCTZ <- BLNS25_932295
                                     << "TOXAN@]"  // BLNS26_DSJEKV <- BLNS26_386401
                                     << "TMX^JDA"  // BLNS27_FSUAOJ <- BLNS27_580046
                                     << "TA@RAZ@"  // BLNS28_JKYJQK <- BLNS28_904968
                                     << "TLXRJGA"  // BLNS29_GSYALJ <- BLNS29_684016
                                     << "TCZAJ_J"; // BLNS30_HQJATA <- BLNS30_766099
}

cLicenceManager::~cLicenceManager()
{

}

void cLicenceManager::initialize()
{
    int          nLicenceId = 0;
    QSqlQuery   *poQuery = NULL;

    try
    {
        _checkCode();

        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences WHERE active=1 ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_qdLastValidated   = poQuery->value( 2 ).toDate();
            m_qsLicenceString   = poQuery->value( 1 ).toString();
            nLicenceId          = poQuery->value( 0 ).toInt();

            if( nLicenceId > 1 )
            {
                m_LicenceType = LTYPE_REGISTERED;
                _checkValidity();
            }
            g_poPrefs->setLicenceId( nLicenceId );
            g_obLogger(cSeverity::INFO) << "Initialized with " << m_qsLicenceString << " and " << nLicenceId << ". Last validated = " << m_qdLastValidated.toString() << EOM;
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

bool cLicenceManager::isDemo()
{
    return m_LicenceType == LTYPE_DEMO || m_LicenceType == LTYPE_EXPIRED || m_LicenceType == LTYPE_INVALID;
}

int cLicenceManager::daysRemain()
{
    if ( !m_qdLastValidated.isValid() )
        return 0;

    int nDays = m_qdLastValidated.daysTo(QDate::currentDate());
    // nDays<0 means currentdate is before lastvalidated - error
    if ( nDays < 0 )
        return 0;

    // itt lehet nDays 0..n .. EXP-days lehet -vegtelen..EXP
    nDays = EXPIRE_IN_DAYS - nDays;
    if ( nDays < 0 ) // ha ez mar tobb mint EXP_IN_DAYS akkor nincs tobb nap hatra
        nDays = 0;

    return nDays;
}

int cLicenceManager::validateLicence( const QString &p_qsLicenceString )
{
    // liszensz kód mentése az adatbázisba, amennyiben nincs még kód
    // ha van már kód, akkor hibaüzenet
    // ...
    int nRet = ERR_NO_ERROR;

    if( p_qsLicenceString.length() != 13 )
    {
        return ERR_KEY_FORMAT_MISMATCH;
    }

    if( p_qsLicenceString.left(4).toUpper().compare("BLNS") ||
        p_qsLicenceString.at(4).isDigit() ||
        p_qsLicenceString.at(5).isDigit() ||
        p_qsLicenceString.at(6) != QChar('_') )
    {
        return ERR_KEY_FORMAT_MISMATCH;
    }

    return nRet;
}

void cLicenceManager::_checkCode()
{
    QSettings   settings( "HKEY_LOCAL_MACHINE\\Software\\SV", QSettings::NativeFormat );

    if( !settings.contains( "Code" ) )
    {
        // Kod nem letezik, letre kell hozni
        int         nMin = 100000;
        int         nMax = 999999;
        QDateTime   startDate( QDate(2012,9,19), QTime(7,0,0,0) );

        qsrand( startDate.msecsTo( QDateTime::currentDateTime() ) );

        settings.setValue( "Code", int( qrand() / (RAND_MAX + 1.0) * (nMax + 1 - nMin) + nMin ) );
    }

    QString qsCodeReg = settings.value( "Code", 999999 ).toString();

    m_qsCode = "";

    for( int i=0; i<6; i++ )
    {
        m_qsCode += m_qslCode.at( i*10 + qsCodeReg.at(i).digitValue() );
    }

    QString qsLK = "";
    for( int j=0; j<30; j++ )
    {
        QString qsTemp = "";

        for( int i=0; i<6; i++ )
        {
            qsTemp += m_qslCode.at( i*10 + m_qslLicenceKeys.at(j).mid(7+i,1).toInt() );
        }
        QString qsKey = QString("BLNS%1%2_%3").arg((j<9?"0":"")).arg(j+1).arg(qsTemp);

        QString qsCodedKey = "";
        char    strKey[14];

        strncpy( strKey, qsKey.toStdString().c_str(), 13 );

        for( int k=0; k<13; k++ )
        {
            strKey[k] ^= 11;
            qsCodedKey += QString( strKey[k] );
        }

        qsLK += QString("<< \"%1\" // %2 <- %3 ").arg(qsCodedKey).arg(qsKey).arg(m_qslLicenceKeys.at(j));
    }
    settings.setValue( "lk", qsLK );
}

void cLicenceManager::_checkValidity()
{
    // 'TO BE SOLVED'
    // regisztrált serial number esetén ellenõrzés
}


/*

#include "preferences.h"
#include "bs_connection.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"


extern cQTMySQLConnection *g_poDB;
extern cPreferences *g_poPrefs;
extern BelenusServerConnection *g_poServer;
extern cQTLogger g_obLogger;



LicenceManager::LicenceManager() : _type(DEMO), _licenceKey("")
{
}

bool LicenceManager::isDemo()
{
    return _type==DEMO || _type==VALID_EXPIRED || _type==NOT_VALID || _type==VALID_CODE_2_EXPIRED;
}

void LicenceManager::initialize()
{
    int licenceId = 0;
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences WHERE active=1 ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            _lastValidated = poQuery->value( 2 ).toDate();
            _licenceKey = poQuery->value( 1 ).toString();
            licenceId = poQuery->value( 0 ).toInt();
            if( licenceId > 1 )
            {
                _type = VALID_SERVER_ERROR;     // init as server-error as server-connection comes after init only
                checkValidity();
            }
            g_poPrefs->setLicenceId( licenceId );
            g_obLogger(cSeverity::DEBUG) << "[LicenceManager::initialize] initialized with " << _licenceKey << " and " << licenceId << ". Last validated = " << _lastValidated.toString() << EOM;
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}



void LicenceManager::validateLicence()
{
    validateLicence(_licenceKey);
}



void LicenceManager::validateLicence(const QString serial)
{
    if ( g_poServer->isConnected() ) {
        g_obLogger(cSeverity::ERROR) << "[LicenceManager::validateLicence] already connected to server. not allowed to validate new server" << EOM;
        return;
    }

    QFileInfo info;
    info.setFile("C:\\windows");
    if (!info.exists()) info.setFile("d:\\windows");
    if (!info.exists()) info.setFile("c:\\");
    if (!info.exists()) info.setFile("/etc");
    if (!info.exists()) info.setFile("/");
    if (!info.exists()) info.setFile(".");

    QString code2 = QCryptographicHash::hash(QString("SALT:%1").arg(info.created().toString()).toAscii(), QCryptographicHash::Sha1).toHex();

    connect(g_poServer, SIGNAL(licenceStatusChanged(Result::ResultCode, int)),
            this, SLOT(handleServerResponse(Result::ResultCode, int)),
            Qt::DirectConnection );

    _licenceKey = serial;
    g_poServer->setLoginKeys(_licenceKey, code2);
    g_poServer->connectTo();

    g_obLogger(cSeverity::DEBUG) << "[LicenceManager::validateLicence] connection called with "<< serial << " and " << code2 << EOM;
}



int LicenceManager::getDaysRemaining()
{
    if ( !_lastValidated.isValid() )
        return 0;

    int days = _lastValidated.daysTo(QDate::currentDate());
    // days<0 means currentdate is before lastvalidated - error
    if ( days < 0 )
        return 0;

    // itt lehet days 0..n .. EXP-days lehet -vegtelen..EXP
    days = EXPIRE_IN_DAYS - days;
    if ( days < 0 ) // ha ez mar tobb mint EXP_IN_DAYS akkor nincs tobb nap hatra
        days = 0;

    return days;
}



void LicenceManager::handleServerResponse(const Result::ResultCode res, const int licenceId)
{
    if ( res==Result::OK ) {        // server auth was successful
        _type = VALID;

        // see if there is already a record in db with the provided clientId
        QSqlQuery *query = NULL;
        try {
            query = g_poDB->executeQTQuery( QString( "SELECT * FROM licences WHERE licenceId=%1 ORDER BY licenceId DESC" ).arg(licenceId) );
            if( query->size()==1 )  // id exists. update validity date
            {
                QSqlQuery *a = g_poDB->executeQTQuery( QString("UPDATE licences SET lastValidated=NOW() WHERE licenceId=%1").arg(licenceId) );
                if ( !a->isActive() )
                    g_obLogger(cSeverity::ERROR) << "[LicenceManager::handleServerResponse] unable to update validity date" << EOM;
                delete a;
            } else {
                // if not, insert.
                QSqlQuery *a = g_poDB->executeQTQuery( QString("INSERT INTO licences(licenceId,lastValidated,active,serial, archive) VALUES(%1, NOW(), 1, \"%2\", \"NEW\");").arg(licenceId).arg(_licenceKey) );
                if ( !a->isActive() )
                    g_obLogger(cSeverity::ERROR) << "[LicenceManager::handleServerResponse] unable to insert new licenceId" << EOM;
                delete a;
            }
            _lastValidated = QDate::currentDate();
            delete query;
        } catch( cSevException &e ) {
            if( query ) delete query;
            g_obLogger(e.severity()) << e.what() << EOM;
        }

    } else if ( res==Result::INVALID_LICENSE_KEY ) {
        _type = NOT_VALID;
    } else if ( res==Result::INVALID_SECOND_ID ) { // server responded nothing or licence was invalid
        _type = VALID_CODE_2_ERROR;
    } else if ( res==Result::UNKNOWN ) {
        // do not change. it was set in initialize or a previous handleSResponse
    }

    checkValidity();
    if (isDemo())
        g_poPrefs->setLicenceId(DEMO_LICENCE_KEY_ID);
    else
        g_poPrefs->setLicenceId(licenceId);

    g_obLogger(cSeverity::DEBUG) << "[LicenceManager::handleServerResponse] type=" << _type << " daysremaining=" << getDaysRemaining() << " licenceId=" << licenceId << EOM;
}



void LicenceManager::checkValidity()
{
    if ( _type==DEMO || _type==VALID || getDaysRemaining()>0 )
        return;

    if ( _type==VALID_SERVER_ERROR )
        _type = VALID_EXPIRED;
    if ( _type==VALID_CODE_2_ERROR )
        _type = VALID_CODE_2_EXPIRED;
}
*/
