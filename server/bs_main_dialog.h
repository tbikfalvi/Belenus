//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_main_dialog.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo dialogus ablak osztaly leirasa
//====================================================================================

#ifndef BS_MAIN_DIALOG_H
#define BS_MAIN_DIALOG_H

//====================================================================================
#include <QDialog>
#include <QLabel>

#include "bs_server.h"

//====================================================================================
class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:

private:
    QLabel              *statusLabel;
    QLabel              *captionLabel;
    cBelenusTcpServer   *belenusServer;
};
//====================================================================================

#endif
