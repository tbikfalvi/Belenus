//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgglobals.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DLGGLOBALS_H
#define DLGGLOBALS_H

//====================================================================================

#include "../belenus.h"
#include "ui_dlgdbglobals.h"

//====================================================================================

class cDlgDBGlobals : public QDialog, protected Ui::dlgDBGlobals
{
    Q_OBJECT

public:
    cDlgDBGlobals( QWidget *p_poParent = 0 );
    virtual ~cDlgDBGlobals();

    void    autoSynchronization();

private:
    int     m_nTimer;
    bool    m_bAutoSynchronization;
    bool    m_bNormalStart;
    bool    m_bAutoStart;
    bool    m_bAutoClose;

private slots:
    void    on_pbStart_clicked();

protected:
    void    timerEvent(QTimerEvent *event);

};

//====================================================================================
#endif // DLGGLOBALS_H
//====================================================================================
