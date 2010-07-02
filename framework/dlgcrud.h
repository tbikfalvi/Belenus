#ifndef DLGCRUD_H
#define DLGCRUD_H

#include <QSortFilterProxyModel>
#include <QPushButton>

#include "ui_dlgcrud.h"
#include "qtmysqlquerymodel.h"

class cDlgCrud : public QDialog, protected Ui::dlgCrud
{
    Q_OBJECT

public:
    cDlgCrud( QWidget *p_poParent = 0 );
    virtual ~cDlgCrud();

protected:
    QString                 m_qsQuery;
    cQTMySQLQueryModel     *m_poModel;
    QSortFilterProxyModel  *m_poSortedModel;
    QPushButton            *m_poBtnNew;
    QPushButton            *m_poBtnDelete;
    QPushButton            *m_poBtnEdit;
    QPushButton            *m_poBtnClose;
    QPushButton            *m_poBtnSave;
    unsigned int            m_uiSelectedId;
    int                     m_inSelectedRow;

    virtual void setupTableView();
    virtual void enableButtons() {};

protected slots:
    void itemSelectionChanged( const QItemSelection &p_obSelected,
                               const QItemSelection &p_obDeSelected );
    virtual void refreshTable();
    virtual void newClicked( bool ) = 0;
    virtual void deleteClicked( bool ) = 0;
    virtual void editClicked( bool ) = 0;
//    virtual void closeClicked( bool ) = 0;
};

#endif
