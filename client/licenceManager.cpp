
#include <QSettings>
#include <QDateTime>

#include "../framework/qtframework.h"
#include "licenceManager.h"
#include "preferences.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"

extern cPreferences *g_poPrefs;
extern cQTMySQLConnection *g_poDB;
extern cQTLogger g_obLogger;

cLicenceManager::cLicenceManager()
{
    // Default licence type is DEMO
    m_LicenceType = LTYPE_DEMO;

    // The licence order number defines which item in the stringlists should be used
    // The value of the order number has to be greater than 0 and less than LICENCE_MAX_NUMBER+1
    m_nLicenceOrderNumber = 0;

    // Matrix for 6 element code string
    m_qslCode = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J"
                              << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T"
                              << "U" << "V" << "O" << "X" << "Y" << "Z" << "J" << "T" << "K" << "A"
                              << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J"
                              << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T"
                              << "U" << "V" << "O" << "X" << "Y" << "Z" << "J" << "T" << "K" << "A";

    // String array with same characters like matrix for easier decode activation code to numbers
    m_qslCodeString = QStringList() << "ABCDEFGHIJ"
                                    << "KLMNOPQRST"
                                    << "UVOXYZJTKA"
                                    << "ABCDEFGHIJ"
                                    << "KLMNOPQRST"
                                    << "UVOXYZJTKA";

    // Valid licence strings
    // The last 6 digit is a random number int the third column
    // The valid serial numbers located in the column in the middle
    // The serial number's last 6 character is encoded in the first column
    m_qslLicenceKeys = QStringList() << "O[@I_R"  // BLNS01_DPKBTY <- BLNS01_358194
                                     << "L[SHGD"  // BLNS02_GPXCLO <- BLNS02_653212
                                     << "IZ_L[D"  // BLNS03_BQTGPO <- BLNS03_167652
                                     << "BXDL@D"  // BLNS04_ISOGKO <- BLNS04_882602
                                     << "CYSNZ^"  // BLNS05_HRXEQU <- BLNS05_773460
                                     << "B__AX@"  // BLNS06_ITTJSK <- BLNS06_897988
                                     << "MXJJY^"  // BLNS07_FSAARU <- BLNS07_589070
                                     << "AEQIE_"  // BLNS08_JNZBNT <- BLNS08_935137
                                     << "OFRO__"  // BLNS09_DMYDTT <- BLNS09_324397
                                     << "AE]B@S"  // BLNS10_JNVIKX <- BLNS10_931803
                                     << "H[AM[_"  // BLNS11_CPJFPT <- BLNS11_256557
                                     << "CXJBYA"  // BLNS12_HSAIRJ <- BLNS12_789876
                                     << "A[DJ_Q"  // BLNS13_JPOATZ <- BLNS13_952095
                                     << "IERJ[]"  // BLNS14_BNYAPV <- BLNS14_134051
                                     << "ID]JF@"  // BLNS15_BOVAMK <- BLNS15_141028
                                     << "NDQJYD"  // BLNS16_EOZARO <- BLNS16_445072
                                     << "NXAHES"  // BLNS17_ESJCNX <- BLNS17_486233
                                     << "C@^IYA"  // BLNS18_HKUBRJ <- BLNS18_700176
                                     << "CY@N[_"  // BLNS19_HRKEPT <- BLNS19_778457
                                     << "N__MX]"  // BLNS20_ETTFSV <- BLNS20_497581
                                     << "M[@O@Q"  // BLNS21_FPKDKZ <- BLNS21_558305
                                     << "H[JB_]"  // BLNS22_CPAITV <- BLNS22_259891
                                     << "NFJHX^"  // BLNS23_EMACSU <- BLNS23_429280
                                     << "L_DJ[J"  // BLNS24_GTOAPA <- BLNS24_692059
                                     << "AEDH_Q"  // BLNS25_JNOCTZ <- BLNS25_932295
                                     << "OXAN@]"  // BLNS26_DSJEKV <- BLNS26_386401
                                     << "MX^JDA"  // BLNS27_FSUAOJ <- BLNS27_580046
                                     << "A@RAZ@"  // BLNS28_JKYJQK <- BLNS28_904968
                                     << "LXRJGA"  // BLNS29_GSYALJ <- BLNS29_684016
                                     << "CZAJ_J"; // BLNS30_HQJATA <- BLNS30_766099

    // These are the random values
    m_qslLicenceCodes = QStringList()<< "358194"
                                     << "653212"
                                     << "167652"
                                     << "882602"
                                     << "773460"
                                     << "897988"
                                     << "589070"
                                     << "935137"
                                     << "324397"
                                     << "931803"
                                     << "256557"
                                     << "789876"
                                     << "952095"
                                     << "134051"
                                     << "141028"
                                     << "445072"
                                     << "486233"
                                     << "700176"
                                     << "778457"
                                     << "497581"
                                     << "558305"
                                     << "259891"
                                     << "429280"
                                     << "692059"
                                     << "932295"
                                     << "386401"
                                     << "580046"
                                     << "904968"
                                     << "684016"
                                     << "766099";
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
        // Check secret code and create if not exists
        _checkCode();

        // Get latest serial string from database
        // licenceId is 0 for default
        // licenceId is 1 for DEMO
        // First valid serial's id is 2
//        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences WHERE active=1 ORDER BY licenceId DESC LIMIT 1" ) );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
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
            g_obLogger(cSeverity::INFO) << "Initialized with " << m_qsLicenceString << " and " << nLicenceId << ". Last validated = " << m_qdLastValidated.addMonths(6).toString("yyyy/MM/dd") << EOM;
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

    int nDays = QDate::currentDate().daysTo(m_qdLastValidated);
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
    int nRet = ERR_NO_ERROR;

    if( p_qsLicenceString.length() != 13 )
    {
        g_obLogger(cSeverity::INFO) << "Licence length: " << p_qsLicenceString.length() << EOM;
        return ERR_KEY_FORMAT_MISMATCH;
    }

    if( p_qsLicenceString.left(4).toUpper().compare("BLNS") ||
        !p_qsLicenceString.at(4).isDigit() ||
        !p_qsLicenceString.at(5).isDigit() ||
        p_qsLicenceString.at(6) != QChar('_') )
    {
        g_obLogger(cSeverity::INFO) << "Licence begins with " << p_qsLicenceString.left(4).toUpper() << EOM;
        g_obLogger(cSeverity::INFO) << "Licence 4 digit: " << (p_qsLicenceString.at(4).isDigit()?"true":"false") << EOM;
        g_obLogger(cSeverity::INFO) << "Licence 5 digit: " << (p_qsLicenceString.at(5).isDigit()?"true":"false") << EOM;
        g_obLogger(cSeverity::INFO) << "Licence 7th char is '" << p_qsLicenceString.at(6) << "'" << EOM;
        return ERR_KEY_FORMAT_MISMATCH;
    }

    char    strLicenceRandomCode[7];

    strLicenceRandomCode[6] = 0;
    strncpy( strLicenceRandomCode, p_qsLicenceString.right(6).toStdString().c_str(), 6 );
    _EnCode( strLicenceRandomCode, 6 );

    int     nLicenceNumber = p_qsLicenceString.mid( 4, 2 ).toInt();

    if( nLicenceNumber < 1 || nLicenceNumber > LICENCE_MAX_NUMBER )
    {
        g_obLogger(cSeverity::INFO) << "Licence order number is: " << nLicenceNumber << EOM;
        return ERR_KEY_NUMBER_INCORRECT;
    }

    if( m_qslLicenceKeys.at( nLicenceNumber-1 ).compare( QString( strLicenceRandomCode ) ) != 0 )
    {
        return ERR_KEY_NOT_EXISTS;
    }

    if( nRet == ERR_NO_ERROR )
    {
        m_nLicenceOrderNumber = nLicenceNumber;

        QSqlQuery   *poQuery = NULL;

        try
        {
            poQuery = g_poDB->executeQTQuery( QString( "INSERT INTO licences ( `licenceId`, `serial`, `lastValidated` ) VALUES ( %1, '%2', '%3' )" ).arg(nLicenceNumber+1).arg(p_qsLicenceString).arg(QDate::currentDate().toString("yyyy-MM-dd")) );
            if( poQuery ) g_poPrefs->setLicenceId( poQuery->lastInsertId().toUInt() );
            m_nLicenceId = nLicenceNumber;
            m_qsLicenceString = p_qsLicenceString;
        }
        catch( cSevException &e )
        {
            if( poQuery ) delete poQuery;
            g_obLogger(e.severity()) << e.what() << EOM;
        }
    }

    return nRet;
}

int cLicenceManager::activateLicence(const QString &p_qsValidationString)
{
    cTracer obTrace( "cLicenceManager::activateLicence" );

    int nRet = ERR_NO_ERROR;

    if( m_nLicenceOrderNumber < 1 || m_nLicenceOrderNumber > LICENCE_MAX_NUMBER+1 )
    {
        g_obLogger(cSeverity::INFO) << "Licence order number is: " << m_nLicenceOrderNumber << EOM;
        return ERR_KEY_NUMBER_INCORRECT;
    }
    if( p_qsValidationString.length() != 6 )
    {
        return ERR_KEY_FORMAT_MISMATCH;
    }

    QString qsCode = "";

    for( int i=0; i<m_qsCode.length(); i++ )
    {
        int nPos = m_qslCodeString.at(i).indexOf( m_qsCode.at(i), Qt::CaseInsensitive );

        if( nPos == -1 )
        {
            return ERR_ACT_KEY_INCORRECT;
        }
        qsCode.append( QString::number(nPos) );
    }

    QString qsCodeValidation = "";

    for( int i=0; i<p_qsValidationString.length(); i++ )
    {
        int nPos = m_qslCodeString.at(i).indexOf( p_qsValidationString.at(i), Qt::CaseInsensitive );

        if( nPos == -1 )
        {
            return ERR_ACT_KEY_INCORRECT;
        }
        qsCodeValidation.append( QString::number(nPos) );
    }

    QString qsCodeOrigin     = m_qslLicenceCodes.at( m_nLicenceOrderNumber-1 );
    QString qsCodeActivation = "";

    for( int i=0; i<6; i++ )
    {
        int nSum = QString( qsCodeOrigin.at(i) ).toInt() + QString( qsCode.at(i) ).toInt();

        qsCodeActivation.append( QString::number(nSum % 10) );
    }

    if( qsCodeActivation.compare(qsCodeValidation) )
    {
        return ERR_ACT_KEY_INCORRECT;
    }

    QSettings   settings( "HKEY_LOCAL_MACHINE\\Software\\SV", QSettings::NativeFormat );

    settings.setValue( "Active", qsCodeActivation );

    return nRet;
}

QString cLicenceManager::licenceKey() const
{
    return m_qsLicenceString;
}

QString cLicenceManager::validationKey() const
{
    int n1 = m_qsLicenceString.mid( 4, 1 ).toInt();
    int n2 = m_qsLicenceString.mid( 5, 1 ).toInt();

    return QString( "%1%2%3" ).arg( m_qslCode.at(n1) ).arg( m_qslCode.at(10 + n2) ).arg( m_qsCode );
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
/*
    // THIS CODE CREATES THE ENCODED BELENUS LICENCE KEY STRINGLIST
    // USE ONLY IF NEW CODES NEEDED
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
*/
}

void cLicenceManager::_checkValidity()
{
    m_nLicenceOrderNumber = m_qsLicenceString.mid( 4, 2 ).toInt();

    if( m_nLicenceOrderNumber < 1 || m_nLicenceOrderNumber > LICENCE_MAX_NUMBER )
    {
        g_obLogger(cSeverity::ERROR) << "Invalid order number: " << m_nLicenceOrderNumber << EOM;
        return;
    }
    else
    {
        QSettings   settings( "HKEY_LOCAL_MACHINE\\Software\\SV", QSettings::NativeFormat );

        if( settings.contains( "Active" ) )
        {
            QString qsCodeReg = settings.value( "Code", 999999 ).toString();
            QString qsCodeAct = settings.value( "Active", 999999 ).toString();
            QString qsCodeLic = m_qslLicenceCodes.at( m_nLicenceOrderNumber-1 );

            g_obLogger(cSeverity::DEBUG) << "L:" << qsCodeLic << EOM;
            g_obLogger(cSeverity::DEBUG) << "C:" << qsCodeReg << EOM;
            g_obLogger(cSeverity::DEBUG) << "A:" << qsCodeAct << EOM;

            QString qsCodeVer = "";

            for( int i=0; i<6; i++ )
            {
                int nSum = QString( qsCodeReg.at(i) ).toInt() + QString( qsCodeLic.at(i) ).toInt();

                qsCodeVer.append( QString::number(nSum % 10) );
            }

            g_obLogger(cSeverity::DEBUG) << "V:" << qsCodeVer << EOM;

            if( qsCodeVer.compare(qsCodeAct) == 0 )
            {
                m_LicenceType = LTYPE_ACTIVATED;
            }
            else
            {
                return;
            }
        }
        else
        {
            g_obLogger(cSeverity::INFO) << "Licence is not activated yet." << EOM;
        }
    }
}

void cLicenceManager::_EnCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}

void cLicenceManager::_DeCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
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
