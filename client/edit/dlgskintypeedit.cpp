#include <QPushButton>
#include <QMessageBox>

#include "dlgskintypeedit.h"
#include "belenus.h"

cDlgSkinTypeEdit::cDlgSkinTypeEdit( QWidget *p_poParent, cDBSkinTypes *p_poSkinTypes ) : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgSkinTypeEdit::cDlgSkinTypeEdit" );

    setupUi( this );

    setWindowTitle( tr( "Skin type" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    buttonBox->button(QDialogButtonBox::Ok)->setIcon( QIcon("./resources/40x40_ok.png") );
    buttonBox->button(QDialogButtonBox::Cancel)->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poSkinTypes = p_poSkinTypes;

    if( m_poSkinTypes )
    {
        ledNameVal->setText( m_poSkinTypes->name() );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditSkinType", QPoint(270,70) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgSkinTypeEdit::~cDlgSkinTypeEdit()
{
}

void cDlgSkinTypeEdit::accept()
{
    bool  boCanBeSaved = true;

    if( ledNameVal->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of Skin Type can not be empty." ), QMessageBox::Ok );
    }

    if( boCanBeSaved )
    {
        m_poSkinTypes->setLicenceId( g_poPrefs->getLicenceId() );
        m_poSkinTypes->setName( ledNameVal->text() );
        m_poSkinTypes->setActive( true );
        m_poSkinTypes->save();
        QDialog::accept();
    }
}


