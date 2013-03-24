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
    void slot_refreshPrice();
    void on_pbActionTypes_clicked();
    void on_cmbProductAction_currentIndexChanged(int index);
    void on_pbCancel_clicked();
    void on_pbSave_clicked();
    void on_ledPrice_textEdited(const QString &arg1);
    void on_ledVatPercent_textEdited(const QString &arg1);
};

#endif // DLGPRODUCTSTORAGE_H
