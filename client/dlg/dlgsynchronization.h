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

private slots:

};

//====================================================================================
#endif // DLGSYNCHRONIZATION_H
//====================================================================================
