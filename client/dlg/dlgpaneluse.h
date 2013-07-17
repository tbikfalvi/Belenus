#ifndef DLGPANELUSE_H
#define DLGPANELUSE_H

#include <QStringList>

#include "../belenus.h"
#include "ui_dlgpaneluse.h"

//====================================================================================
class cPanelPCUnitUse : public QFrame
{
    Q_OBJECT

public:
    QHBoxLayout     *horizontalLayout;
    QPushButton     *pbUseUnitType;
    QSpacerItem     *horizontalSpacer1;
    QComboBox       *cmbUseUnitCount;
    QSpacerItem     *horizontalSpacer2;

    cPanelPCUnitUse( QWidget *p_poParent = 0, QStringList *p_qslParameters = NULL );

};
//====================================================================================
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

    void            _fillPatientCardUnits();

private slots:

};
//====================================================================================

#endif
