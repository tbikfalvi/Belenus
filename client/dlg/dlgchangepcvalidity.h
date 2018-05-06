#ifndef DLGCHANGEPCVALIDITY_H
#define DLGCHANGEPCVALIDITY_H

#include <QDate>
#include <QCheckBox>

#include "../belenus.h"
#include "ui_dlgchangepcvalidity.h"

//====================================================================================
class cPCUnit : public QCheckBox
{
    Q_OBJECT

public:
    cPCUnit( QWidget *p_poParent = 0, QString p_qsText = "" );
    virtual ~cPCUnit() {}

    QString     unitCondition()     { return m_qsCondition; }

private:
    QString     m_qsCondition;

};

//====================================================================================
class cDlgChangePCValidity : public QDialog, protected Ui::dlgChangePCValidity
{
    Q_OBJECT

public:
    enum selValidity
    {
        SV_NONE = 0,
        SV_ONEDAY,
        SV_ONEWEEK,
        SV_TWOWEEKS,
        SV_ONEMONTH,
        SV_THREEMONTHS,
        SV_HALFYEAR,
        SV_CUSTOM
    };

    cDlgChangePCValidity( QWidget *p_poParent = 0, unsigned int p_uiCardId = 0 );
    virtual ~cDlgChangePCValidity() {}
    QString         unitCondition() { return m_qsUnitCondition; }
    selValidity     selectionValidity( QDate *p_qdDate );
    bool            isUnitsNotSelected();

private:

    QVector<cPCUnit*>   qvPCUnits;
    selValidity         m_selValidity;
    unsigned int        m_uiCardId;
    cDBPatientCard      m_obDBPatientCard;
    QString             m_qsUnitCondition;

private slots:
    void on_pbOk_clicked();
    void on_pbCancel_clicked()  { QDialog::reject(); }
    void selectionChanged();
};

#endif
