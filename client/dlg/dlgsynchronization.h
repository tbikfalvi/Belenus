//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgsynchronization.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#ifndef DLGSYNCHRONIZATION_H
#define DLGSYNCHRONIZATION_H

//====================================================================================

#include "../belenus.h"
#include "../ui_dlgsynchronization.h"

//====================================================================================

class cDlgSynchronization : public QDialog, protected Ui::dlgSynchronization
{
    Q_OBJECT

public:
    cDlgSynchronization( QWidget *p_poParent = 0 );
    virtual ~cDlgSynchronization();
    void    autoSynchronization();

private:
    int     m_nTimer;
    bool    m_bAutoSynchronization;
    bool    m_bAutoStart;
    bool    m_bAutoClose;

private slots:
    void    checkSynchronizationStatus();
    void    on_pbStart_clicked();

protected:
    void    timerEvent(QTimerEvent *event);

};

//====================================================================================
#endif // DLGSYNCHRONIZATION_H
//====================================================================================
