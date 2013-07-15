#ifndef DLGPANELUSE_H
#define DLGPANELUSE_H

#include "../belenus.h"
#include "ui_dlgpaneluse.h"

class cDlgPanelUse : public QDialog, protected Ui::dlgPanelUse
{
    Q_OBJECT

public:
    cDlgPanelUse( QWidget *p_poParent = 0, QString p_qsPanelTitle = "" );
    virtual ~cDlgPanelUse();

    void        setPanelUsePatientCard( unsigned int p_uiPatientCardId );
    void        setPanelUseTime( unsigned int p_uiSeconds );

private:

    unsigned int     m_uiPanelUsePatientCardId;
    unsigned int     m_uiPanelUseTime;

private slots:

};

#endif
