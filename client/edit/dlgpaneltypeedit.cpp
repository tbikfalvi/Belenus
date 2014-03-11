#include <QPushButton>
#include <QMessageBox>

#include "dlgpaneltypeedit.h"
#include "belenus.h"

cDlgPanelTypeEdit::cDlgPanelTypeEdit( QWidget *p_poParent, cDBPanelTypes *p_poPanelTypes ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelTypeEdit::cDlgPanelTypeEdit" );

    setupUi( this );

    setWindowTitle( tr( "Panel type" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    buttonBox->button(QDialogButtonBox::Ok)->setIcon( QIcon("./resources/40x40_ok.png") );
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPanelTypes = p_poPanelTypes;

    if( m_poPanelTypes )
    {
        ledNameVal->setText( m_poPanelTypes->name() );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPanelType", QPoint(270,70) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgPanelTypeEdit::~cDlgPanelTypeEdit()
{
}

void cDlgPanelTypeEdit::accept()
{
    bool  boCanBeSaved = true;

    if( ledNameVal->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of Panel Type can not be empty." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poPanelTypes->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPanelTypes->setName( ledNameVal->text() );
        m_poPanelTypes->setActive( true );
        m_poPanelTypes->save();
        QDialog::accept();
    }
}


