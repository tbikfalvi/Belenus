#ifndef DLGPATIENTCARD_H
#define DLGPATIENTCARD_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>
#include <QCheckBox>

#include "../framework/dlgcrud.h"

class cDlgPatientCard : public cDlgCrud
{
    Q_OBJECT

public:
    cDlgPatientCard( QWidget *p_poParent = 0 );
    virtual ~cDlgPatientCard();

    QHBoxLayout     *horizontalLayout;
    QLabel          *lblBarcode;
    QLineEdit       *ledBarcode;
    QLabel          *lblOwner;
    QLineEdit       *ledOwner;
    QSpacerItem     *horizontalSpacer1;
    QPushButton     *pbPatientCardReplace;
    QPushButton     *pbPartnerCardAssign;
    QPushButton     *pbAddUnits;
    QPushButton     *pbRemoveUnits;
    QCheckBox       *chkRFID;
    QCheckBox       *chkAssigned;
    QCheckBox       *chkHasComment;
    QLabel          *lblMessage;

protected:
    void timerEvent( QTimerEvent *p_poEvent );
    virtual void     setupTableView();
    virtual void     enableButtons();

signals:
    void             signalReplacePatientCard( const QString &p_qsBarcode );
    void             signalAssignPartnerCard( const QString &p_qsBarcode );

private:
    QWidget         *m_poParent;
    QString          m_qsCondition;
    int                          m_nTimer;

    bool             _isPatientCardNotForService();

protected slots:
    virtual void     refreshTable( QString p_qsCondition = "" );
    virtual void     newClicked( bool );
    virtual void     deleteClicked( bool );
    virtual void     editClicked( bool );
    virtual void     _slotAddUnits();
    virtual void     _slotRemoveUnits();
    virtual void     _slotPatientCardReplace();
    virtual void     _slotPartnerCardAssign();
};

#endif // DLGPATIENTCARD_H
