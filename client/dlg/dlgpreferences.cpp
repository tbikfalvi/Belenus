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

cDlgPreferences::cDlgPreferences( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPreferences::cDlgPreferences" );

    setupUi( this );

    setWindowTitle( tr( "Preferences" ) );
    setWindowIcon( QIcon("./resources/40x40_settings.png") );

    unsigned int  uiConLevel, uiDBLevel, uiGUILevel;
    g_poPrefs->getLogLevels( &uiConLevel, &uiDBLevel, &uiGUILevel );
    sliConsoleLogLevel->setValue( uiConLevel );
    sliDBLogLevel->setValue( uiDBLevel );
    sliGUILogLevel->setValue( uiGUILevel );

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
    obLangCodes << "uk";
    obLangCodes.sort();
    cmbAppLang->addItems( obLangCodes );
    m_inLangIdx = cmbAppLang->findText( g_poPrefs->getLang() );
    if( m_inLangIdx == -1 ) m_inLangIdx = cmbAppLang->findText( "uk" );
    cmbAppLang->setCurrentIndex( m_inLangIdx );

    ledBarcodePrefix->setValidator( new QIntValidator( ledBarcodePrefix ) );
    spbBarcodeLen->setValue( g_poPrefs->getBarcodeLength() );
    ledBarcodePrefix->setText( g_poPrefs->getBarcodePrefix() );

    spbPanels->setMaximum( g_poPrefs->getPanelCount() );
    spbPanels->setValue( g_poPrefs->getPanelsPerRow() );

    ledServerHost->setText( g_poPrefs->getServerAddress() );
    ledServerPort->setText( g_poPrefs->getServerPort() );

    spbCOM->setValue( g_poPrefs->getCommunicationPort() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getMainBackground() ) );
    btnMainBackground->setIcon( QIcon( obColorIcon ) );

    ledVatPercent->setText( QString::number( g_poPrefs->getDeviceUseVAT() ) );

    chkAutoCloseCassa->setChecked( g_poPrefs->getCassaAutoClose() );

    ledDefaultCountry->setText( g_poPrefs->getDefaultCountry() );

//    btbButtons->standardButton( QDialogButtonBox::Ok ).setIcon( QIcon("./resources/40x40_ok.png") );
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

void cDlgPreferences::on_btnMainBackground_clicked( bool )
{
    QColor obNewColor = QColorDialog::getColor( QColor( g_poPrefs->getMainBackground() ), this );
    if( obNewColor.isValid() ) g_poPrefs->setMainBackground( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( g_poPrefs->getMainBackground() ) );
    btnMainBackground->setIcon( QIcon( obColorIcon ) );
}

void cDlgPreferences::on_spbBarcodeLen_valueChanged( int p_inValue )
{
    ledBarcodePrefix->setMaxLength( p_inValue - 1 );
}

void cDlgPreferences::accept()
{
    g_poPrefs->setLogLevels( sliConsoleLogLevel->value(),
                             sliDBLogLevel->value(),
                             sliGUILogLevel->value() );
    g_poPrefs->setLang( cmbAppLang->currentText() );
    if( m_inLangIdx != cmbAppLang->currentIndex() )
        QMessageBox::information( this, tr( "Information" ),
                                  tr( "Some of the changes you made will only be applied after the application is restarted." ) );

    g_poPrefs->setPanelsPerRow( spbPanels->value() );
    g_poPrefs->setBarcodeLength( spbBarcodeLen->value() );
    g_poPrefs->setBarcodePrefix( ledBarcodePrefix->text() );

    g_poPrefs->setServerAddress( ledServerHost->text() );
    g_poPrefs->setServerPort( ledServerPort->text() );
    g_poPrefs->setCommunicationPort( spbCOM->value() );

    g_poPrefs->setDeviceUseVAT( ledVatPercent->text().toInt() );

    g_poPrefs->setCassaAutoClose( chkAutoCloseCassa->isChecked() );

    g_poPrefs->setDefaultCountry( ledDefaultCountry->text() );

    g_poPrefs->save();

    QDialog::accept();
}
