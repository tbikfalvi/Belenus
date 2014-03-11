#ifndef DLGCASSAEDIT_H
#define DLGCASSAEDIT_H

#include <QSortFilterProxyModel>
#include <QPushButton>

#include "belenus.h"
#include "ui_dlgcassa.h"
#include "db/dbcassa.h"
#include "../../framework/qtmysqlquerymodel.h"

class cDlgCassaEdit : public QDialog, protected Ui::dlgCassaEdit
{
    Q_OBJECT

public:
    cDlgCassaEdit( QWidget *p_poParent = 0 );
    virtual ~cDlgCassaEdit();

public slots:

protected:
    QString                  m_qsQuery;
    cQTMySQLQueryModel      *m_poModel;
    QSortFilterProxyModel   *m_poSortedModel;
    unsigned int             m_uiSelectedId;
    int                      m_inSelectedRow;
    cDBCassa                *m_poCassa;

    void setupTableView();
    void enableButtons();
//    QString convertCurrency( int p_nCurrencyValue, QString p_qsCurrency );

protected slots:
    void refreshTable();
    void itemSelectionChanged( const QItemSelection &p_obSelected,
                               const QItemSelection &p_obDeSelected );

private slots:
    void on_pbCashGet_clicked();
    void on_pbCashAdd_clicked();
    void on_pbClose_clicked();
    void on_pbExpense_clicked();
};

#endif
