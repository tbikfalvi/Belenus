#ifndef DLGPANELUSE_H
#define DLGPANELUSE_H

#include <QStringList>
#include <QVector>

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
    QLabel          *lblValid;
    QLabel          *lblValidData;

    QSpacerItem     *horizontalSpacer3;
    cPanelPCUnitUse( QWidget *p_poParent = 0, QStringList *p_qslParameters = NULL );

    int              lengthSeconds();
    QStringList      usedUnitIds();

private:
    int              m_nUnitTime;
    QStringList      m_qslUnitIds;

signals:
    void             signalButtonClicked();
    void             signalComboIndexChanged();

private slots:
    void             slotButtonClicked();
    void             slotComboUnitUpdated();

};
//====================================================================================
class cDlgPanelUse : public QDialog, protected Ui::dlgPanelUse
{
    Q_OBJECT

public:
    cDlgPanelUse( QWidget *p_poParent = 0, QString p_qsPanelTitle = "" );
    virtual ~cDlgPanelUse();

    void                         setPanelUsePatientCard( unsigned int p_uiPatientCardId );
    void                         setPanelUseTime( unsigned int p_uiSeconds );
    void                         setPanelUseTime();
    void                         setPanelUsePrice();
    unsigned int                 panelUseSeconds();
    unsigned int                 panelUsePrice();
    QStringList                  panelUnitIds();

private:

    QVector<cPanelPCUnitUse*>    qvPanelUseUnits;
    unsigned int                 m_uiPanelUsePatientCardId;
    unsigned int                 m_uiPanelUseTime;
    unsigned int                 m_uiPanelUsePrice;
    QStringList                  m_qslUnitIds;

private slots:
    void                         slotPatientCardUseUpdated();
    void                         on_pbOk_clicked();
    void                         on_pbCancel_clicked();
};
//====================================================================================

#endif
