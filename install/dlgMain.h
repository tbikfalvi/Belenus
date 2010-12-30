//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgMain.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DLGMAIN_H
#define DLGMAIN_H

//====================================================================================

#include <QDialog>

//====================================================================================

#include "ui_dlgMain.h"

//====================================================================================
class dlgMain : public QDialog, protected Ui::dlgMain
{
    Q_OBJECT

public:
    explicit dlgMain(QWidget *parent = 0);
    ~dlgMain();

private:
    void processPage( int p_nPage );
private slots:
    void on_pbCancel_clicked();
    void on_pbPrev_clicked();
    void on_pbNext_clicked();
    void on_pbStartInstall_clicked();
};
//====================================================================================
#endif // DLGMAIN_H
//====================================================================================
