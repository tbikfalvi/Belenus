#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelsetting.h"
#include "belenus.h"

cDlgPanelSetting::cDlgPanelSetting( QWidget *p_poParent, cDBPanel *p_poPanels ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelSetting::cDlgPanelSetting" );

    setupUi( this );

    setWindowTitle( tr( "Panel settings" ) );
    setWindowIcon( QIcon("./resources/40x40_device.png") );



    buttonBox->button(QDialogButtonBox::Ok)->setIcon( QIcon("./resources/40x40_ok.png") );
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPanels = p_poPanels;

    if( m_poPanels )
    {
        ledTitle->setText( m_poPanels->title() );

        QSqlQuery *poQueryType;

        poQueryType = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, name FROM panelTypes WHERE active=1" ) );
        while( poQueryType->next() )
        {
            cmbPanelType->addItem( poQueryType->value( 1 ).toString(), poQueryType->value( 0 ) );
            if( poQueryType->value( 0 ).toUInt() == m_poPanels->panelTypeId() )
                cmbPanelType->setCurrentIndex( cmbPanelType->count()-1 );
        }
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPanel", QPoint(270,70) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgPanelSetting::~cDlgPanelSetting()
{
}

void cDlgPanelSetting::accept()
{
    bool  boCanBeSaved = true;

    if( ledTitle->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Title of Panel can not be empty." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanels->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanels->setTitle( ledTitle->text() );
        m_poPanels->setPanelTypeId( cmbPanelType->itemData( cmbPanelType->currentIndex() ).toUInt() );
        m_poPanels->setActive( true );
        m_poPanels->save();
        QDialog::accept();
    }
}


