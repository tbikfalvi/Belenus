#ifndef DLGPANELTYPEEDIT_H
#define DLGPANELTYPEEDIT_H

#include "ui_dlgpaneltypeedit.h"

class cDlgPanelTypeEdit : public QDialog, private Ui::dlgPanelTypeEdit
{
    Q_OBJECT

public:
    cDlgPanelTypeEdit( QWidget *p_poParent = 0 );
    virtual ~cDlgPanelTypeEdit();

    QString  panelTypeName( void ) const;
    void setPanelTypeName( const QString &p_qsName );
};

#endif // DLGPANELTYPEEDIT_H
