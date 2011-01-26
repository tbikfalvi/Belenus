#ifndef DLGLOGS_H
#define DLGLOGS_H

#include <QSortFilterProxyModel>

#include "ui_dlglogs.h"
#include "../../framework/qtmysqlquerymodel.h"

class cLogsSqlModel : public cQTMySQLQueryModel
{
    Q_OBJECT

public:
    cLogsSqlModel( QObject *p_poParent = 0 );
    virtual ~cLogsSqlModel();

    virtual QVariant data( const QModelIndex &item, int role ) const;
};

class cDlgLogs : public QDialog, private Ui::dlgLogs
{
    Q_OBJECT

public:
    cDlgLogs( QWidget *p_poParent = 0 );
    ~cDlgLogs();

private:
    cLogsSqlModel          *m_poModel;
    QSortFilterProxyModel  *m_poSortedModel;

    void refreshTable( void );

private slots:
    void on_sliFilter_valueChanged( int p_inValue );
    void dateChanged( const QDate & );
};

#endif
