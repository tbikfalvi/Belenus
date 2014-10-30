#ifndef DLGPANELUSE_H
#define DLGPANELUSE_H

#include <QStringList>
#include <QVector>

#include "../belenus.h"
#include "db/dbpatientcard.h"
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

    void             setFocus();
    void             setAutoSelected();
    void             setOrderNum( unsigned int p_uiOrderNum );
    int              lengthSeconds();
    QStringList      usedUnitIds();

private:
    int              m_nUnitTime;
    unsigned int     m_uiOrderNum;
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
    cDlgPanelUse( QWidget *p_poParent = 0, unsigned int p_uiPanelId = 0 );
    virtual ~cDlgPanelUse();

    void                         enableCardUsage( bool p_bEnabled );
    void                         enableCashUsage( bool p_bEnabled );
    void                         setPanelUsePatientCard( QString p_qsPatientCardBarcode );
    void                         setPanelUsePatientCard( unsigned int p_uiPatientCardId );
    void                         setPanelUseTimeCash( unsigned int p_uiSeconds );
    void                         setPanelUseTime( unsigned int p_uiSeconds );
    void                         setPanelUseTime();
    void                         setPanelUsePrice();
    unsigned int                 panelUseSecondsCard();
    unsigned int                 panelUseSecondsCash();
    unsigned int                 panelUsePrice();
    unsigned int                 panelUsePatientCardId();
    QString                      panelUsePatientCardBarcode();
    int                          countPatientCardUnitsLeft();
    QStringList                  panelUnitIds();
    unsigned int                 panelTypeId();

private:

    QWidget                     *m_poParent;
    QWidget                     *m_poMsg;
    bool                         m_bInit;
    cDBPatientCard               m_obDBPatientCard;
    unsigned int                 m_uiPanelId;
    QVector<cPanelPCUnitUse*>    qvPanelUseUnits;
    unsigned int                 m_uiPanelUsePatientCardId;
    unsigned int                 m_uiPanelBaseTimeCard;
    unsigned int                 m_uiPanelBaseTimeCash;
    unsigned int                 m_uiPanelUseTimeCard;
    unsigned int                 m_uiPanelUseTimeCash;
    unsigned int                 m_uiPanelUsePrice;
    QStringList                  m_qslUnitIds;
    bool                         m_bIsEnterAccepted;
    bool                         m_bIsCardCanBeUsed;
    bool                         m_bIsCashCanBeUsed;
    QStringList                  m_qslPanelUseTimes;
    unsigned int                 m_uiIndexOfTime;
    unsigned int                 m_uiPanelTypeId;

    void                        _enablePanelUseTypes();

private slots:
    void                         slotPatientCardUseUpdated();
    void                         on_pbOk_clicked();
    void                         on_pbCancel_clicked();
    void                         on_cmbTimeIntervall_currentIndexChanged(int index);
    void                         on_ledPatientCardBarcode_returnPressed();
    void                         on_pbReloadPC_clicked();
    void on_ledPatientCardBarcode_textEdited(const QString &arg1);
};
//====================================================================================

#endif
