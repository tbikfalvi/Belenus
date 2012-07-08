#ifndef CDLGSTORNO_H
#define CDLGSTORNO_H

#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>

#include "belenus.h"
#include "../framework/dlgcrud.h"

class cDlgStorno : public cDlgCrud
{
    Q_OBJECT

public:
    QHBoxLayout *horizontalLayoutTop;
    QLabel      *lblComment;
    QLineEdit   *ledComment;
    QSpacerItem *horizontalSpacerTop;
    QPushButton *pbStorno;
    QPushButton *pbExit;

    cDlgStorno( QWidget *p_poParent = 0 );
    ~cDlgStorno();

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbStorno_clicked();
};

#endif // CDLGSTORNO_H
