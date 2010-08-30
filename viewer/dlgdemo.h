#ifndef DLGDEMO_H
#define DLGDEMO_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include "ui_dlgdemo.h"

#include "../framework/qtmysqlquerymodel.h"

class cDlgDemo : public QDialog, private Ui::dlgDemo
{
    Q_OBJECT

public:
    cDlgDemo( QWidget *parent = 0 );
    ~cDlgDemo();

private:
    cQTMySQLQueryModel     *m_poModel;
    QSortFilterProxyModel  *m_poSortedModel;

protected slots:
    virtual void refreshTable();
};

#endif // DLGDEMO_H
