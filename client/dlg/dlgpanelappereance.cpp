#include <QPushButton>
#include <QMessageBox>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>

#include "dlgpanelappereance.h"

cDlgPanelAppereance::cDlgPanelAppereance( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Panel settings") );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    pDBPanelStatusSettings = new cDBPanelStatusSettings();

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, name FROM panelStatuses WHERE active=1 AND panelStatusId>0" ) );
    while( poQuery->next() )
    {
        cmbPanelStatus->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }

    cmbPanelText->addItem( tr("Status name"), 0 );
    cmbPanelText->addItem( tr("Length of current status"), 1 );
    cmbPanelText->addItem( tr("Length of next status"), 2 );
    cmbPanelText->addItem( tr("Information"), 3 );

}

cDlgPanelAppereance::~cDlgPanelAppereance()
{
    if( pDBPanelStatusSettings ) delete pDBPanelStatusSettings;
}


void cDlgPanelAppereance::on_pbBackgroundColor_clicked()
{
    QColor obNewColor = QColorDialog::getColor( QColor( QString("#00ff00") ), this );
//    if( obNewColor.isValid() ) g_poPrefs->setMainBackground( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( obNewColor.name() ) );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );
}

void cDlgPanelAppereance::on_pbFontColor_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok)
    {
        pDBPanelStatusSettings->setStatusFontName( font.family() );
        //pDBPanelStatusSettings->setStatusFontColor(  );
        pDBPanelStatusSettings->setStatusFontSize( font.pixelSize() );
    }
}

void cDlgPanelAppereance::on_cmbPanelStatus_currentIndexChanged(int)
{
    try
    {
        pDBPanelStatusSettings->loadStatus( cmbPanelStatus->itemData(0).toUInt() );
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
            switch( cmbPanelStatus->itemData( cmbPanelStatus->currentIndex()).toInt() )
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
            pDBPanelStatusSettings->setTimerFontName( "Arial" );
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

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( pDBPanelStatusSettings->backgroundColor() ) );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );

    QFont   obFont;

    obFont = lblSample->font();
    switch( cmbPanelText->currentIndex() )
    {
        case 0:
            obFont.setFamily( pDBPanelStatusSettings->statusFontName() );
            obFont.setPixelSize( pDBPanelStatusSettings->statusFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( tr("status") );
            break;
        case 1:
            obFont.setFamily( pDBPanelStatusSettings->statusFontName() );
            obFont.setPixelSize( pDBPanelStatusSettings->statusFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( tr("current length") );
            break;
        case 2:
            obFont.setFamily( pDBPanelStatusSettings->statusFontName() );
            obFont.setPixelSize( pDBPanelStatusSettings->statusFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( tr("next length") );
            break;
        case 3:
            obFont.setFamily( pDBPanelStatusSettings->statusFontName() );
            obFont.setPixelSize( pDBPanelStatusSettings->statusFontSize() );
            obFont.setBold( true );
            obFont.setCapitalization( QFont::AllUppercase );
            lblSample->setFont( obFont );
            lblSample->setText( tr("information") );
            break;
    }
}

void cDlgPanelAppereance::on_cmbPanelText_currentIndexChanged(int)
{

}

void cDlgPanelAppereance::on_pbTextColor_clicked()
{

}
