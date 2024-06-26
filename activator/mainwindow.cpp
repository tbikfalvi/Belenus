
#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
                                     << "LDJH@^";  // BLNS99_GOACKU <- BLNS99_649200

    m_qslLicenceKeys1 = QStringList()<< "THG^AG"  // BLNL00_CLUJLA <- BLNL00_210919
                                     << "TA@SIZ"  // BLNL01_JKXBQU <- BLNL01_903160
                                     << "THX@L_"  // BLNL02_CSKGTX <- BLNL02_288693
                                     << "THZSAZ"  // BLNL03_CQXJQU <- BLNL03_263960
                                     << "TMX_IZ"  // BLNL04_FSTBQJ <- BLNL04_587166
                                     << "TI_QHF"  // BLNL05_BTZCMJ <- BLNL05_195226
                                     << "TIE@LF"  // BLNL06_BNKGMK <- BLNL06_138628
                                     << "THEJHE"  // BLNL07_CNACNJ <- BLNL07_239236
                                     << "TLXDH@"  // BLNL08_GSOCKV <- BLNL08_682201
                                     << "TOFDH["  // BLNL09_DMOCPJ <- BLNL09_322256
                                     << "TC@SMF"  // BLNL10_HKXFMU <- BLNL10_703520
                                     << "TB[@M_"  // BLNL11_IPKFTK <- BLNL11_858598
                                     << "TAFQMZ"  // BLNL12_JMZFQU <- BLNL12_925560
                                     << "TC[@MX"  // BLNL13_HPKFSZ <- BLNL13_758585
                                     << "TBD_N@"  // BLNL14_IOTEKU <- BLNL14_847400
                                     << "TOX]B_"  // BLNL15_DSVITJ <- BLNL15_381896
                                     << "TO_DB_"  // BLNL16_DTOITO <- BLNL16_392892
                                     << "TA_RBG"  // BLNL17_JTYILU <- BLNL17_994810
                                     << "TO@]O_"  // BLNL18_DKVDTY <- BLNL18_301394
                                     << "THZJBX"  // BLNL19_CQAISU <- BLNL19_269880
                                     << "TH[_A@"; // BLNL20_CPTJKU <- BLNL20_257900

    m_qslActiveKeys = QStringList()  << "BLNS01"    // Budapest             Szt. Istvan korut
                                     << "BLNS02"    // London               Beckhanem
                                     << "BLNS03"    // London               Sheenlane
                                     << "BLNS04"    // London               ButterSea park
                                     << "BLNS05"    // Mosonmagyarovar
                                     << "BLNS06"    // London               Putney
                                     << "BLNS07"    // Budapest             Margit krt
                                     << "BLNS08"    // London               Streatham
                                     << "BLNS09"    // Békéscsaba
                                     << "BLNS10"    // Budapest             Bartók Béla út
                                     << "BLNS11"    // Budapest             Kolosy tér
                                     << "BLNS12"    // Siófok
                                     << "BLNS13"    // Budapest             Ferenc krt.
                                     << "BLNS14"    // Budapest             Béke út
                                     << "BLNS15"    // München
                                     << "BLNS16"    // Debrecen
//                                   << "BLNS17"    // Kisvárda
                                     << "BLNS18"    // Rózsadomb
                                     << "BLNS19"    // Pécs
                                     << "BLNS20"    // Sopron
                                     << "BLNS21"    // Baja
                                     << "BLNS22"    // Székesfehérvár
                                     << "BLNS23"    // Dunaszerdahely
                                     << "BLNS24"    // Budapest             Kossuth L. u.
                                     << "BLNS25"    // Budapest             Keleti pu.
                                     << "BLNS26"    // Eger
                                     << "BLNS27"    // Miskolc
                                     << "BLNS28"    // Nyiregyhaza
                                     << "BLNS29"    // Kecskemét
//                                   << "BLNS30"    //                      Győri liszensz
                                     << "BLNS31"    // Szeged
                                     << "BLNS32"    // Veszprém
                                     << "BLNS33"    // Irorszag             Cork
                                     << "BLNS34"    // London               Mall
                                     << "BLNS35"    // Budapest             Október 23. u.
                                     << "BLNS36"    // Budakeszi
                                     << "BLNS37"    // London               Broomley
                                     << "BLNS38"    // Dunaujvaros
                                     << "BLNS39"    // Zalaegerszeg
                                     << "BLNS40"    // Tatabanya
                                     << "BLNS41"    // Budapest             Oktogon
                                     << "BLNS42"    // Budapest             Lőrinc
                                     << "BLNS43"    // Szombathely
                                     << "BLNS44"    // Gyöngyös
                                     << "BLNS45"    // Érd
                                     << "BLNS46"    // Budaörs
                                     << "BLNS47"    // Tata
                                     << "BLNS48"    // Besztercebánya
                                     << "BLNS49"    // Komárom
                                     << "BLNS50"    // Budapest             Erzsébet krt.
                                     << "BLNS51"    // Hódmezővásárhely
                                     << "BLNS52"    // Kalocsa
                                     << "BLNS53"    // Kiskörös
                                     << "BLNS54"    // Dabas
                                     << "BLNS55"    // Tarnaméra
                                     << "BLNS56"    // Orosháza
                                     << "BLNS57"    // Gödöllő
                                     << "BLNS58"    // Paks
                                     << "BLNS59"    // Irorszag             Dublin
                                     << "BLNS60"    // Csepel
                                     << "BLNS61"    // Dunakeszi
                                     << "BLNS62"    // Budapest Blaha
                                     << "BLNS63"    // Budapest   Rakos ut
                                     << "BLNS64"    // Romania    Nagyvarad
                                     << "BLNS65"    // Budapest             Csepel
                                     << "BLNS66"    // Budapest             Zuglo
                                     << "BLNS67"    // Torokbalint
                                     << "BLNS68"    // Dublin   Dun Laogharie
                                     << "BLNS69"    //
                                     << "BLNS70"    // Budapest             Alkotas
                                     << "BLNS71"    // Kolozsvar
                                     << "BLNS72"    // Verona
                                     << "BLNS73"    // Ajka
                                     << "BLNS74"    //
                                     << "BLNS75"    //
                                     << "BLNS76"    //
                                     << "BLNS77"    //
                                     << "BLNS78"    //
                                     << "BLNS79"    //
                                     << "BLNS80"    //
                                     << "BLNS81"    //
                                     << "BLNS82"    //
                                     << "BLNS83"    //
                                     << "BLNS84"    //
                                     << "BLNS85"    //
                                     << "BLNS86"    //
                                     << "BLNS87"    //
                                     << "BLNS88"    //
                                     << "BLNS89"    //
                                     << "BLNS90"    //
                                     << "BLNS91"    //
                                     << "BLNS92"    //
                                     << "BLNS93"    //
                                     << "BLNS94"    //
                                     << "BLNS95"    //
                                     << "BLNS96"    //
                                     << "BLNS97"    //
                                     << "BLNS98"    //
                                     << "BLNS99"    //
                                     << "BLNL00"
                                     << "BLNL01"
                                     << "BLNL02"
                                     << "BLNL03"
                                     << "BLNL04"
                                     << "BLNL05"
                                     << "BLNL06"
                                     << "BLNL07"
                                     << "BLNL08"
                                     << "BLNL09"
                                     << "BLNL10"
                                     << "BLNL11"
                                     << "BLNL12"
                                     << "BLNL13"
                                     << "BLNL14"
                                     << "BLNL15"
                                     << "BLNL16"
                                     << "BLNL17"
                                     << "BLNL18"
                                     << "BLNL19"
                                     << "BLNL20";

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
                                     << "649200"   // 99
                                     << "210919"
                                     << "903160"
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

//    _fillLicenceTree();

    QStringList qslHeader = QStringList() << tr("Licence string") << tr("Activation\ndate") << tr("Activation\ncode") << tr("Validation\ncode");

    ui->treeLicences->setHeaderLabels( qslHeader );

    ui->lblCheckCodeOk->setVisible( false );
    ui->lblCheckCodeCancel->setVisible( false );

    ui->lblLicenceKeyString->setText( "BLNS..." );
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent( QCloseEvent *p_poEvent )
{
//    if( QMessageBox::question( this, tr("Question"),
//                               tr("Are you sure you want to close the application?"),
//                               QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
//    {
        p_poEvent->accept();
//    }
//    else
//    {
//        p_poEvent->ignore();
//    }
}

void MainWindow::_EnCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}

void MainWindow::_DeCode( char *str, int size )
{
   for(int i=0;i<size;i++)
   {
      str[i] ^= 11;
   }
}

void MainWindow::on_treeLicences_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *)
{
    if( current != NULL )
    {
        ui->pbEditLicenceKey->setEnabled( true );
        ui->pbDeleteLicenceKey->setEnabled( true );
    }
    else
    {
        ui->pbEditLicenceKey->setEnabled( false );
        ui->pbDeleteLicenceKey->setEnabled( false );
    }
}


void MainWindow::on_pbNewLicenceKey_clicked()
{
    QString qsRegKey = ui->ledRegistrationCode->text();

    if( qsRegKey.length() != 8 )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Registration code length is invalid") );
        ui->lblCheckCodeCancel->setVisible( true );
        return;
    }

    int n1 = m_qslCodeString.at(0).indexOf( qsRegKey.at(0), Qt::CaseInsensitive );
    int n2 = m_qslCodeString.at(1).indexOf( qsRegKey.at(1), Qt::CaseInsensitive );

    int nOrder = n1*10+n2;

    if( nOrder > 0 && nOrder < LICENCE_MAX_NUMBER+1 )
    {
        if( !m_qslActiveKeys.contains( QString("BLNS%1%2").arg(n1).arg(n2) ) )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("This licence string is not valid.") );
            return;
        }

        char    strLicenceRandomCode[7];

        strLicenceRandomCode[6] = 0;
        strncpy( strLicenceRandomCode, m_qslLicenceKeys.at(nOrder-1).toStdString().c_str(), 6 );
        _EnCode( strLicenceRandomCode, 6 );

        m_qsCode = m_qslLicenceCodes.at(nOrder-1);

        ui->ledLicenceKeyName->setText( QString("BLNS%1%2_%3").arg(n1).arg(n2).arg(strLicenceRandomCode) );
//        ui->lblLicenceKeyString->setText( QString("BLNS%1%2_%3 (%4)").arg(n1).arg(n2).arg(strLicenceRandomCode).arg(m_qslLicenceCodes.at(nOrder-1)) );
        ui->lblLicenceKeyString->setText( QString("BLNS%1%2_%3").arg(n1).arg(n2).arg(strLicenceRandomCode) );
        ui->ledCode->setText( ui->ledRegistrationCode->text().right(6) );
        ui->deActivated->setDate( QDate::currentDate() );
        ui->pbValidateLicence->setFocus();
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"), tr("Registration code is invalid - BLNS%1%2").arg(n1).arg(n2) );
        ui->lblCheckCodeCancel->setVisible( true );
        return;
    }

    m_bLicenceModify = false;

    ui->treeLicences->setEnabled( false );

    ui->pbNewLicenceKey->setEnabled( false );
    ui->pbEditLicenceKey->setEnabled( false );
    ui->pbDeleteLicenceKey->setEnabled( false );

    ui->ledLicenceKeyName->setEnabled( true );
    ui->pbSaveLicence->setEnabled( true );
    ui->pbCancelLicence->setEnabled( true );
}

void MainWindow::on_pbEditLicenceKey_clicked()
{
    m_bLicenceModify = true;

    ui->treeLicences->setEnabled( false );

    ui->pbNewLicenceKey->setEnabled( false );
    ui->pbEditLicenceKey->setEnabled( false );
    ui->pbDeleteLicenceKey->setEnabled( false );

    ui->ledLicenceKeyName->setEnabled( true );
    ui->pbSaveLicence->setEnabled( true );
    ui->pbCancelLicence->setEnabled( true );

    QTreeWidgetItem *item = ui->treeLicences->currentItem();

    ui->ledLicenceKeyName->setText( item->text( 0 ) );

    int n1 = ui->ledLicenceKeyName->text().mid(4,1).toInt();
    int n2 = ui->ledLicenceKeyName->text().mid(5,1).toInt();

    int nOrder = n1*10+n2;

    m_qsCode = m_qslLicenceCodes.at(nOrder-1);
//    ui->lblLicenceKeyString->setText( QString("%1 (%2)").arg(ui->ledLicenceKeyName->text()).arg(m_qslLicenceCodes.at(nOrder-1)) );
    ui->lblLicenceKeyString->setText( QString("%1").arg(ui->ledLicenceKeyName->text()) );

    if( item->text(2).length() > 0 )
    {
        ui->deActivated->setDate( QDate::fromString( item->text(1), "yyyy/MM/dd" ) );
        ui->ledCode->setText( item->text(2) );
        ui->ledValidator->setText( item->text(3) );
    }
}

void MainWindow::on_pbDeleteLicenceKey_clicked()
{
    QTreeWidgetItem *item = ui->treeLicences->currentItem();

    ui->treeLicences->takeTopLevelItem( ui->treeLicences->indexOfTopLevelItem(item) );

    delete item;
}

void MainWindow::on_pbSaveLicence_clicked()
{
    if( m_bLicenceModify )
    {
        QTreeWidgetItem *item = ui->treeLicences->currentItem();
        item->setText( 0, ui->ledLicenceKeyName->text() );
        item->setText( 1, ui->deActivated->text() );
        item->setText( 2, ui->ledCode->text().left(6) );
        item->setText( 3, ui->ledValidator->text().left(6) );
    }
    else
    {
        QList<QTreeWidgetItem *> lstResult = ui->treeLicences->findItems( ui->ledLicenceKeyName->text(), Qt::MatchFixedString );

        if( lstResult.count() > 0 )
        {
            QMessageBox::warning( this, tr("Attention"), tr("Defined licence already exists.") );
            return;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem( ui->treeLicences );
        item->setData( 0, Qt::DisplayRole, ui->ledLicenceKeyName->text() );
        item->setData( 1, Qt::DisplayRole, ui->deActivated->text() );
        item->setData( 2, Qt::DisplayRole, ui->ledCode->text().left(6) );
        item->setData( 3, Qt::DisplayRole, ui->ledValidator->text().left(6) );
    }

    _saveLicenceTree();

    on_pbCancelLicence_clicked();
}

void MainWindow::on_pbCancelLicence_clicked()
{
//    ui->treeLicences->setEnabled( true );

    ui->pbNewLicenceKey->setEnabled( true );

    ui->ledLicenceKeyName->setEnabled( false );
    ui->pbSaveLicence->setEnabled( false );
    ui->pbCancelLicence->setEnabled( false );

    ui->ledLicenceKeyName->setText( "" );

//    ui->lblLicenceKeyString->setText( "BLNSxx_AAAAAA" );
    ui->lblLicenceKeyString->setText( "BLNS..." );
    ui->ledCode->setText( "" );
    ui->ledValidator->setText( "" );
//    ui->deActivated->setDate( QDate::currentDate() );

    ui->lblCheckCodeOk->setVisible( false );
    ui->lblCheckCodeCancel->setVisible( false );
}


void MainWindow::on_ledLicenceKeyName_textChanged(const QString&)
{
    ui->lblCheckCodeOk->setVisible( false );
    ui->lblCheckCodeCancel->setVisible( false );

    bool    bEnable = _validateLicenceStr( ui->ledLicenceKeyName->text() );

//    ui->ledCode->setEnabled( bEnable );
    ui->ledValidator->setEnabled( bEnable );
//    ui->deActivated->setEnabled( bEnable );
    ui->pbValidateLicence->setEnabled( bEnable );
    if( bEnable )
    {
        ui->lblCheckCodeOk->setVisible( true );
        ui->lblCheckCodeCancel->setVisible( false );
    }
    else
    {
        ui->lblCheckCodeOk->setVisible( false );
        ui->lblCheckCodeCancel->setVisible( true );
    }
}

bool MainWindow::_validateLicenceStr(QString p_qsLicenceString )
{
//    int nRet = ERR_NO_ERROR;

    if( p_qsLicenceString.length() != 13 )
    {
//        nRet = ERR_KEY_FORMAT_MISMATCH;
        return false;
    }

    if( p_qsLicenceString.left(4).toUpper().compare("BLNS") ||
        !p_qsLicenceString.at(4).isDigit() ||
        !p_qsLicenceString.at(5).isDigit() ||
        p_qsLicenceString.at(6) != QChar('_') )
    {
//        g_obLogger(cSeverity::INFO) << "Licence begins with " << p_qsLicenceString.left(4).toUpper() << EOM;
//        g_obLogger(cSeverity::INFO) << "Licence 4 digit: " << (p_qsLicenceString.at(4).isDigit()?"true":"false") << EOM;
//        g_obLogger(cSeverity::INFO) << "Licence 5 digit: " << (p_qsLicenceString.at(5).isDigit()?"true":"false") << EOM;
//        g_obLogger(cSeverity::INFO) << "Licence 7th char is '" << p_qsLicenceString.at(6) << "'" << EOM;
//        nRet = ERR_KEY_FORMAT_MISMATCH;
        return false;
    }

    char    strLicenceRandomCode[7];

    strLicenceRandomCode[6] = 0;
    strncpy( strLicenceRandomCode, p_qsLicenceString.right(6).toStdString().c_str(), 6 );
    _EnCode( strLicenceRandomCode, 6 );

    int     nLicenceNumber = p_qsLicenceString.mid( 4, 2 ).toInt();

    if( nLicenceNumber < 1 || nLicenceNumber > LICENCE_MAX_NUMBER )
    {
//        g_obLogger(cSeverity::INFO) << "Licence order number is: " << nLicenceNumber << EOM;
//        nRet = ERR_KEY_NUMBER_INCORRECT;
        return false;
    }

    if( m_qslLicenceKeys.at( nLicenceNumber-1 ).compare( QString( strLicenceRandomCode ) ) != 0 )
    {
//        nRet = ERR_KEY_NOT_EXISTS;
        return false;
    }

    return true;
}

void MainWindow::on_pbValidateLicence_clicked()
{
    QString qsCodeActivation = "";

    for( int i=0; i<ui->ledCode->text().length(); i++ )
    {
        int nPos = m_qslCodeString.at(i).indexOf( ui->ledCode->text().at(i), Qt::CaseInsensitive );

        if( nPos == -1 )
        {
            return; //ERR_ACT_KEY_INCORRECT;
        }
        qsCodeActivation.append( QString::number(nPos) );
    }

    ui->ledCode->setText( QString("%1 (%2)").arg(ui->ledCode->text()).arg(qsCodeActivation) );

    QString qsCodeValidation = "";

    for( int i=0; i<6; i++ )
    {
        int nSum = QString( qsCodeActivation.at(i) ).toInt() + QString( m_qsCode.at(i) ).toInt();

        qsCodeValidation.append( QString::number(nSum % 10) );
    }

    QString qsStringValidation = "";

    for( int i=0; i<6; i++ )
    {
        qsStringValidation.append( m_qslCode.at( i*10 + qsCodeValidation.at(i).digitValue() ) );
    }

//    ui->ledValidator->setText( QString("%1 (%2)").arg(qsStringValidation).arg(qsCodeValidation) );
    ui->ledValidator->setText( qsStringValidation );
}

void MainWindow::_fillLicenceTree()
{
    QFile   file( QString("licences.txt") );

    if( !file.open(QIODevice::ReadOnly) )
        return;

    QTextStream in(&file);

    while( !in.atEnd() )
    {
        QString     qsLine      = in.readLine();
        QStringList qslLicences = qsLine.split( "\t" );

        QTreeWidgetItem *item = new QTreeWidgetItem( ui->treeLicences );
        item->setData( 0, Qt::DisplayRole, qslLicences.at(0) );
        item->setData( 1, Qt::DisplayRole, qslLicences.at(1) );
        item->setData( 2, Qt::DisplayRole, qslLicences.at(2) );
        item->setData( 3, Qt::DisplayRole, qslLicences.at(3) );
    }

    file.close();
}

void MainWindow::_saveLicenceTree()
{
    QFile   file( QString("licences.txt") );

    file.open( QIODevice::WriteOnly );

    for( int i=0; i<ui->treeLicences->topLevelItemCount(); i++ )
    {
        QTreeWidgetItem *item = ui->treeLicences->topLevelItem(i);

        file.write( QString("%1\t%2\t%3\t%4\n").arg(item->text(0)).arg(item->text(1)).arg(item->text(2)).arg(item->text(3)).toStdString().c_str() );
    }
    file.close();
}

void MainWindow::on_ledRegistrationCode_textEdited(const QString &/*arg1*/)
{
    on_pbCancelLicence_clicked();
}
