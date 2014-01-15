#include <QPushButton>
#include <QMessageBox>

#include "dlgpanelgroupedit.h"
#include "belenus.h"

cDlgPanelGroupEdit::cDlgPanelGroupEdit( QWidget *p_poParent, cDBPanelGroups *p_poPanelGroups ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelGroupEdit::cDlgPanelGroupEdit" );

    setupUi( this );

    setWindowTitle( tr( "Panel type" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    m_poPanelGroups = p_poPanelGroups;

    if( m_poPanelGroups )
    {
        ledNameVal->setText( m_poPanelGroups->name() );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPanelGroup", QPoint(270,70) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgPanelGroupEdit::~cDlgPanelGroupEdit()
{
}

void cDlgPanelGroupEdit::accept()
{
    bool  boCanBeSaved = true;

    if( ledNameVal->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of Panel Type can not be empty." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanelGroups->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanelGroups->setName( ledNameVal->text() );
        m_poPanelGroups->setActive( true );
        m_poPanelGroups->save();
        QDialog::accept();
    }
}


