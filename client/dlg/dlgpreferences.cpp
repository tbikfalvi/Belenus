#include <QStringList>
#include <QDir>
#include <QRegExp>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QPixmap>
#include <QColor>
#include <QIcon>
#include <QColorDialog>
#include <QFileDialog>
#include "dlgpreferences.h"
#include "../framework/sevexception.h"
#include "dlgpanelappereance.h"

cDlgPreferences::cDlgPreferences( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPreferences::cDlgPreferences" );

    setupUi( this );

    m_nTimer        = 0;
    m_dlgProgress   = new cDlgProgress( this );

    setWindowTitle( tr( "Preferences" ) );
    setWindowIcon( QIcon("./resources/40x40_settings.png") );

    pbTestGibbig->setIcon( QIcon("./resources/40x40_check_connection.png") );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPreferences", QPoint(460,410) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    tbwPreferences->setCurrentIndex( 0 );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    unsigned int  uiConLevel, uiDBLevel, uiGUILevel, uiFileLevel;
    g_poPrefs->getLogLevels( &uiConLevel, &uiDBLevel, &uiGUILevel, &uiFileLevel );
    sliConsoleLogLevel->setValue( 1/*uiConLevel*/ );
    sliDBLogLevel->setValue( 1/*uiDBLevel*/ );
    sliGUILogLevel->setValue( uiGUILevel );
    sliFileLogLevel->setValue( uiFileLevel );

    QStringList obFilters( g_poPrefs->getLangFilePrefix() + "*.qm" );
    QDir        obLangDir( "lang" );
    QStringList obLangFiles = obLangDir.entryList( obFilters, QDir::Files | QDir::Readable, QDir::Name );
    QRegExp     obLangCodeRegExp( "^" + g_poPrefs->getLangFilePrefix() + "(\\w*)\\.qm$" );
    QStringList obLangCodes;
    for( int i = 0; i < obLangFiles.size(); i++ )
    {
        int inPos = obLangCodeRegExp.indexIn( obLangFiles[i] );
        if( inPos != -1 ) obLangCodes << obLangCodeRegExp.cap( 1 );
    }
    obLangCodes << "en";
    obLangCodes.sort();
    cmbAppLang->addItems( obLangCodes );
    m_inLangIdx = cmbAppLang->findText( g_poPrefs->getLang() );
    //if( m_inLangIdx == -1 ) m_inLangIdx = cmbAppLang->findText( "uk" );
    cmbAppLang->setCurrentIndex( m_inLangIdx );

    cmbDateFormat->setCurrentIndex(-1);
    int nDateFormatIndex = cmbDateFormat->findText( g_poPrefs->getDateFormat() );
    if( nDateFormatIndex < 1 ) nDateFormatIndex = 0;
    cmbDateFormat->setCurrentIndex( nDateFormatIndex );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"ABOUT_INFO_LINK\" " ) );
    poQuery->first();
    ledAboutLink->setText( poQuery->value(0).toString() );
    ledAboutLink->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

    ledBarcodePrefix->setValidator( new QIntValidator( ledBarcodePrefix ) );
    spbBarcodeLen->setValue( g_poPrefs->getBarcodeLength() );
    ledBarcodePrefix->setText( g_poPrefs->getBarcodePrefix() );
    chkCardProductBarcodeLength->setChecked( g_poPrefs->isBarcodeLengthDifferent() );

    spbPanels->setMaximum( g_poPrefs->getPanelCount() );
    spbPanels->setValue( g_poPrefs->getPanelsPerRow() );
    pbSecondaryWindow->setChecked( g_poPrefs->isSecondaryWindowVisible() );
    if( g_poPrefs->isSecondaryWindowVisible() )
    {
        pbSecondaryWindow->setIcon( QIcon("./resources/40x40_secondary_on.png") );
        lblSecondaryWindow->setText( tr("Visible") );
    }
    else
    {
        pbSecondaryWindow->setIcon( QIcon("./resources/40x40_secondary_off.png") );
        lblSecondaryWindow->setText( tr("Hidden") );
    }

    ledServerHost->setText( g_poPrefs->getServerAddress() );
    ledServerPort->setText( g_poPrefs->getServerPort() );
    chkDBAutoSynchron->setChecked( g_poPrefs->getDBAutoArchive() );
    chkDBGlobalAutoSynchron->setChecked( g_poPrefs->getDBGlobalAutoSynchronize() );

    spbCOM->setValue( g_poPrefs->getCommunicationPort() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getMainBackground() ) );
    btnMainBackground->setIcon( QIcon( obColorIcon ) );

    obColorIcon.fill( QColor( g_poPrefs->getSecondaryBackground() ) );
    btnSecondaryBackground->setIcon( QIcon( obColorIcon ) );

    obColorIcon.fill( QColor( g_poPrefs->getSecondaryFrame() ) );
    btnSecondaryFrame->setIcon( QIcon( obColorIcon ) );

    chkSecondaryCaption->setChecked( g_poPrefs->isSecondaryCaptionVisible() );

    ledVatPercent->setText( QString::number( g_poPrefs->getDeviceUseVAT() ) );
    ledDeviceAdminPassword->setEnabled( false );
    ledDeviceAdminPasswordCheck->setEnabled( false );
    pbModifyDevAdminPsw->setIcon( QIcon("./resources/40x40_key.png") );
    pbCancelModifyPsw->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbCancelModifyPsw->setVisible( false );

    chkAutoCloseCassa->setChecked( g_poPrefs->getCassaAutoClose() );
    chkCassaAutoWithdrawal->setChecked( g_poPrefs->getCassaAutoWithdrawal() );
    if( !chkAutoCloseCassa->isChecked() )
    {
        chkCassaAutoWithdrawal->setChecked( false );
        chkCassaAutoWithdrawal->setEnabled( false );
    }
    chkAutoOpenNewCassa->setChecked( g_poPrefs->getCassaAutoCreate() );
    rbCassaContinueWithBalance->setEnabled( g_poPrefs->getCassaAutoCreate() );
    rbCassaContinueWithoutBalance->setEnabled( g_poPrefs->getCassaAutoCreate() );
    switch( g_poPrefs->getCassaCreateType() )
    {
        case 1:
            rbCassaContinueWithBalance->setChecked( true );
            break;
        case 2:
            rbCassaContinueWithoutBalance->setChecked( true );
            break;
    }

    ledCurrencyFullName->setText( g_poPrefs->getCurrencyLong() );
    ledCurrencyShortName->setText( g_poPrefs->getCurrencyShort() );
    ledSeparatorDecimal->setText( g_poPrefs->getCurrencyDecimalSeparator() );
    ledSeparatorThousand->setText( g_poPrefs->getCurrencySeparator() );

    ledDefaultCountry->setText( g_poPrefs->getDefaultCountry() );

    connect( ledPCLostVatpercent, SIGNAL(textChanged(QString)), this, SLOT(on_ledPCLostPrice_textChanged(QString)) );
    connect( ledPCPartnerVatpercent, SIGNAL(textChanged(QString)), this, SLOT(on_ledPCPartnerPrice_textChanged(QString)) );

    cCurrency   cPrice( g_poPrefs->getPatientCardLostPrice(), cCurrency::CURR_GROSS, g_poPrefs->getPatientCardLostPriceVat() );

    ledPCLostPrice->setText( cPrice.currencyString() );
    ledPCLostVatpercent->setText( QString::number(g_poPrefs->getPatientCardLostPriceVat()) );

    cCurrency   cPricePartner( g_poPrefs->getPatientCardPartnerPrice(), cCurrency::CURR_GROSS, g_poPrefs->getPatientCardPartnerPriceVat() );

    ledPCPartnerPrice->setText( cPricePartner.currencyString() );
    ledPCPartnerVatpercent->setText( QString::number(g_poPrefs->getPatientCardPartnerPriceVat()) );

    pbPanelSettings->setIcon( QIcon("./resources/40x40_settings.png") );

    ledGibbigName->setText( g_poPrefs->getGibbigName() );
    ledGibbigPassword->setText( g_poPrefs->getGibbigPassword() );

    chkEnableGibbig->setChecked( g_poPrefs->isGibbigEnabled() );
    pbTestGibbig->setEnabled( g_poPrefs->isGibbigEnabled() );
    sbGibbigWaitTime->setEnabled( g_poPrefs->isGibbigEnabled() );
    sbGibbigWaitTime->setValue( g_poPrefs->getGibbigMessageWaitTime() );

    gbDBSynchron->setVisible( false );

//    btbButtons->standardButton( QDialogButtonBox::Ok ).setIcon( QIcon("./resources/40x40_ok.png") );

    ledBinaryLocation->setText( g_poPrefs->getDirDbBinaries() );
    ledBackupLocation->setText( g_poPrefs->getDirDbBackup() );
    chkBackupDatabase->setChecked( g_poPrefs->isForceBackupDatabase() );
    chkEnableDatabaseBackup->setChecked( g_poPrefs->isBackupDatabase() );

    chkMonday->setEnabled( false );
    chkTuesday->setEnabled( false );
    chkWednesday->setEnabled( false );
    chkThursday->setEnabled( false );
    chkFriday->setEnabled( false );
    chkSaturday->setEnabled( false );
    chkSunday->setEnabled( false );

    switch( g_poPrefs->getBackupDatabaseType() )
    {
        case 1:
            rbBackupOnExitIfConfirmed->setChecked( true );
            break;
        case 2:
            rbBackupOnExitCassaClose->setChecked( true );
            break;
        case 3:
            rbBackupOnEveryWeek->setChecked( true );
            break;
        case 4:
            rbBackupOnEveryMonth->setChecked( true );
            break;
        case 5:
            rbBackupOnExitDays->setChecked( true );
            break;
    }

    chkMonday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkTuesday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkWednesday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkThursday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkFriday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkSaturday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkSunday->setEnabled( chkEnableDatabaseBackup->isChecked() );

    QString qsBackupDays = g_poPrefs->getBackupDatabaseDays();

    chkMonday->setChecked( qsBackupDays.contains(tr("Mon"), Qt::CaseInsensitive) );
    chkTuesday->setChecked( qsBackupDays.contains(tr("Tue"), Qt::CaseInsensitive) );
    chkWednesday->setChecked( qsBackupDays.contains(tr("Wed"), Qt::CaseInsensitive) );
    chkThursday->setChecked( qsBackupDays.contains(tr("Thu"), Qt::CaseInsensitive) );
    chkFriday->setChecked( qsBackupDays.contains(tr("Fri"), Qt::CaseInsensitive) );
    chkSaturday->setChecked( qsBackupDays.contains(tr("Sat"), Qt::CaseInsensitive) );
    chkSunday->setChecked( qsBackupDays.contains(tr("Sun"), Qt::CaseInsensitive) );

    chkFapad->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    chkFapad->setChecked( g_poPrefs->isFapados() );

    if( g_poPrefs->isFapados() )
    {
        pbPanelSettings->setEnabled( g_obUser.isInGroup( cAccessGroup::SYSTEM ) );

        tbwPreferences->setTabEnabled( 5, g_obUser.isInGroup( cAccessGroup::SYSTEM ) );
    }
}

cDlgPreferences::~cDlgPreferences()
{
    delete m_dlgProgress;

    g_poPrefs->setDialogSize( "EditPreferences", QPoint( width(), height() ) );
}

void cDlgPreferences::timerEvent(QTimerEvent *)
{
    killTimer( m_nTimer );
    m_nTimer = 0;

    setCursor( Qt::ArrowCursor);

    if( g_poGibbig->gibbigIsErrorOccured() )
    {
        QMessageBox::warning( this, tr("Warning"),
                              tr("Authentication with server failed.\n"
                                 "Please check your server and user settings.") );
    }
}

void cDlgPreferences::on_sliConsoleLogLevel_valueChanged( int p_inValue )
{
    lblConsoleLogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)p_inValue ) );
}

void cDlgPreferences::on_sliDBLogLevel_valueChanged( int p_inValue )
{
    lblDBLogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)p_inValue ) );
}

void cDlgPreferences::on_sliGUILogLevel_valueChanged( int p_inValue )
{
    lblGUILogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)p_inValue ) );
}

void cDlgPreferences::on_sliFileLogLevel_valueChanged( int p_inValue )
{
    lblFileLogLevelValue->setText( cSeverity::toStr( (cSeverity::teSeverity)p_inValue ) );
}

void cDlgPreferences::on_btnMainBackground_clicked( bool )
{
    QColor obNewColor = QColorDialog::getColor( QColor( g_poPrefs->getMainBackground() ), this );
    if( obNewColor.isValid() ) g_poPrefs->setMainBackground( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getMainBackground() ) );
    btnMainBackground->setIcon( QIcon( obColorIcon ) );
}

void cDlgPreferences::on_chkAutoCloseCassa_clicked()
{
    if( chkAutoCloseCassa->isChecked() )
    {
        chkCassaAutoWithdrawal->setEnabled( true );
    }
    else
    {
        chkCassaAutoWithdrawal->setEnabled( false );
        chkCassaAutoWithdrawal->setChecked( false );
    }
}

void cDlgPreferences::on_spbBarcodeLen_valueChanged( int p_inValue )
{
    ledBarcodePrefix->setMaxLength( p_inValue - 1 );
}

void cDlgPreferences::accept()
{
    if( ledSeparatorDecimal->text().compare( ledSeparatorThousand->text() ) == 0 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("Decimal symbol and Digit grouping symbol can not be the same.") );
        return;
    }
    if( spbBarcodeLen->value() != g_poPrefs->getBarcodeLength() )
    {
        if( spbBarcodeLen->value() > g_poPrefs->getBarcodeLength() &&
            QMessageBox::question( this, tr("Question"),
                                   tr("Are you sure want to increase the length of the barcode of the patientcard?\n\n"
                                      "Please note that the barcodes will be supplemented with additional "
                                      "'0' characters at the beginning of the barcode for every patientcards, "
                                      "where the barcode is shorter than %1 characters.").arg( spbBarcodeLen->value() ),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
        {
            g_poPrefs->setBarcodeLength( spbBarcodeLen->value(), true );
            _increasePatientCardBarcodes();
        }
        else if( spbBarcodeLen->value() < g_poPrefs->getBarcodeLength() )
        {
            int nRet = g_obGen.customMsgBox( this, cGeneral::MSG_WARNING,
                                          tr(" Cut the beginning | Cut the end | Abort  "),
                                          tr("Are you sure you want to decrease the length of the barcode "
                                             "of the patientcard?\n\n"
                                             "Please note that all of the patientcards will be affected"
                                             "where the length of the barcode is longer than %1 characters").arg( spbBarcodeLen->value() ),
                                          tr("If you select 'Cut the beginning' the first %1 characters will "
                                             "be removed from the beginning of the barcode\n"
                                             "If you select 'Cut the end' the last %1 characters will"
                                             "be removed at the end of the barcode").arg( g_poPrefs->getBarcodeLength()-spbBarcodeLen->value() ) );
            switch( nRet )
            {
                case 1:
                {
                    g_poPrefs->setBarcodeLength( spbBarcodeLen->value(), true );
                    _decreasePatientCardBarcodes( true );
                    break;
                }
                case 2:
                {
                    g_poPrefs->setBarcodeLength( spbBarcodeLen->value(), true );
                    _decreasePatientCardBarcodes( false );
                    break;
                }
                default:
                    return;
            }
        }
    }

    g_poPrefs->setLogLevels( sliConsoleLogLevel->value(),
                             sliDBLogLevel->value(),
                             sliGUILogLevel->value(),
                             sliFileLogLevel->value() );

    g_poPrefs->setLang( cmbAppLang->currentText() );
    if( m_inLangIdx != cmbAppLang->currentIndex() )
    {
        g_obGen.setApplicationLanguage( cmbAppLang->currentText() );
        _updateDatabaseLanguage();
//        QMessageBox::information( this, tr( "Information" ),
//                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );
    }


    g_poPrefs->setPanelsPerRow( spbPanels->value() );
    g_poPrefs->setBarcodeLength( spbBarcodeLen->value() );
    g_poPrefs->setBarcodePrefix( ledBarcodePrefix->text() );
    g_poPrefs->setBarcodeLengthDifferent( chkCardProductBarcodeLength->isChecked() );

    g_poPrefs->setServerAddress( ledServerHost->text() );
    g_poPrefs->setServerPort( ledServerPort->text() );
    g_poPrefs->setDBAutoArchive( chkDBAutoSynchron->isChecked() );
    g_poPrefs->setDBGlobalAutoSynchronize( chkDBGlobalAutoSynchron->isChecked() );

    g_poPrefs->setCommunicationPort( spbCOM->value() );

    g_poPrefs->setDeviceUseVAT( ledVatPercent->text().toInt() );

    g_poPrefs->setCassaAutoClose( chkAutoCloseCassa->isChecked() );
    g_poPrefs->setCassaAutoWithdrawal( chkCassaAutoWithdrawal->isChecked() );
    g_poPrefs->setCassaAutoCreate( chkAutoOpenNewCassa->isChecked() );
    if( rbCassaContinueWithBalance->isChecked() )
        g_poPrefs->setCassaCreateType( 1 );
    else if( rbCassaContinueWithoutBalance->isChecked() )
        g_poPrefs->setCassaCreateType( 2 );
    g_poPrefs->setCurrencyLong( ledCurrencyFullName->text() );
    g_poPrefs->setCurrencyShort( ledCurrencyShortName->text() );
    g_poPrefs->setCurrencyDecimalSeparator( ledSeparatorDecimal->text() );
    g_poPrefs->setCurrencySeparator( ledSeparatorThousand->text() );

    g_poPrefs->setDefaultCountry( ledDefaultCountry->text() );

    g_poPrefs->setSecondaryWindowVisibility( pbSecondaryWindow->isChecked() );

    g_poPrefs->setSecondaryCaptionVisibility( chkSecondaryCaption->isChecked() );

    cCurrency   cPrice( ledPCLostPrice->text(), cCurrency::CURR_GROSS, ledPCLostVatpercent->text().toInt() );

    g_poPrefs->setPatientCardLostPrice( cPrice.currencyValue().toInt() );
    g_poPrefs->setPatientCardLostPriceVat( ledPCLostVatpercent->text().toInt() );

    cCurrency   cPricePartner( ledPCPartnerPrice->text(), cCurrency::CURR_GROSS, ledPCPartnerVatpercent->text().toInt() );

    g_poPrefs->setPatientCardPartnerPrice( cPricePartner.currencyValue().toInt() );
    g_poPrefs->setPatientCardPartnerPriceVat( ledPCPartnerVatpercent->text().toInt() );

    g_poPrefs->setGibbigName( ledGibbigName->text() );
    g_poPrefs->setGibbigPassword( ledGibbigPassword->text() );
    g_poPrefs->setGibbigEnabled( chkEnableGibbig->isChecked() );
    g_poPrefs->setGibbigMessageWaitTime( sbGibbigWaitTime->value() );

    if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"ABOUT_INFO_LINK\" " ).arg( ledAboutLink->text() ) );
    }

    g_poPrefs->setDirDbBinaries( ledBinaryLocation->text() );
    g_poPrefs->setDirDbBackup( ledBackupLocation->text() );
    g_poPrefs->setForceBackupDatabase( chkBackupDatabase->isChecked() );
    g_poPrefs->setBackupDatabase( chkEnableDatabaseBackup->isChecked() );

    if( rbBackupOnExitIfConfirmed->isChecked() )    g_poPrefs->setBackupDatabaseType( 1 );
    if( rbBackupOnExitCassaClose->isChecked() )     g_poPrefs->setBackupDatabaseType( 2 );
    if( rbBackupOnEveryWeek->isChecked() )          g_poPrefs->setBackupDatabaseType( 3 );
    if( rbBackupOnEveryMonth->isChecked() )         g_poPrefs->setBackupDatabaseType( 4 );
    if( rbBackupOnExitDays->isChecked() )           g_poPrefs->setBackupDatabaseType( 5 );

    QString qsBackupDays = "";

    if( chkMonday->isChecked() )    qsBackupDays.append( tr(" Mon") );
    if( chkTuesday->isChecked() )   qsBackupDays.append( tr(" Tue") );
    if( chkWednesday->isChecked() ) qsBackupDays.append( tr(" Wed") );
    if( chkThursday->isChecked() )  qsBackupDays.append( tr(" Thu") );
    if( chkFriday->isChecked() )    qsBackupDays.append( tr(" Fri") );
    if( chkSaturday->isChecked() )  qsBackupDays.append( tr(" Sat") );
    if( chkSunday->isChecked() )    qsBackupDays.append( tr(" Sun") );

    g_poPrefs->setBackupDatabaseDays( qsBackupDays );

    g_poPrefs->setDateFormat( cmbDateFormat->currentText() );

    g_poPrefs->setFapados( chkFapad->isChecked() );

    g_poPrefs->save();

    QDialog::accept();
}


void cDlgPreferences::on_pbPanelSettings_clicked()
{
    cDlgPanelAppereance     obDlgPanelAppereance( this );

    if( obDlgPanelAppereance.exec() == QDialog::Accepted )
    {
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );
    }
}

void cDlgPreferences::on_pbSecondaryWindow_toggled(bool checked)
{
    if( checked )
    {
        pbSecondaryWindow->setIcon( QIcon("./resources/40x40_secondary_on.png") );
        lblSecondaryWindow->setText( tr("Visible") );
    }
    else
    {
        pbSecondaryWindow->setIcon( QIcon("./resources/40x40_secondary_off.png") );
        lblSecondaryWindow->setText( tr("Hidden") );
    }
}

void cDlgPreferences::on_btnSecondaryBackground_clicked()
{
    QColor obNewColor = QColorDialog::getColor( QColor( g_poPrefs->getSecondaryBackground() ), this );
    if( obNewColor.isValid() ) g_poPrefs->setSecondaryBackground( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getSecondaryBackground() ) );
    btnSecondaryBackground->setIcon( QIcon( obColorIcon ) );
}

void cDlgPreferences::on_ledPCLostPrice_textChanged(const QString& /*arg1*/)
{
    cCurrency currPrice( ledPCLostPrice->text(), cCurrency::CURR_GROSS, ledPCLostVatpercent->text().toInt() );

    lblPCLostPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledPCLostVatpercent->text()) );
}

void cDlgPreferences::on_ledPCPartnerPrice_textChanged(const QString &/*arg1*/)
{
    cCurrency currPrice( ledPCPartnerPrice->text(), cCurrency::CURR_GROSS, ledPCPartnerVatpercent->text().toInt() );

    lblPCPartnerPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledPCPartnerVatpercent->text()) );
}


void cDlgPreferences::on_chkAutoOpenNewCassa_clicked()
{
    rbCassaContinueWithBalance->setEnabled( chkAutoOpenNewCassa->isChecked() );
    rbCassaContinueWithoutBalance->setEnabled( chkAutoOpenNewCassa->isChecked() );
}

void cDlgPreferences::on_chkEnableDatabaseBackup_clicked()
{
    rbBackupOnExitIfConfirmed->setEnabled( chkEnableDatabaseBackup->isChecked() );
    rbBackupOnExitCassaClose->setEnabled( chkEnableDatabaseBackup->isChecked() );
    rbBackupOnEveryWeek->setEnabled( chkEnableDatabaseBackup->isChecked() );
    rbBackupOnEveryMonth->setEnabled( chkEnableDatabaseBackup->isChecked() );
    rbBackupOnExitDays->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkMonday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkTuesday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkWednesday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkThursday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkFriday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkSaturday->setEnabled( chkEnableDatabaseBackup->isChecked() );
    chkSunday->setEnabled( chkEnableDatabaseBackup->isChecked() );
}

void cDlgPreferences::on_pbChangeBinaryLocation_clicked()
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       ledBinaryLocation->text(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qsDir.replace( '/', '\\' );

    if( qsDir.length() > 0 )
    {
        ledBinaryLocation->setText( qsDir );
    }
}

void cDlgPreferences::on_pbBackupLocation_clicked()
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       ledBackupLocation->text(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qsDir.replace( '/', '\\' );

    if( qsDir.length() > 0 )
    {
        ledBackupLocation->setText( qsDir );
    }
}

void cDlgPreferences::_updateDatabaseLanguage()
{
    g_poDB->executeQTQuery( QString("UPDATE agetypes SET ageTypeName=\"%1\" WHERE ageTypeId=0 ").arg( tr("Not defined") ) );
    g_poDB->executeQTQuery( QString("UPDATE agetypes SET ageTypeName=\"%1\" WHERE ageTypeId=1 ").arg( tr("Younger than 18") ) );
    g_poDB->executeQTQuery( QString("UPDATE agetypes SET ageTypeName=\"%1\" WHERE ageTypeId=7 ").arg( tr("Above 60") ) );

    g_poDB->executeQTQuery( QString("UPDATE genders SET genderName=\"%1\" WHERE genderId=0 ").arg( tr("Not defined") ) );
    g_poDB->executeQTQuery( QString("UPDATE genders SET genderName=\"%1\" WHERE genderId=1 ").arg( tr("Male") ) );
    g_poDB->executeQTQuery( QString("UPDATE genders SET genderName=\"%1\" WHERE genderId=2 ").arg( tr("Female") ) );

    g_poDB->executeQTQuery( QString("UPDATE skinTypes SET skinTypeName=\"%1\" WHERE skinTypeId=0 ").arg( tr("Not defined") ) );

    g_poDB->executeQTQuery( QString("UPDATE patientCardTypes SET name=\"%1\" WHERE patientCardTypeId=1 ").arg( tr("Service cards") ) );

    g_poDB->executeQTQuery( QString("UPDATE patientCards SET comment=\"%1\" WHERE patientCardId=1 ").arg( tr("Service card, only for service usage.") ) );

    g_poDB->executeQTQuery( QString("UPDATE panelgroups SET name=\"%1\" WHERE panelGroupId=0 ").arg( tr("<No group associated>") ) );

    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=1 ").arg( tr("Using solarium") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=2 ").arg( tr("Selling patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=3 ").arg( tr("Filling patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=4 ").arg( tr("Selling product") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=5 ").arg( tr("Replacing lost patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=6 ").arg( tr("Assign patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=7 ").arg( tr("Other") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=8 ").arg( tr("Other") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=9 ").arg( tr("Storage action") ) );
    g_poDB->executeQTQuery( QString("UPDATE ledgerTypes SET name=\"%1\" WHERE ledgerTypeId=10 ").arg( tr("Casssa expense") ) );

    g_poDB->executeQTQuery( QString("UPDATE productactiontype SET name=\"%1\" WHERE productActionTypeId=1 ").arg( tr("Add product to storage") ) );
    g_poDB->executeQTQuery( QString("UPDATE productactiontype SET name=\"%1\" WHERE productActionTypeId=2 ").arg( tr("Product disposal") ) );
    g_poDB->executeQTQuery( QString("UPDATE productactiontype SET name=\"%1\" WHERE productActionTypeId=3 ").arg( tr("Product donate") ) );

    g_poDB->executeQTQuery( QString("UPDATE paymentMethods SET name=\"%1\" WHERE paymentMethodId=1 ").arg( tr("Cash") ) );
    g_poDB->executeQTQuery( QString("UPDATE paymentMethods SET name=\"%1\" WHERE paymentMethodId=2 ").arg( tr("Credit card") ) );

    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=1 ").arg( tr("Guest entered into database") ) );
    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=2 ").arg( tr("Purchase patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=3 ").arg( tr("Refill patientcard") ) );
    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=4 ").arg( tr("Purchase product") ) );
    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=5 ").arg( tr("Using device with card") ) );
    g_poDB->executeQTQuery( QString("UPDATE patienthistorytype SET name=\"%1\" WHERE patientHistoryTypeId=6 ").arg( tr("Using device with cash") ) );
}

void cDlgPreferences::on_cmbDateFormat_currentIndexChanged(const QString &arg1)
{
    lblDateFormatExample->setText( tr( "(example %1)" ).arg( QDate::currentDate().toString(arg1) ) );
}

void cDlgPreferences::_increasePatientCardBarcodes()
{
    m_dlgProgress->showProgressBar( 10000 );
    m_dlgProgress->showProgress();

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardId FROM patientcards WHERE patientCardId>1 AND LENGTH(barcode)<%1" ).arg( g_poPrefs->getBarcodeLength() ) );

    m_dlgProgress->setMax( poQuery->size() );

    while( poQuery->next() )
    {
        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( poQuery->value(0).toUInt() );

        QString qsBarcode = obDBPatientCard.barcode();

        while( qsBarcode.length() < g_poPrefs->getBarcodeLength() )
        {
            qsBarcode = QString( "0%1" ).arg(qsBarcode);
        }

        obDBPatientCard.setBarcode( qsBarcode );
        obDBPatientCard.save();

        m_dlgProgress->stepProgressBar();
    }
    m_dlgProgress->hideProgress();
}

void cDlgPreferences::_decreasePatientCardBarcodes(bool p_bCutBegin)
{
    m_dlgProgress->showProgressBar( 10000 );
    m_dlgProgress->showProgress();

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardId FROM patientcards WHERE patientCardId>1 AND LENGTH(barcode)>%1" ).arg( g_poPrefs->getBarcodeLength() ) );

    m_dlgProgress->setMax( poQuery->size() );

    while( poQuery->next() )
    {
        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( poQuery->value(0).toUInt() );

        QString qsBarcode;

        if( p_bCutBegin )
        {
            qsBarcode = obDBPatientCard.barcode().right( g_poPrefs->getBarcodeLength() );
        }
        else
        {
            qsBarcode = obDBPatientCard.barcode().left( g_poPrefs->getBarcodeLength() );
        }

        obDBPatientCard.setBarcode( qsBarcode );
        obDBPatientCard.save();

        m_dlgProgress->stepProgressBar();
    }
    m_dlgProgress->hideProgress();
}

void cDlgPreferences::on_pbTestGibbig_clicked()
{
    g_poGibbig->gibbigAuthenticate();
    setCursor( Qt::WaitCursor);

    m_nTimer = startTimer( 5000 );
}

void cDlgPreferences::on_chkEnableGibbig_clicked(bool checked)
{
    pbTestGibbig->setEnabled( checked );
    sbGibbigWaitTime->setEnabled( checked );
}

void cDlgPreferences::on_btnSecondaryFrame_clicked()
{
    QColor obNewColor = QColorDialog::getColor( QColor( g_poPrefs->getSecondaryFrame() ), this );
    if( obNewColor.isValid() ) g_poPrefs->setSecondaryFrame( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getSecondaryFrame() ) );
    btnSecondaryFrame->setIcon( QIcon( obColorIcon ) );
}

void cDlgPreferences::on_pbModifyDevAdminPsw_clicked()
{
    if( ledDeviceAdminPassword->isEnabled() )
    {
        // Password modified
        if( ledDeviceAdminPassword->text().compare( ledDeviceAdminPasswordCheck->text() ) != 0 )
        {
            QMessageBox::critical( this, tr( "Error" ),
                                   tr( "Values of the Password and Retype Password fields are not the same" ) );
            return;
        }

        QByteArray  obPwdHash = QCryptographicHash::hash( ledDeviceAdminPassword->text().toAscii(), QCryptographicHash::Sha1 );

        if( g_poPrefs->checkExtendedAdminPassword("") )
        {
            g_poPrefs->setExtendedAdminPassword( QString( obPwdHash.toHex() ) );
        }
        else
        {
            g_poPrefs->createExtendedAdminPassword( QString( obPwdHash.toHex() ) );
        }
        on_pbCancelModifyPsw_clicked();
    }
    else
    {
        // Enable edit fields for modifications
        bool    bEnableModification = false;

        if( g_poPrefs->checkExtendedAdminPassword("") )
        {
            // Password already exists, request it to modify
            if( g_obGen.isExtendedOrSystemAdmin() )
            {
                bEnableModification = true;
            }
        }
        else
        {
            // Password not exists, request SysAdmin password
            if( g_obGen.isSystemAdmin() )
            {
                bEnableModification = true;
            }
        }

        ledDeviceAdminPassword->setEnabled( bEnableModification );
        ledDeviceAdminPasswordCheck->setEnabled( bEnableModification );
        pbCancelModifyPsw->setEnabled( bEnableModification );
        pbCancelModifyPsw->setVisible( bEnableModification );

        if( bEnableModification )
        {
            ledDeviceAdminPassword->setText( "" );
            ledDeviceAdminPasswordCheck->setText( "" );
            pbModifyDevAdminPsw->setIcon( QIcon("./resources/40x40_ok.png") );
        }
        else
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The password you entered is not valid\n"
                                     "to modify device admin password.") );
        }
    }
}

void cDlgPreferences::on_pbCancelModifyPsw_clicked()
{
    ledDeviceAdminPassword->setText( "" );
    ledDeviceAdminPasswordCheck->setText( "" );
    ledDeviceAdminPassword->setEnabled( false );
    ledDeviceAdminPasswordCheck->setEnabled( false );
    pbModifyDevAdminPsw->setIcon( QIcon("./resources/40x40_key.png") );
    pbCancelModifyPsw->setEnabled( false );
    pbCancelModifyPsw->setVisible( false );
}
