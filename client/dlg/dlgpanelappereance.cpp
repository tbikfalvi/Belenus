#include <QPushButton>
#include <QMessageBox>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QFontDatabase>

//====================================================================================

#include "dlgpanelappereance.h"

//====================================================================================
cDlgPanelAppereance::cDlgPanelAppereance( QWidget *p_poParent ) : QDialog( p_poParent )
//====================================================================================
{
    m_bInit = true;

    setupUi( this );

    setWindowTitle( tr("Panel settings") );
    setWindowIcon( QIcon("./resources/40x40_device.png") );
    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbFont->setIcon( QIcon("./resources/40x40_font.png") );

    pbFont->setEnabled( false );
    pbFont->setVisible( false );

    QSqlQuery               *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, name FROM panelStatuses WHERE active=1 AND panelStatusId>0" ) );
    while( poQuery->next() )
    {
        cmbPanelStatus->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );

        cDBPanelStatusSettings  *pDBPanelStatusSettings = new cDBPanelStatusSettings();

        try
        {
            pDBPanelStatusSettings->loadStatus( poQuery->value( 0 ).toUInt() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("Panelstatus settings id not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
            }
            else
            {
                pDBPanelStatusSettings->createNew();
                pDBPanelStatusSettings->setLicenceId( g_poPrefs->getLicenceId() );
                pDBPanelStatusSettings->setPanelStatusId( poQuery->value( 0 ).toUInt() );
                switch( poQuery->value( 0 ).toUInt() )
                {
                    case 1:
                        pDBPanelStatusSettings->setBackgroundColor( "#00ff00" );
                        break;
                    case 2:
                        pDBPanelStatusSettings->setBackgroundColor( "#ffff00" );
                        break;
                    case 3:
                        pDBPanelStatusSettings->setBackgroundColor( "#ff0000" );
                        break;
                    case 4:
                        pDBPanelStatusSettings->setBackgroundColor( "#ffff00" );
                        break;
                }
                pDBPanelStatusSettings->setStatusFontName( "Arial" );
                pDBPanelStatusSettings->setStatusFontSize( 18 );
                pDBPanelStatusSettings->setStatusFontColor( "#000000" );
                pDBPanelStatusSettings->setTimerFontName( "Book Antiqua" );
                pDBPanelStatusSettings->setTimerFontSize( 30 );
                pDBPanelStatusSettings->setTimerFontColor( "#000000" );
                pDBPanelStatusSettings->setNextFontName( "Arial" );
                pDBPanelStatusSettings->setNextFontSize( 18 );
                pDBPanelStatusSettings->setNextFontColor( "#000000" );
                pDBPanelStatusSettings->setInfoFontName( "Arial" );
                pDBPanelStatusSettings->setInfoFontSize( 10 );
                pDBPanelStatusSettings->setInfoFontColor( "#000000" );
                pDBPanelStatusSettings->setActive( true );
            }
        }
        m_obStatusSettings.push_back( pDBPanelStatusSettings );
    }

    cmbPanelText->addItem( tr("Status name"), 0 );
    cmbPanelText->addItem( tr("Length of current status"), 1 );
    cmbPanelText->addItem( tr("Length of next status"), 2 );
    cmbPanelText->addItem( tr("Information"), 3 );

    QFontDatabase   fontDatabase;

    foreach (QString family, fontDatabase.families())
    {
        cmbFontNames->addItem( family );
    }

    m_bInit = false;

    frmPanelTitle->setStyleSheet( QString("QFrame { background-color: lightgray }") );

    updatePanelSettings();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPanelAppereance", QPoint(455,400) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//====================================================================================
cDlgPanelAppereance::~cDlgPanelAppereance()
//====================================================================================
{
    g_poPrefs->setDialogSize( "EditPanelAppereance", QPoint( width(), height() ) );
}
//====================================================================================
void cDlgPanelAppereance::on_cmbPanelStatus_currentIndexChanged(int)
//====================================================================================
{
    if( m_bInit ) return;

    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::on_pbBackgroundColor_clicked()
//====================================================================================
{
    QColor obNewColor = QColorDialog::getColor( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->backgroundColor() ), this );
    if( obNewColor.isValid() )
        m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setBackgroundColor( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    QColor  colorFill = QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->backgroundColor() );
    obColorIcon.fill( colorFill );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );
    frmPanelBody->setStyleSheet( QString("QFrame { background-color: %1 }").arg( colorFill.name() ) );
}
//====================================================================================
void cDlgPanelAppereance::on_cmbPanelText_currentIndexChanged(int)
//====================================================================================
{
    if( m_bInit ) return;

    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::on_cmbFontNames_currentIndexChanged(int index)
//====================================================================================
{
    switch( cmbPanelText->currentIndex() )
    {
        case 0:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setStatusFontName( cmbFontNames->currentText() );
            break;
        case 1:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setTimerFontName( cmbFontNames->currentText() );
            break;
        case 2:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setNextFontName( cmbFontNames->currentText() );
            break;
        case 3:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setInfoFontName( cmbFontNames->currentText() );
            break;
    }
    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::on_spinFontSize_valueChanged(int )
//====================================================================================
{
    switch( cmbPanelText->currentIndex() )
    {
        case 0:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setStatusFontSize( spinFontSize->value() );
            break;
        case 1:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setTimerFontSize( spinFontSize->value() );
            break;
        case 2:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setNextFontSize( spinFontSize->value() );
            break;
        case 3:
            m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setInfoFontSize( spinFontSize->value() );
            break;
    }
    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::on_pbFont_clicked()
//====================================================================================
{
    QFontDialog obFontDialog( this );

    QFont obFont = lblFont->font();
    switch( cmbPanelText->currentIndex() )
    {
        case 0:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontSize() );
            break;
        case 1:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontSize() );
            break;
        case 2:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontSize() );
            break;
        case 3:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontSize() );
            break;
    }
    obFontDialog.setCurrentFont( obFont );
    if( obFontDialog.exec() == QDialog::Accepted )
    {
        obFont = obFontDialog.currentFont();
        switch( cmbPanelText->currentIndex() )
        {
            case 0:
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setStatusFontName( obFont.family() );
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setStatusFontSize( obFont.pixelSize() );
                break;
            case 1:
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setTimerFontName( obFont.family() );
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setTimerFontSize( obFont.pixelSize() );
                break;
            case 2:
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setNextFontName( obFont.family() );
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setNextFontSize( obFont.pixelSize() );
                break;
            case 3:
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setInfoFontName( obFont.family() );
                m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setInfoFontSize( obFont.pixelSize() );
                break;
        }
    }
    lblSample->setFont( obFont );

    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::on_pbTextColor_clicked()
//====================================================================================
{
    QColor obNewColor = QColorDialog::getColor( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontColor() ), this );
    if( obNewColor.isValid() )
        m_obStatusSettings.at(cmbPanelStatus->currentIndex())->setStatusFontColor( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontColor() ) );
    pbTextColor->setIcon( QIcon( obColorIcon ) );

    updatePanelSettings();
}
//====================================================================================
void cDlgPanelAppereance::updatePanelSettings()
//====================================================================================
{
    QPixmap  obColorIcon( 24, 24 );

    QColor  colorFill = QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->backgroundColor() );
    obColorIcon.fill( colorFill );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );
    frmPanelBody->setStyleSheet( QString("QFrame { background-color: %1 }").arg( colorFill.name() ) );

    QFont   obFont;

    obFont = lblSample->font();
    switch( cmbPanelText->currentIndex() )
    {
        case 0:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontColor()).name()).arg(tr("status")) );
            obColorIcon.fill( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->statusFontColor() ) );
            break;
        case 1:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontColor()).name()).arg(tr("current length")) );
            obColorIcon.fill( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->timerFontColor() ) );
            break;
        case 2:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontColor()).name()).arg(tr("next length")) );
            obColorIcon.fill( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->nextFontColor() ) );
            break;
        case 3:
            obFont.setFamily( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontName() );
            obFont.setPixelSize( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontColor()).name()).arg(tr("information")) );
            obColorIcon.fill( QColor( m_obStatusSettings.at(cmbPanelStatus->currentIndex())->infoFontColor() ) );
            break;
    }
    pbTextColor->setIcon( QIcon( obColorIcon ) );
    cmbFontNames->setCurrentIndex( cmbFontNames->findText( obFont.family() ) );
    spinFontSize->setValue( obFont.pixelSize() );
}
//====================================================================================
void cDlgPanelAppereance::accept ()
//====================================================================================
{
    for( unsigned int i = 0; i < m_obStatusSettings.size(); i ++ )
    {
        m_obStatusSettings.at( i )->save();
    }

    for( unsigned int i = 0; i < m_obStatusSettings.size(); i ++ ) if( m_obStatusSettings.at( i ) ) delete m_obStatusSettings.at( i );

    QDialog::accept();
}
//====================================================================================
