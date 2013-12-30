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
#include "dlgpreferences.h"
#include "../framework/sevexception.h"
#include "dlgpanelappereance.h"

cDlgPreferences::cDlgPreferences( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPreferences::cDlgPreferences" );

    setupUi( this );

    setWindowTitle( tr( "Preferences" ) );
    setWindowIcon( QIcon("./resources/40x40_settings.png") );

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
    sliConsoleLogLevel->setValue( uiConLevel );
    sliDBLogLevel->setValue( uiDBLevel );
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
    //obLangCodes << "uk";
    obLangCodes.sort();
    cmbAppLang->addItems( obLangCodes );
    m_inLangIdx = cmbAppLang->findText( g_poPrefs->getLang() );
    //if( m_inLangIdx == -1 ) m_inLangIdx = cmbAppLang->findText( "uk" );
    cmbAppLang->setCurrentIndex( m_inLangIdx );

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

    ledVatPercent->setText( QString::number( g_poPrefs->getDeviceUseVAT() ) );

    chkAutoCloseCassa->setChecked( g_poPrefs->getCassaAutoClose() );
    chkCassaAutoWithdrawal->setChecked( g_poPrefs->getCassaAutoWithdrawal() );
    if( !chkAutoCloseCassa->isChecked() )
    {
        chkCassaAutoWithdrawal->setChecked( false );
        chkCassaAutoWithdrawal->setEnabled( false );
    }

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

//    btbButtons->standardButton( QDialogButtonBox::Ok ).setIcon( QIcon("./resources/40x40_ok.png") );
}

cDlgPreferences::~cDlgPreferences()
{
    g_poPrefs->setDialogSize( "EditPreferences", QPoint( width(), height() ) );
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
    g_poPrefs->setLogLevels( sliConsoleLogLevel->value(),
                             sliDBLogLevel->value(),
                             sliGUILogLevel->value(),
                             sliFileLogLevel->value() );

    g_poPrefs->setLang( cmbAppLang->currentText() );
    if( m_inLangIdx != cmbAppLang->currentIndex() )
    {
        g_obGen.setApplicationLanguage( cmbAppLang->currentText() );
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

    g_poPrefs->setDefaultCountry( ledDefaultCountry->text() );

    g_poPrefs->setSecondaryWindowVisibility( pbSecondaryWindow->isChecked() );

    cCurrency   cPrice( ledPCLostPrice->text(), cCurrency::CURR_GROSS, ledPCLostVatpercent->text().toInt() );

    g_poPrefs->setPatientCardLostPrice( cPrice.currencyValue().toInt() );
    g_poPrefs->setPatientCardLostPriceVat( ledPCLostVatpercent->text().toInt() );

    cCurrency   cPricePartner( ledPCPartnerPrice->text(), cCurrency::CURR_GROSS, ledPCPartnerVatpercent->text().toInt() );

    g_poPrefs->setPatientCardPartnerPrice( cPricePartner.currencyValue().toInt() );
    g_poPrefs->setPatientCardPartnerPriceVat( ledPCPartnerVatpercent->text().toInt() );

    g_poPrefs->save();

    if( g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"ABOUT_INFO_LINK\" " ).arg( ledAboutLink->text() ) );
    }

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

void cDlgPreferences::on_ledPCLostPrice_textChanged(const QString &arg1)
{
    cCurrency currPrice( ledPCLostPrice->text(), cCurrency::CURR_GROSS, ledPCLostVatpercent->text().toInt() );

    lblPCLostPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledPCLostVatpercent->text()) );
}

void cDlgPreferences::on_ledPCPartnerPrice_textChanged(const QString &arg1)
{
    cCurrency currPrice( ledPCPartnerPrice->text(), cCurrency::CURR_GROSS, ledPCPartnerVatpercent->text().toInt() );

    lblPCPartnerPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledPCPartnerVatpercent->text()) );
}

