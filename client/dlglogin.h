#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include "ui_dlglogin.h"
#include "../framework/qtmysqlquerymodel.h"

class cDlgLogIn : public QDialog, private Ui::dlgLogIn
{
    Q_OBJECT

public:
    cDlgLogIn( QWidget *p_poParent = 0 );
    virtual ~cDlgLogIn();

public slots:
    virtual void accept();
    virtual void reject();

private:
    cQTMySQLQueryModel *m_poModel;
};

#endif
