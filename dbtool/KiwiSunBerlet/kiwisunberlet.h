#ifndef KIWISUNBERLET_H
#define KIWISUNBERLET_H

#include <QDialog>
#include "ui_kiwisunberlet.h"

class KiwiSunBerlet : public QDialog, protected Ui_KiwiSunBerlet
{
    Q_OBJECT

public:
    explicit KiwiSunBerlet(QWidget *parent = 0);
    ~KiwiSunBerlet();

private:
    Ui::KiwiSunBerlet *ui;

private slots:
    void on_pbImportPCU_clicked();
    void on_pbSelectPCU_clicked();
};

#endif // KIWISUNBERLET_H
