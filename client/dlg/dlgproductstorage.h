#ifndef DLGPRODUCTSTORAGE_H
#define DLGPRODUCTSTORAGE_H

#include <QDialog>
#include <QStringList>

#include "belenus.h"
#include "ui_dlgproductstorage.h"
#include "../db/dbproduct.h"

class dlgProductStorage : public QDialog, protected Ui::dlgProductStorage
{
    Q_OBJECT

public:
    explicit dlgProductStorage( QWidget *parent = 0, cDBProduct *p_poProduct = NULL );
    ~dlgProductStorage();

private:
    bool             m_bInit;
    QStringList      m_qslActionTooltip;
    cDBProduct      *m_poProduct;

    void            _fillProductActionList();

private slots:
    void on_pbActionTypes_clicked();
    void on_cmbProductAction_currentIndexChanged(int index);
    void on_pbRefreshSP_clicked();
    void on_pbRefreshNP_clicked();
    void on_ledSumPrice_textEdited(QString );
    void on_ledNetPrice_textEdited(QString );
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
};

#endif // DLGPRODUCTSTORAGE_H
