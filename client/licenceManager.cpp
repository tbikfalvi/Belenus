//============================================================================================================================
//
// Liszensz manager
//
//============================================================================================================================

#include <QSettings>
#include <QDateTime>

//----------------------------------------------------------------------------------------------------------------------------

#include "../framework/qtframework.h"
#include "licenceManager.h"
#include "preferences.h"
#include "../framework/qtmysqlconnection.h"
#include "../framework/qtlogger.h"
#include "general.h"

//----------------------------------------------------------------------------------------------------------------------------

extern cPreferences        *g_poPrefs;
extern cQTMySQLConnection  *g_poDB;
extern cQTLogger            g_obLogger;
extern cGeneral             g_obGen;

//============================================================================================================================
cLicenceManager::cLicenceManager()
//----------------------------------------------------------------------------------------------------------------------------
{
    m_uiLicenceId       = 1;
    m_qsLicenceString   = "BLNS_SERIAL_DEMO";
    m_qdLastValidated   = QDate::currentDate();
    m_qsState           = "DEMO";
    m_LicenceType       = cLicenceManager::LTYPE_DEMO;
}

//============================================================================================================================
cLicenceManager::~cLicenceManager()
//----------------------------------------------------------------------------------------------------------------------------
{
}

//============================================================================================================================
void cLicenceManager::initialize()
//----------------------------------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::INFO) << "Licence initialization started" << EOM;

    QSqlQuery   *poQuery = NULL;

    try
    {
        // Get the actually used licence data
        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_uiLicenceId       = poQuery->value( 0 ).toUInt();
            m_qsLicenceString   = poQuery->value( 1 ).toString();
            m_qdLastValidated   = poQuery->value( 9 ).toDate();
            m_qsState           = poQuery->value( 10 ).toString();
        }

        g_poPrefs->setLicenceId( m_uiLicenceId );

        g_obLogger(cSeverity::INFO) << "Initialization finished successfully" << EOM;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

//============================================================================================================================
unsigned int cLicenceManager::licenceID()
//----------------------------------------------------------------------------------------------------------------------------
{
    return m_uiLicenceId;
}

//============================================================================================================================
QString cLicenceManager::licenceSerialString()
//----------------------------------------------------------------------------------------------------------------------------
{
    return m_qsLicenceString;
}

//============================================================================================================================
cLicenceManager::licenceType cLicenceManager::licenceState()
//----------------------------------------------------------------------------------------------------------------------------
{
    if( m_qsState.compare( "VALIDATED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_VALIDATED;
    }
    else if( m_qsState.compare( "UNVALIDATED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_UNVALIDATED;
    }
    else if( m_qsState.compare( "UNREGISTERED" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_UNREGISTERED;
    }
    else if( m_qsState.compare( "INVALID" ) == 0 )
    {
        m_LicenceType = cLicenceManager::LTYPE_INVALID;
    }
    else
    {
        m_LicenceType = cLicenceManager::LTYPE_DEMO;
    }

    return m_LicenceType;
}

//============================================================================================================================
cLicenceManager::licenceType cLicenceManager::checkLicenceState()
//----------------------------------------------------------------------------------------------------------------------------
{
    QSqlQuery   *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT type FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_qsState = poQuery->value( 0 ).toString();
        }
        g_obLogger(cSeverity::DEBUG) << "Licence current state: " << m_qsState << EOM;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    return licenceState();
}

//============================================================================================================================
void cLicenceManager::deactivate()
//----------------------------------------------------------------------------------------------------------------------------
{
    try
    {
        m_qsState = "INVALID";
        m_LicenceType = cLicenceManager::LTYPE_INVALID;

        g_poDB->executeQTQuery( QString( "UPDATE licences SET type='%1' WHERE licenceId = %2 " ).arg( m_qsState ).arg( m_uiLicenceId ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
}

/*
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
                                     << "C_RCX^"  // BLNS60_HTYHSU <- BLNS60_794780
                                     << "CEAN_S"  // BLNS61_HNJETX <- BLNS61_736493
                                     << "BGSBEA"  // BLNS62_ILXINJ <- BLNS62_813836
                                     << "AE_IZ]"  // BLNS63_JNTBQV <- BLNS63_937161
                                     << "IXQAYD"  // BLNS64_BSZJRO <- BLNS64_185972
                                     << "CYDBDS"  // BLNS65_HROIOX <- BLNS65_772843
                                     << "BZ@HY^"  // BLNS66_IQKCRU <- BLNS66_868270
                                     << "N[^J[S"  // BLNS67_EPUAPX <- BLNS67_450053
                                     << "I@AO[A"  // BLNS68_BKJDPJ <- BLNS68_106356
                                     << "L@AIZR"  // BLNS69_GKJBQY <- BLNS69_606164
                                     << "MYRLDQ"  // BLNS70_FRYGOZ <- BLNS70_574645
                                     << "N_RCZ^"  // BLNS71_ETYHQU <- BLNS71_494760
                                     << "H_SNX_"  // BLNS72_CTXEST <- BLNS72_293487
                                     << "HDSO@^"  // BLNS73_COXDKU <- BLNS73_243300
                                     << "C_DHFA"  // BLNS74_HTOCMJ <- BLNS74_792226
                                     << "A[]AZR"  // BLNS75_JPVJQY <- BLNS75_951964
                                     << "LXSLED"  // BLNS76_GSXGNO <- BLNS76_683632
                                     << "BZSLEQ"  // BLNS77_IQXGNZ <- BLNS77_863635
                                     << "HERNED"  // BLNS78_CNYENO <- BLNS78_234432
                                     << "ID^CXR"  // BLNS79_BOUHSY <- BLNS79_140784
                                     << "HD^NY^"  // BLNS80_COUERU <- BLNS80_240470
                                     << "NGJCGA"  // BLNS81_ELAHLJ <- BLNS81_419716
                                     << "IGAMD]"  // BLNS82_BLJFOV <- BLNS82_116541
                                     << "HYDNX@"  // BLNS83_CROESK <- BLNS83_272488
                                     << "CX@LX@"  // BLNS84_HSKGSK <- BLNS84_788688
                                     << "LD^BDQ"  // BLNS85_GOUIOZ <- BLNS85_640845
                                     << "AY]OEA"  // BLNS86_JRVDNJ <- BLNS86_971336
                                     << "OYACF_"  // BLNS87_DRJHMT <- BLNS87_376727
                                     << "H[^HZR"  // BLNS88_CPUCQY <- BLNS88_250264
                                     << "M[QCFA"  // BLNS89_FPZHMJ <- BLNS89_555726
                                     << "I_]IER"  // BLNS90_BTVBNY <- BLNS90_191134
                                     << "CGRBZ@"  // BLNS91_HLYIQK <- BLNS91_714868
                                     << "LYSM@^"  // BLNS92_GRXFKU <- BLNS92_673500
                                     << "HX^MF]"  // BLNS93_CSUFMV <- BLNS93_280521
                                     << "NYSMED"  // BLNS94_ERXFNO <- BLNS94_473532
                                     << "O@SIF^"  // BLNS95_DKXBMU <- BLNS95_303120
                                     << "AYSLYQ"  // BLNS96_JRXGRZ <- BLNS96_973675
                                     << "MXROXD"  // BLNS97_FSYDSO <- BLNS97_584382
                                     << "OZSHY@"  // BLNS98_DQXCRK <- BLNS98_363278
                                     << "LDJH@^"; // BLNS99_GOACKU <- BLNS99_649200


    m_qslLicenceKeys1 = QStringList()<< "A@SIZ^"  // BLNL01_JKXBQU <- BLNL01_903160
                                     << "HX@L_S"  // BLNL02_CSKGTX <- BLNL02_288693
                                     << "HZSAZ^"  // BLNL03_CQXJQU <- BLNL03_263960
                                     << "MX_IZA"  // BLNL04_FSTBQJ <- BLNL04_587166
                                     << "I_QHFA"  // BLNL05_BTZCMJ <- BLNL05_195226
                                     << "IE@LF@"  // BLNL06_BNKGMK <- BLNL06_138628
                                     << "HEJHEA"  // BLNL07_CNACNJ <- BLNL07_239236
                                     << "LXDH@]"  // BLNL08_GSOCKV <- BLNL08_682201
                                     << "OFDH[A"  // BLNL09_DMOCPJ <- BLNL09_322256
                                     << "C@SMF^"  // BLNL10_HKXFMU <- BLNL10_703520
                                     << "B[@M_@"  // BLNL11_IPKFTK <- BLNL11_858598
                                     << "AFQMZ^"  // BLNL12_JMZFQU <- BLNL12_925560
                                     << "C[@MXQ"  // BLNL13_HPKFSZ <- BLNL13_758585
                                     << "BD_N@^"  // BLNL14_IOTEKU <- BLNL14_847400
                                     << "OX]B_A"  // BLNL15_DSVITJ <- BLNL15_381896
                                     << "O_DB_D"  // BLNL16_DTOITO <- BLNL16_392892
                                     << "A_RBG^"  // BLNL17_JTYILU <- BLNL17_994810
                                     << "O@]O_R"  // BLNL18_DKVDTY <- BLNL18_301394
                                     << "HZJBX^"  // BLNL19_CQAISU <- BLNL19_269880
                                     << "H[_A@^";  // BLNL20_CPTJKU <- BLNL20_257900

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
                                     << "794780"
                                     << "736493"
                                     << "813836"
                                     << "937161"
                                     << "185972"
                                     << "772843"
                                     << "868270"
                                     << "450053"
                                     << "106356"
                                     << "606164"
                                     << "574645"
                                     << "494760"
                                     << "293487"
                                     << "243300"
                                     << "792226"
                                     << "951964"
                                     << "683632"
                                     << "863635"
                                     << "234432"
                                     << "140784"
                                     << "240470"
                                     << "419716"
                                     << "116541"
                                     << "272488"
                                     << "788688"
                                     << "640845"
                                     << "971336"
                                     << "376727"
                                     << "250264"
                                     << "555726"
                                     << "191134"
                                     << "714868"
                                     << "673500"
                                     << "280521"
                                     << "473532"
                                     << "303120"
                                     << "973675"
                                     << "584382"
                                     << "363278"
                                     << "649200";

    m_qslLicenceCodes1 =QStringList()<< "903160"
                                     << "288693"
                                     << "263960"
                                     << "587166"
                                     << "195226"
                                     << "138628"
                                     << "239236"
                                     << "682201"
                                     << "322256"
                                     << "703520"
                                     << "858598"
                                     << "925560"
                                     << "758585"
                                     << "847400"
                                     << "381896"
                                     << "392892"
                                     << "994810"
                                     << "301394"
                                     << "269880"
                                     << "257900";

    m_nLicenceId        = 0;
    m_qsLicenceString   = "";
    m_qsAct             = "";
    m_qsCod             = "";
    m_qsCode            = "";
}

cLicenceManager::~cLicenceManager()
{

}

void cLicenceManager::initialize()
{
    QSqlQuery   *poQuery = NULL;

    try
    {
        // Get the actually used licence data
        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated, Act, Cod FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
        if( poQuery->first() )
        {
            m_nLicenceId        = poQuery->value( 0 ).toUInt();
            m_qsLicenceString   = poQuery->value( 1 ).toString();
            m_qdLastValidated   = poQuery->value( 2 ).toDate();
            m_qsAct             = poQuery->value( 3 ).toString();
            m_qsCod             = poQuery->value( 4 ).toString();
        }
        g_obLogger(cSeverity::INFO) << "Initialized with " << m_qsLicenceString
                                    << " and Licence " << m_nLicenceId
                                    << ". Application is valid until " << m_qdLastValidated.toString("yyyy/MM/dd")
                                    << " with code [" << m_qsCod << "]"
                                    << EOM;

        // Check secret code and create if not exists for non-demo licence id
        _checkCode();

        // If licence id is not demo
        if( m_nLicenceId > 1 )
        {
            m_LicenceType = LTYPE_REGISTERED;
            _checkValidity();
        }
        g_poPrefs->setLicenceId( m_nLicenceId );

        // Get last validated from settings and update with licence date if it has default value
        m_qdLicenceLastValidated = QDate::fromString( g_poPrefs->getLicenceLastValidated().left(10), "yyyy-MM-dd" );

        if( m_qdLicenceLastValidated.year() == 2000 &&
            m_qdLicenceLastValidated.month() == 1 &&
            m_qdLicenceLastValidated.day() == 1 )
        {
            g_poPrefs->setLicenceLastValidated( QString( "%1 12:00:00" ).arg( m_qdLastValidated.toString("yyyy-MM-dd") ) );
        }
        g_obLogger(cSeverity::INFO) << "Initialization finished" << EOM;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
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
    if ( !m_qdLastValidated.isValid() || !m_qdLicenceLastValidated.isValid() )
        return 0;

    QDate   qdCurrent       = QDate::currentDate();
    int     nDaysToExpire   = 0;

    if( m_qdLicenceLastValidated < m_qdLastValidated )
    {
        nDaysToExpire = qdCurrent.daysTo( m_qdLicenceLastValidated );
    }
    else
    {
        nDaysToExpire = qdCurrent.daysTo( m_qdLastValidated );
    }

    g_obLogger(cSeverity::INFO) << "nDaysToExpire: " << nDaysToExpire << EOM;

    if ( nDaysToExpire < 0 ) // ha ez mar tobb mint EXP_IN_DAYS akkor nincs tobb nap hatra
        nDaysToExpire = 0;

    return nDaysToExpire;
}

void cLicenceManager::validateApplication( QString p_qsDate )
{
    m_qdLastValidated = QDate::fromString( p_qsDate, "yyyy-MM-dd" );

    g_poDB->executeQTQuery( QString("UPDATE licences SET lastValidated=\"%1\" WHERE licenceId=%2").arg( p_qsDate ).arg( g_poPrefs->getLicenceId() ) );
}

int cLicenceManager::activateLicence(const QString &p_qsLicenceString , bool p_bChangeLicence )
{
    g_obLogger(cSeverity::INFO) << "Activating licence: " << p_qsLicenceString << EOM;
    int nRet = ERR_NO_ERROR;

    g_obLogger(cSeverity::DEBUG) << "Checking licence length" << EOM;
    if( p_qsLicenceString.length() != 13 )
    {
        g_obLogger(cSeverity::INFO) << "Licence - " << p_qsLicenceString << " - length: " << p_qsLicenceString.length() << EOM;
        return ERR_KEY_FORMAT_MISMATCH;
    }

    g_obLogger(cSeverity::DEBUG) << "Checking licence format" << EOM;
    if( (p_qsLicenceString.left(4).toUpper().compare("BLNS") && p_qsLicenceString.left(4).toUpper().compare("BLNL")) ||
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

    g_obLogger(cSeverity::DEBUG) << "Encoding last 6 character" << EOM;
    char    strLicenceRandomCode[7];

    strLicenceRandomCode[6] = 0;
    strncpy( strLicenceRandomCode, p_qsLicenceString.right(6).toStdString().c_str(), 6 );
    _EnCode( strLicenceRandomCode, 6 );

    g_obLogger(cSeverity::DEBUG) << "Encoded string: " << strLicenceRandomCode << EOM;

    int     nLicenceNumber = p_qsLicenceString.mid( 4, 2 ).toInt();

    g_obLogger(cSeverity::DEBUG) << "Licence number: " << nLicenceNumber << EOM;

    if( p_qsLicenceString.left(4).toUpper().compare("BLNS") == 0 )
    // Elso szaz liszensz kod
    {
        g_obLogger(cSeverity::DEBUG) << "First licence pack" << EOM;

        if( nLicenceNumber < 1 || nLicenceNumber > LICENCE_MAX_NUMBER )
        {
            g_obLogger(cSeverity::INFO) << "BLNS Licence order number is: " << nLicenceNumber << EOM;
            return ERR_KEY_NUMBER_INCORRECT;
        }

        if( m_qslLicenceKeys.at( nLicenceNumber-1 ).compare( QString( strLicenceRandomCode ) ) != 0 )
        {
            return ERR_KEY_NOT_EXISTS;
        }
    }
    else if( p_qsLicenceString.left(4).toUpper().compare("BLNL") == 0 )
    // Uj BLNL liszensz kodok
    {
        g_obLogger(cSeverity::DEBUG) << "Second licence pack" << EOM;

        if( nLicenceNumber < 1 || nLicenceNumber > LICENCE_MAX_NUMBER1 )
        {
            g_obLogger(cSeverity::INFO) << "BLNL Licence order number is: " << nLicenceNumber << EOM;
            return ERR_KEY_NUMBER_INCORRECT;
        }

        if( m_qslLicenceKeys1.at( nLicenceNumber-1 ).compare( QString( strLicenceRandomCode ) ) != 0 )
        {
            return ERR_KEY_NOT_EXISTS;
        }
    }

    if( nRet == ERR_NO_ERROR && m_qsLicenceString.compare( p_qsLicenceString ) != 0 )
    {
        m_nLicenceOrderNumber = nLicenceNumber;

        QSqlQuery   *poQuery = NULL;

        try
        {
            if( p_bChangeLicence )
            {
                poQuery = g_poDB->executeQTQuery( QString( "UPDATE licences SET `serial`=\"%1\" WHERE licenceId=%2" )
                                                  .arg( p_qsLicenceString )
                                                  .arg( g_poPrefs->getLicenceId() ) );
            }
            else
            {
                poQuery = g_poDB->executeQTQuery( QString( "INSERT INTO licences ( `licenceId`, `serial`, `lastValidated` ) VALUES ( %1, '%2', '%3' )" ).arg(nLicenceNumber+1).arg(p_qsLicenceString).arg(QDate::currentDate().toString("yyyy-MM-dd")) );
                if( poQuery ) g_poPrefs->setLicenceId( poQuery->lastInsertId().toUInt() );
            }
            m_qsLicenceString = p_qsLicenceString;
        }
        catch( cSevException &e )
        {
            if( poQuery ) delete poQuery;
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
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

    m_qsAct = qsCodeActivation;
    g_poDB->executeQTQuery( QString("UPDATE licences SET active=1, Act=\"%2\" WHERE licenceId=%1")
                            .arg( g_poPrefs->getLicenceId() )
                            .arg( m_qsAct ) );

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
    g_obLogger(cSeverity::DEBUG) << "Checking codes " << EOM;
    // Cod nem letezik, letre kell hozni
    if( m_nLicenceId > 1 && m_qsCod.length() < 1 )
    {
        int         nMin = 100000;
        int         nMax = 999999;
        QDateTime   startDate( QDate(2012,9,19), QTime(7,0,0,0) );

        qsrand( startDate.msecsTo( QDateTime::currentDateTime() ) );

        int nCode = int( qrand() / (RAND_MAX + 1.0) * (nMax + 1 - nMin) + nMin );

        g_poDB->executeQTQuery( QString( "UPDATE licences SET Cod=\"%1\" WHERE licenceId=%2 " )
                                        .arg( nCode )
                                        .arg( m_nLicenceId ) );
        m_qsCod = QString::number( nCode );

        m_qsCode = "";

        for( int i=0; i<6; i++ )
        {
            m_qsCode += m_qslCode.at( i*10 + m_qsCod.at(i).digitValue() );
        }
    }
    else if( m_qsCod.length() == 6 )
    {
        for( int i=0; i<6; i++ )
        {
            m_qsCode += m_qslCode.at( i*10 + m_qsCod.at(i).digitValue() );
        }
    }
}

void cLicenceManager::_checkValidity()
{
    m_LicenceType = LTYPE_ACTIVATED;
    return;

//-----------------------------------------------------------------------------------------------------------------------------------
// itt kezdődik az eredeti rész

    m_nLicenceOrderNumber = m_qsLicenceString.mid( 4, 2 ).toInt();

    if( m_nLicenceOrderNumber < 1 || m_nLicenceOrderNumber > LICENCE_MAX_NUMBER )
    {
        g_obLogger(cSeverity::ERROR) << "Invalid order number: " << m_nLicenceOrderNumber << EOM;
        return;
    }
    else
    {
        if( m_qsAct.length() > 0 )
        {
            QString qsCodeReg = m_qsCod;
            QString qsCodeAct = m_qsAct;
            QString qsCodeLic = m_qslLicenceCodes.at( m_nLicenceOrderNumber-1 );

            g_obLogger(cSeverity::DEBUG) << "L:" << qsCodeLic << EOM;   // Random number of the licence string decoded from last 6 digit
            g_obLogger(cSeverity::DEBUG) << "C:" << qsCodeReg << EOM;   // Random 6 digit number created after valid licence string added
            g_obLogger(cSeverity::DEBUG) << "A:" << qsCodeAct << EOM;   // Calculated once when Sys admin validates the licence string
                                                                        // from L and C with this algorythm -> A(i) = ( L(i) + C(i) ) % 10

            QString qsCodeVer = "";

            for( int i=0; i<6; i++ )
            {
                int nSum = QString( qsCodeReg.at(i) ).toInt() + QString( qsCodeLic.at(i) ).toInt();

                qsCodeVer.append( QString::number(nSum % 10) );
            }

            g_obLogger(cSeverity::DEBUG) << "V:" << qsCodeVer << EOM;   // Calculated the same like A
                                                                        // If application not validated or copied from another computer this shouldn't be the same like A
                                                                        // DB reload should delete the A value
            if( qsCodeVer.compare(qsCodeAct) == 0 )                     // A and V should be the same
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
    return "nem mukodik";

// ez új liszensz kódokat állít elő, nem lesz rá szükség

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

void cLicenceManager::refreshValidationDates()
{
    QSqlQuery   *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial, lastValidated FROM licences ORDER BY licenceId DESC LIMIT 1" ) );
    if( poQuery->first() )
    {
        m_qdLastValidated   = poQuery->value( 2 ).toDate();
    }

    m_qdLicenceLastValidated = QDate::fromString( g_poPrefs->getLicenceLastValidated().left(10), "yyyy-MM-dd" );
}
*/



