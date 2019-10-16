#ifndef DLGSELECTMAILRECIPIENT_H
#define DLGSELECTMAILRECIPIENT_H

#include <QStringList>
#include <QVector>

#include "../belenus.h"
#include "ui_dlgSelectMailRecipient.h"

//====================================================================================
class cDlgSelectMailRecipient : public QDialog, protected Ui::dlgSelectMailRecipient
{
    Q_OBJECT

public:
    QStringList qslRecipients;

    cDlgSelectMailRecipient( QWidget *p_poParent = 0 );
    virtual ~cDlgSelectMailRecipient();

private slots:
    void        on_pbOk_clicked();
    void        on_pbCancel_clicked();

    void        _refreshList();
    void on_listRecipients_clicked(const QModelIndex &index);
};
//====================================================================================

#endif
