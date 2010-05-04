#include "dlgpaneltypeedit.h"
#include "belenus.h"

cDlgPanelTypeEdit::cDlgPanelTypeEdit( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPanelTypeEdit::cDlgPanelTypeEdit" );

    setupUi( this );
}

cDlgPanelTypeEdit::~cDlgPanelTypeEdit()
{
}

QString  cDlgPanelTypeEdit::panelTypeName( void ) const
{
    return ledNameVal->text();
}

void cDlgPanelTypeEdit::setPanelTypeName( const QString &p_qsName )
{
    ledNameVal->setText( p_qsName );
}
