
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
                                     << "CZAJ_J"  // BLNS30_HQJATA <- BLNS30_766099
                                     << "N_@OF_"  // BLNS31_ETKDMT <- BLNS31_498327
                                     << "BYSHE]"  // BLNS32_IRXCNV <- BLNS32_873231
                                     << "HERIXR"  // BLNS33_CNYBSY <- BLNS33_234184
                                     << "H_@OYR"  // BLNS34_CTKDRY <- BLNS34_298374
                                     << "IEQC_A"  // BLNS35_BNZHTJ <- BLNS35_135796
                                     << "HDABYS"  // BLNS36_COJIRX <- BLNS36_246873
                                     << "M_JC_^"  // BLNS37_FTAHTU <- BLNS37_599790
                                     << "N[QBYD"  // BLNS38_EPZIRO <- BLNS38_455872
                                     << "LFAJX^"  // BLNS39_GMJASU <- BLNS39_626080
                                     << "IYJLDS"  // BLNS40_BRAGOX <- BLNS40_179643
                                     << "L[AC[R"  // BLNS41_GPJHPY <- BLNS41_656754
                                     << "LYAIDR"  // BLNS42_GRJBOY <- BLNS42_676144
                                     << "CY_C[^"  // BLNS43_HRTHPU <- BLNS43_777750
                                     << "O@RH@_"  // BLNS44_DKYCKT <- BLNS44_304207
                                     << "OG_I[R"  // BLNS45_DLTBPY <- BLNS45_317154
                                     << "IEAMD@"  // BLNS46_BNJFOK <- BLNS46_136548
                                     << "L@]HGA"  // BLNS47_GKVCLJ <- BLNS47_601216
                                     << "H_SOFR"  // BLNS48_CTXDMY <- BLNS48_293324
                                     << "OZSMX^"  // BLNS49_DQXFSU <- BLNS49_363580
                                     << "H__OEA"  // BLNS50_CTTDNJ <- BLNS50_297336
                                     << "CZQNGQ"  // BLNS51_HQZELZ <- BLNS51_765415
                                     << "HXDH[@"  // BLNS52_CSOCPK <- BLNS52_282258
                                     << "AD^ID^"  // BLNS53_JOUBOU <- BLNS53_940140
                                     << "CFSCEA"  // BLNS54_HMXHNJ <- BLNS54_723736
                                     << "H[SJ@Q"  // BLNS55_CPXAKZ <- BLNS55_253005
                                     << "A@^BF_"  // BLNS56_JKUIMT <- BLNS56_900827
                                     << "NZ_N@]"  // BLNS57_EQTEKV <- BLNS57_467401
                                     << "OX@M[_"  // BLNS58_DSKFPT <- BLNS58_388557
                                     << "M@AHE]"  // BLNS59_FKJCNV <- BLNS59_506231
                                     << "C_RCX^";  // BLNS60_HTYHSU <- BLNS60_794780

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
                                     << "766099"
                                     << "498327"
                                     << "873231"
                                     << "234184"
                                     << "298374"
                                     << "135796"
                                     << "246873"
                                     << "599790"
                                     << "455872"
                                     << "626080"
                                     << "179643"
                                     << "656754"
                                     << "676144"
                                     << "777750"
                                     << "304207"
                                     << "317154"
                                     << "136548"
                                     << "601216"
                                     << "293324"
                                     << "363580"
                                     << "297336"
                                     << "765415"
                                     << "282258"
                                     << "940140"
                                     << "723736"
                                     << "253005"
                                     << "900827"
                                     << "467401"
                                     << "388557"
                                     << "506231"
                                     << "794780";

    m_qsCode = "";
    m_qsAct  = "";
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

QString cLicenceManager::lastValidated()
{
    return m_qdLastValidated.toString( "yyyy-MM-dd" );
}

cLicenceManager::licenceType cLicenceManager::ltLicenceType()
{
    return m_LicenceType;
}

int cLicenceManager::daysRemain()
{
    if ( !m_qdLastValidated.isValid() )
        return 0;

    int nDays = m_qdLastValidated.daysTo( QDate::currentDate() );

    g_obLogger(cSeverity::INFO) << "nDays: " << nDays*(-1) << EOM;

    nDays = EXPIRE_IN_DAYS - nDays;
    if ( nDays < 0 ) // ha ez mar tobb mint EXP_IN_DAYS akkor nincs tobb nap hatra
        nDays = 0;

    return nDays;
}

void cLicenceManager::validateApplication( QString p_qsDate )
{
    m_qdLastValidated = QDate::fromString( p_qsDate, "yyyy-MM-dd" );

    g_poDB->executeQTQuery( QString("UPDATE licences SET lastValidated=\"%1\" WHERE licenceId=%2").arg( p_qsDate ).arg( g_poPrefs->getLicenceId() ) );
}

int cLicenceManager::activateLicence( const QString &p_qsLicenceString )
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

    if( nRet == ERR_NO_ERROR && m_qsLicenceString.compare( p_qsLicenceString ) != 0 )
    {
        m_nLicenceOrderNumber = nLicenceNumber;

        QSqlQuery   *poQuery = NULL;

        try
        {
            poQuery = g_poDB->executeQTQuery( QString( "INSERT INTO licences ( `licenceId`, `serial`, `lastValidated` ) VALUES ( %1, '%2', '%3' )" ).arg(nLicenceNumber+1).arg(p_qsLicenceString).arg(QDate::currentDate().toString("yyyy-MM-dd")) );
            if( poQuery ) g_poPrefs->setLicenceId( poQuery->lastInsertId().toUInt() );
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

int cLicenceManager::validateLicence(const QString &p_qsValidationString)
{
    cTracer obTrace( "cLicenceManager::validateLicence" );

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

    g_poDB->executeQTQuery( QString("UPDATE licences SET active=1 WHERE licenceId=%1").arg( g_poPrefs->getLicenceId() ) );

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

QString cLicenceManager::activationKey()
{
    QString qsAct = "";

    if( m_qsAct.length() == 6 )
    {
        for( int i=0; i<6; i++ )
        {
            qsAct.append( m_qslCode.at( i*10 + m_qsAct.at(i).digitValue() ) );
        }
    }

    return qsAct;
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
                m_qsAct = qsCodeAct;
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
            if( daysRemain() > 7 )
            {
                validateApplication( QDate::currentDate().addDays(7).toString("yyyy-MM-dd") );
            }
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

QString cLicenceManager::createLicenceKey( QString qsNumber )
{
    if( m_qslLicenceCodes.contains( qsNumber ) )
    {
        return "Already entered";
    }

    m_qslLicenceCodes << qsNumber;

    QString qsLK = "";
    QString qsTemp = "";

    if( qsNumber.length() != 6 )
    {
        return "Length should be 6";
    }

    for( int i=0; i<6; i++ )
    {
        qsTemp += m_qslCode.at( i*10 + qsNumber.mid(i,1).toInt() );
    }

    g_obLogger(cSeverity::DEBUG) << "LICENCE qsTemp: " << qsTemp << EOM;

    int j = m_qslLicenceCodes.count()-1;

    QString qsKey = QString("BLNS%1%2_%3").arg((j<9?"0":"")).arg(j+1).arg(qsTemp);

    g_obLogger(cSeverity::DEBUG) << "LICENCE qsKey: " << qsKey << EOM;

    QString qsCodedKey = "";
    char    strKey[14];

    strncpy( strKey, qsKey.toStdString().c_str(), 13 );

    for( int k=0; k<13; k++ )
    {
        strKey[k] ^= 11;
        qsCodedKey += QString( strKey[k] );
    }

    g_obLogger(cSeverity::DEBUG) << "LICENCE qsCode: " << qsCodedKey << EOM;

    qsLK += QString("<< \"%1\"  // %2 <- BLNS%3_%4 ").arg(qsCodedKey.mid(7,6)).arg(qsKey).arg(j+1).arg(qsNumber);

    return qsLK;
}
