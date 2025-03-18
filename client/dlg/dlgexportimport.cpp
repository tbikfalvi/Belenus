
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>

#include "dlgexportimport.h"
#include "dlgprogress.h"
#include "../db/dbpatientcardtype.h"
#include "../db/dbvalidtimeperiods.h"
#include "../db/dbpaneluses.h"
#include "../db/dbproducttype.h"
#include "../db/dbproduct.h"
#include "../db/dbdiscount.h"
#include "../db/dbskintypes.h"

cDlgExportImport::cDlgExportImport( QWidget *p_poParent, teProcessType p_teExportImport ) : QDialog( p_poParent )
{
    setupUi( this );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );
    pbDir->setIcon( QIcon("./resources/40x40_directory.png") );
    pbExecute->setIcon( QIcon("./resources/40x40_database_sync.png") );

    pbPrev->setEnabled( false );
    pbNext->setEnabled( false );
    pbExecute->setEnabled( false );

    pageWizard->setCurrentIndex( 0 );

    obProcessDoc    = new QDomDocument( "ExportImport" );
    m_dlgProgress   = new cDlgProgress( this );

    m_teExportImport = p_teExportImport;

    m_qsDir  = QDir::currentPath();
    m_qsFile = "";

    m_qsDir.replace( '/', '\\' );
    if( m_qsDir.right(1).compare("\\") == 0 )
    {
        m_qsDir = m_qsDir.left(m_qsDir.length()-1);
    }

    if( m_teExportImport == PT_EXPORT )
    {
        lblTitle_1->setText( tr("Data export") );
        lblTitle_2->setText( tr("Select destination directory") );

        rbGuest->setEnabled( true );

        chkPatientCardTypes->setVisible( false );
        chkPanelUses->setVisible( false );
        chkProductTypes->setVisible( false );
        chkProducts->setVisible( false );
        chkDiscounts->setVisible( false );
        chkSkinTypes->setVisible( false );
    }
    else
    {
        lblTitle_1->setText( tr("Data import") );
        lblTitle_2->setText( tr("Select file to be imported") );

        chkPatientCardTypes->setEnabled( true );
        chkPanelUses->setEnabled( true );
        chkProductTypes->setEnabled( true );
        chkProducts->setEnabled( true );
        chkDiscounts->setEnabled( true );
        chkSkinTypes->setEnabled( true );

        rbGuest->setVisible( false );
    }

    connect( rbGuest,              SIGNAL(clicked()), this, SLOT(slot_export_selected()) );
    connect( chkDiscounts,          SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
    connect( chkPanelUses,          SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
    connect( chkPatientCardTypes,   SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
    connect( chkProducts,           SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
    connect( chkProductTypes,       SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
    connect( chkSkinTypes,          SIGNAL(clicked()), this, SLOT(slot_import_selected()) );
}

cDlgExportImport::~cDlgExportImport()
{
    delete m_dlgProgress;
}

void cDlgExportImport::on_pbPrev_clicked()
{
    if( pageWizard->currentIndex() > 0 )
    {
        pageWizard->setCurrentIndex( pageWizard->currentIndex()-1 );
    }
    _processPage( false );
}

void cDlgExportImport::on_pbNext_clicked()
{
    if( pageWizard->currentIndex() < 2 )
    {
        pageWizard->setCurrentIndex( pageWizard->currentIndex()+1 );
    }
    _processPage();
}

void cDlgExportImport::on_pbExecute_clicked()
{
    if( m_teExportImport == PT_EXPORT )
    {
        if( rbGuest->isChecked() )
        {
            _ExportGuests();
        }
        QMessageBox::information( this, tr("Information"), tr("Export process has been finished.") );
    }
    else
    {
        if( !_LoadImportFile() ) return;

        if( chkPatientCardTypes->isChecked() )
        {
            _importPatientCardTypes();
        }
        if( chkPanelUses->isChecked() )
        {
            _importPanelUses();
        }
        if( chkProductTypes->isChecked() )
        {
            _importProductTypes();
        }
        if( chkProducts->isChecked() )
        {
            _importProducts();
        }
        if( chkDiscounts->isChecked() )
        {
            _importDiscounts();
        }
        if( chkSkinTypes->isChecked() )
        {
            _importSkinTypes();
        }
        QMessageBox::information( this, tr("Information"), tr("Import process has been finished") );
    }
}

void cDlgExportImport::on_pbExit_clicked()
{
    close();
}

void cDlgExportImport::slot_import_selected()
{
    if( chkDiscounts->isChecked() ||
        chkPanelUses->isChecked() ||
        chkPatientCardTypes->isChecked() ||
        chkProducts->isChecked() ||
        chkProductTypes->isChecked() ||
        chkSkinTypes->isChecked() )
    {
        pbNext->setEnabled( true );
    }
    else
    {
        pbNext->setEnabled( false );
        m_qsFile = "";
        lblComment->setText( "" );
    }
}

void cDlgExportImport::slot_export_selected()
{
    if( rbGuest->isChecked() )
    {
        pbNext->setEnabled( true );
    }
    else
    {
        pbNext->setEnabled( false );
        m_qsFile = "";
        lblComment->setText( "" );
    }
}

/*
void cDlgExportImport::on_rbGuest_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_GUEST;
    m_qsFile = "";
    lblComment->setText( "" );
}

void cDlgExportImport::on_rbPatientCard_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_PCARD;
    m_qsFile = "";
    lblComment->setText( "" );
    if( m_teExportImport == PT_IMPORT )
    {
        lblComment->setText( tr("Please note that only the barcode of the patientcards will be imported.\n"
                                "Duplicated barcodes or barcodes with invalid length will be skipped.") );
    }
}

void cDlgExportImport::on_rbProduct_clicked()
{
    pbNext->setEnabled( true );
    m_teData = DD_PROD;
    m_qsFile = "";
    lblComment->setText( "" );
    if( m_teExportImport == PT_IMPORT )
    {
        lblComment->setText( tr("Products with duplicated barcodes will be skipped.") );
    }
}
*/

void cDlgExportImport::_processPage( bool p_bMoveForward )
{
    switch( pageWizard->currentIndex() )
    {
        case 1:
        {
            if( p_bMoveForward )
            {
                if( m_teExportImport == PT_EXPORT )
                {
                    if( rbGuest->isChecked() )
                    {
                        m_qsFile = QString( "belenus_export_guest_%1.txt" ).arg( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
                    }
                }
                QString qsTarget = QString("%1\\%2").arg( m_qsDir ).arg( m_qsFile );
                ledTarget->setText( qsTarget );
                ledTarget->setToolTip( qsTarget );
            }
            pbPrev->setEnabled( true );
            pbNext->setEnabled( m_qsDir.length() > 0 && m_qsFile.length() > 0 );
            pbExecute->setEnabled( false );
            break;
        }
        case 2:
        {
            if( m_teExportImport == PT_EXPORT )
            {
                if( rbGuest->isChecked() )
                {
                    teInfo->setText( tr("Exporting guest data to the following file:\n\n"
                                        "%1").arg(ledTarget->text()) );
                }
            }
            else
            {
                QString qsImported = "";

                teInfo->append( tr("After clicking the Execute button, the application will import:\n") );
                if( chkPatientCardTypes->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("patientcard types") );
                }
                if( chkPanelUses->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("panel uses") );
                }
                if( chkProductTypes->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("product types") );
                }
                if( chkProducts->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("products") );
                }
                if( chkDiscounts->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("discounts") );
                }
                if( chkSkinTypes->isChecked() )
                {
                    if( qsImported.length() > 0 ) qsImported.append( ", " );
                    qsImported.append( tr("skin types") );
                }
                teInfo->append( qsImported );
            }
            pbPrev->setEnabled( true );
            pbNext->setEnabled( false );
            pbExecute->setEnabled( true );
            break;
        }
        case 0:
        default:
        {
            pbPrev->setEnabled( false );
            slot_import_selected();
            pbExecute->setEnabled( false );
            break;
        }
    }
}

void cDlgExportImport::on_pbDir_clicked()
{
    if( m_teExportImport == PT_EXPORT )
    {
        QString qsDir = QFileDialog::getExistingDirectory( this, tr("Select target directory"),
                                                           m_qsDir,
                                                           QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if( qsDir.length() > 0 )
        {
            m_qsDir = qsDir.replace( '/', '\\' );
            if( m_qsDir.right(1).compare("\\") == 0 )
            {
                m_qsDir = m_qsDir.left(m_qsDir.length()-1);
            }
        }
    }
    else
    {
        QFileDialog dlgFileOpen( this );

        dlgFileOpen.setDirectory( m_qsDir );
        dlgFileOpen.setFileMode( QFileDialog::ExistingFile );
        dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
        dlgFileOpen.setViewMode( QFileDialog::Detail );

        if( dlgFileOpen.exec() )
        {
            m_qsDir  = dlgFileOpen.directory().absolutePath();
            m_qsDir.replace( '/', '\\' );
            if( m_qsDir.right(1).compare("\\") == 0 )
            {
                m_qsDir = m_qsDir.left(m_qsDir.length()-1);
            }
            m_qsFile = dlgFileOpen.selectedFiles().at(0).right( dlgFileOpen.selectedFiles().at(0).length()-m_qsDir.length()-1 );
        }
    }

    _processPage();
}

void cDlgExportImport::_ExportGuests()
{
    m_dlgProgress->show();

    QStringList  qslLines;
    QSqlQuery   *poQuery = g_poDB->executeQTQuery( QString( "SELECT name, email, mobile, address FROM patients WHERE active=1 ORDER BY name" ) );

    while( poQuery->next() )
    {
        QString qsLine = "";

        qsLine.append( poQuery->value( 0 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 1 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 2 ).toString() );
        qsLine.append( "\t" );
        qsLine.append( poQuery->value( 3 ).toString() );
        qsLine.append( "\n" );

        qslLines << qsLine;
    }

    QFile   file( ledTarget->text() );

    file.open( QIODevice::WriteOnly );

    file.write( "#Guest name\tE-Mail\tPhone\tAddress\n" );

    for( int i=0; i<qslLines.count(); i++ )
    {
        file.write( qslLines.at(i).toStdString().c_str() );
    }
    file.close();

    m_dlgProgress->hide();
}

bool cDlgExportImport::_LoadImportFile()
{
    QFile   qfFile( m_qsFile );

    if( !qfFile.exists() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("The following file is missing:\n"
                                 "%1\n\n"
                                 "Please create the file and fulfill it with proper data.\n"
                                 "For more information about import file, check the manual\n"
                                 "or contact the application provider.").arg( m_qsFile ) );
        return false;
    }

    if( !qfFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Unable to read the following file:\n"
                                                      "%1").arg( m_qsFile ) );
        return false;
    }

    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;

    qfFile.seek( 0 );
    if( !obProcessDoc->setContent( &qfFile, &qsErrorMsg, &inErrorLine ) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "Error occured during parsing file:\n'%1'\n\nError in line %2: %3" )
                              .arg( m_qsFile ).arg( inErrorLine ).arg( qsErrorMsg ) );
        qfFile.close();
        return false;
    }
    qfFile.close();

    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles;
    QString         qsVerDB = docRoot.attribute( "version_db", "" );
    QString         qsVerApp= docRoot.attribute( "version_app", "" );

    if( qsVerDB.compare(g_poPrefs->getVersionDb()) || qsVerApp.compare(g_poPrefs->getVersion()) )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr( "The version information in import file is not match.\n\n"
                                  "Correct version numbers:\n"
                                  "Application version number: %1\n"
                                  "Database version number: %2\n\n"
                                  "Version numbers in import file:\n"
                                  "Application version number: %3\n"
                                  "Database version number: %4\n\n"
                                  "Please use import file with correct versions.")
                              .arg( g_poPrefs->getVersion() )
                              .arg( g_poPrefs->getVersionDb() )
                              .arg( qsVerApp )
                              .arg( qsVerDB ) );
        return false;
    }

    int nCountPCT   = 0;
    int nCountPU    = 0;
    int nCountPT    = 0;
    int nCountP     = 0;
    int nCountD     = 0;
    int nCountST    = 0;

    obFiles     = docRoot.elementsByTagName("patientcardtypes").at( 0 ).toElement().elementsByTagName( "patientcardtype" );
    nCountPCT   = obFiles.count();
    obFiles     = docRoot.elementsByTagName("paneluses").at( 0 ).toElement().elementsByTagName( "paneluse" );
    nCountPU    = obFiles.count();
    obFiles     = docRoot.elementsByTagName("producttypes").at( 0 ).toElement().elementsByTagName( "producttype" );
    nCountPT    = obFiles.count();
    obFiles     = docRoot.elementsByTagName("products").at( 0 ).toElement().elementsByTagName( "product" );
    nCountP     = obFiles.count();
    obFiles     = docRoot.elementsByTagName("discounts").at( 0 ).toElement().elementsByTagName( "discount" );
    nCountD     = obFiles.count();
    obFiles     = docRoot.elementsByTagName("skintypes").at( 0 ).toElement().elementsByTagName( "skintype" );
    nCountST    = obFiles.count();

    g_obLogger(cSeverity::DEBUG) << "Number of items found in import file PCT ["
                                 << nCountPCT
                                 << "] PU ["
                                 << nCountPU
                                 << "] PT ["
                                 << nCountPT
                                 << "] P ["
                                 << nCountP
                                 << "] D ["
                                 << nCountD
                                 << "] ST ["
                                 << nCountST
                                 << "]"
                                 << EOM;
    return true;
}

void cDlgExportImport::_importPatientCardTypes()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("patientcardtypes")
                              .at( 0 ).toElement().elementsByTagName( "patientcardtype" );

    teInfo->append( tr("\nPatientcard types imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBPatientCardType  obDBPatientCardType;

            cCurrency cPrice( obFiles.at(i).toElement().attribute("price"),
                                 cCurrency::CURR_GROSS,
                                 obFiles.at(i).toElement().attribute("vat").toInt() );

            obDBPatientCardType.setLicenceId( g_poPrefs->getLicenceId() );
            obDBPatientCardType.setName( obFiles.at(i).toElement().attribute("name") );
            obDBPatientCardType.setPrice( cPrice.currencyValue().toInt() );
            obDBPatientCardType.setVatpercent( obFiles.at(i).toElement().attribute("vat").toInt() );
            obDBPatientCardType.setUnits( obFiles.at(i).toElement().attribute("unit_count").toInt() );
            obDBPatientCardType.setValidDateFrom( obFiles.at(i).toElement().attribute("valid_from") );
            obDBPatientCardType.setValidDateTo( obFiles.at(i).toElement().attribute("valid_to") );
            obDBPatientCardType.setValidDays( obFiles.at(i).toElement().attribute("valid_days").toUInt() );
            obDBPatientCardType.setUnitTime( obFiles.at(i).toElement().attribute("unit_time").toInt() );
            obDBPatientCardType.setActive( true );

            if( obDBPatientCardType.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import patientcard type skipped. No name defined" << EOM;
                continue;
            }
            if( obDBPatientCardType.validDays() < 1 )
            {
                QDate   qdFrom = QDate::fromString( obDBPatientCardType.validDateFrom(), "yyyy-MM-dd" );
                QDate   qdTo   = QDate::fromString( obDBPatientCardType.validDateTo(), "yyyy-MM-dd" );

                if( qdTo <= QDate::currentDate() ||
                    qdFrom > qdTo )
                {
                    g_obLogger(cSeverity::WARNING) << "Import patientcard type skipped. Invalid dates defined" << EOM;
                    continue;
                }
            }

            obDBPatientCardType.save();

            QStringList qslPeriods;

            qslPeriods << tr("00:00 => 23:59 Mon Tue Wed Thu Fri Sat Sun");

            cDBValidTimePeriod  obDBValidTimePeriod;

            obDBValidTimePeriod.setLicenceId( obDBPatientCardType.licenceId() );
            obDBValidTimePeriod.setPatientCardTypeId( obDBPatientCardType.id() );
            obDBValidTimePeriod.saveList( qslPeriods );

            g_obLogger(cSeverity::INFO) << "Patientcard [" << obDBPatientCardType.name() << "] imported" << EOM;
            teInfo->append( obDBPatientCardType.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgExportImport::_importPanelUses()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("paneluses")
                              .at( 0 ).toElement().elementsByTagName( "paneluse" );

    teInfo->append( tr("\nPanel uses imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBPanelUses    obDBPanelUses;

            cCurrency cPrice( obFiles.at(i).toElement().attribute("price"), cCurrency::CURR_GROSS, 0 );

            obDBPanelUses.setLicenceId( g_poPrefs->getLicenceId() );
            //obDBPanelUses.setPanelId( 1 );
            obDBPanelUses.setName( obFiles.at(i).toElement().attribute("name") );
            obDBPanelUses.setUseTime( obFiles.at(i).toElement().attribute("minute").toInt() );
            obDBPanelUses.setUsePrice( cPrice.currencyValue().toInt() );

            if( obDBPanelUses.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import paneluse skipped. No name defined" << EOM;
                continue;
            }
            if( obDBPanelUses.useTime() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import paneluse skipped. Use time not defined" << EOM;
                continue;
            }
            if( cPrice.currencyValue().toInt() <= 0 )
            {
                g_obLogger(cSeverity::WARNING) << "Import paneluse skipped. Price not defined" << EOM;
                continue;
            }

            obDBPanelUses.save();

            g_obLogger(cSeverity::INFO) << "Panel use [" << obDBPanelUses.name() << "] imported" << EOM;
            teInfo->append( obDBPanelUses.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgExportImport::_importProductTypes()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("producttypes")
                              .at( 0 ).toElement().elementsByTagName( "producttype" );

    teInfo->append( tr("\nProduct types imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBProductType  obDBProductType;

            obDBProductType.setName( obFiles.at(i).toElement().attribute("name") );

            if( obDBProductType.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import product type skipped. No name defined" << EOM;
                continue;
            }

            obDBProductType.save();

            g_obLogger(cSeverity::INFO) << "Product type [" << obDBProductType.name() << "] imported" << EOM;
            teInfo->append( obDBProductType.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgExportImport::_importProducts()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("products")
                              .at( 0 ).toElement().elementsByTagName( "product" );

    teInfo->append( tr("\nProducts imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBProduct  obDBProduct;

            cCurrency cPriceBuy( obFiles.at(i).toElement().attribute("price_buy"),
                                    cCurrency::CURR_GROSS,
                                    obFiles.at(i).toElement().attribute("vat_buy").toInt() );
            cCurrency currPriceSell( obFiles.at(i).toElement().attribute("price_sell"),
                                     cCurrency::CURR_GROSS,
                                     obFiles.at(i).toElement().attribute("vat_sell").toInt() );

            obDBProduct.setName( obFiles.at(i).toElement().attribute("name") );
            obDBProduct.setBarcode( obFiles.at(i).toElement().attribute("barcode") );
            obDBProduct.setProductCount( obFiles.at(i).toElement().attribute("count").toUInt() );
            obDBProduct.setNetPriceBuy( cPriceBuy.currencyValue().toInt() );
            obDBProduct.setVatPercentBuy( obFiles.at(i).toElement().attribute("vat_buy").toInt() );
            obDBProduct.setNetPriceSell( currPriceSell.currencyValue().toUInt() );
            obDBProduct.setVatPercentSell( obFiles.at(i).toElement().attribute("vat_sell").toInt() );

            if( obDBProduct.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import product skipped. No name defined" << EOM;
                continue;
            }
            if( obDBProduct.barcode().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import product skipped. No barcode defined" << EOM;
                continue;
            }
            if( cPriceBuy.currencyValue().toInt() <= 0 )
            {
                g_obLogger(cSeverity::WARNING) << "Import product skipped. Price not defined" << EOM;
                continue;
            }

            obDBProduct.save();

            g_obLogger(cSeverity::INFO) << "Product [" << obDBProduct.name() << "] imported" << EOM;
            teInfo->append( obDBProduct.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgExportImport::_importDiscounts()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("discounts")
                              .at( 0 ).toElement().elementsByTagName( "discount" );

    teInfo->append( tr("\nDiscounts imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBDiscount  obDBDiscount;
            int          nDiscountType  = obFiles.at(i).toElement().attribute("disc_type").toInt();
            QString      qsValueType    = obFiles.at(i).toElement().attribute("value_type");
            QString      qsError        = "No name defined";

            obDBDiscount.setLicenceId( g_poPrefs->getLicenceId() );
            switch (nDiscountType)
            {
            case 1:
                obDBDiscount.setName( tr("Regular customer discount") );
                obDBDiscount.setRegularCustomer( true );
                break;
            case 2:
                obDBDiscount.setName( tr("Employee discount") );
                obDBDiscount.setEmployee( true );
                break;
            case 3:
                obDBDiscount.setName( tr("Service discount") );
                obDBDiscount.setService( true );
                break;
            case 4:
            {
                cDBGuest    obDBGuest;

                obDBGuest.load( obFiles.at(i).toElement().attribute("name") );
                if( obDBGuest.id() > 0 )
                {
                    obDBDiscount.setName( tr("Guest discount - %1")
                                          .arg( obFiles.at(i).toElement().attribute("name") ) );
                    obDBDiscount.setGuestId( obDBGuest.id() );
                }
                else
                {
                    obDBDiscount.setName( "" );
                    qsError = "Guest not found";
                }
                break;
            }
            case 6:
            {
                cDBProduct  obDBProduct;

                obDBProduct.loadName( obFiles.at(i).toElement().attribute("name") );
                if( obDBProduct.id() > 0 )
                {
                    obDBDiscount.setName( tr("Product discount - %1") );
                    obDBDiscount.setProductId( obDBProduct.id() );
                }
                else
                {
                    obDBDiscount.setName( "" );
                    qsError = "Product not found";
                }
                break;
            }
            case 8:
                obDBDiscount.setName( obFiles.at(i).toElement().attribute("name") );
                break;
            case 9:
                obDBDiscount.setName( tr("Time zone discount - %1 - %2")
                                      .arg( obFiles.at(i).toElement().attribute("time_start") )
                                      .arg( obFiles.at(i).toElement().attribute("time_stop") ) );
                obDBDiscount.setTimezoneStart( obFiles.at(i).toElement().attribute("time_start") );
                obDBDiscount.setTimezoneStop( obFiles.at(i).toElement().attribute("time_stop") );
                break;
            default:
                obDBDiscount.setName( "" );
                qsError = "Invalid discount type";
                break;
            }
            if( qsValueType.compare("fix") == 0 )
            {
                cCurrency   cValue( obFiles.at(i).toElement().attribute("value") );
                obDBDiscount.setDiscountValue( cValue.currencyValue().toInt() );
                obDBDiscount.setDiscountPercent( 0 );
            }
            else if( qsValueType.compare("percent") == 0 )
            {
                obDBDiscount.setDiscountValue( 0 );
                obDBDiscount.setDiscountPercent( obFiles.at(i).toElement().attribute("value").toInt() );
            }
            else
            {
                g_obLogger(cSeverity::WARNING) << "Import discount skipped. Invalid value type defined" << EOM;
                continue;
            }

            if( obDBDiscount.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import discount skipped. " << qsError << EOM;
                continue;
            }

            obDBDiscount.save();

            g_obLogger(cSeverity::INFO) << "Discount [" << obDBDiscount.name() << "] imported" << EOM;
            teInfo->append( obDBDiscount.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

void cDlgExportImport::_importSkinTypes()
{
    QDomElement     docRoot = obProcessDoc->documentElement();
    QDomNodeList    obFiles = docRoot.elementsByTagName("skintypes")
                              .at( 0 ).toElement().elementsByTagName( "skintype" );

    teInfo->append( tr("\nSkin types imported:") );

    for( int i=0; i<obFiles.count(); i++ )
    {
        try
        {
            cDBSkinTypes  obDBSkinType;

            obDBSkinType.setName( obFiles.at(i).toElement().attribute("name") );

            if( obDBSkinType.name().length() < 1 )
            {
                g_obLogger(cSeverity::WARNING) << "Import skin type skipped. No name defined" << EOM;
                continue;
            }

            obDBSkinType.save();

            g_obLogger(cSeverity::INFO) << "Skin type [" << obDBSkinType.name() << "] imported" << EOM;
            teInfo->append( obDBSkinType.name() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}

