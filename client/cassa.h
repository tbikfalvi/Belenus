#ifndef CASSA_H
#define CASSA_H

#include <QWidget>
#include <QString>

#include "../framework/sevexception.h"
#include "db/dbcassa.h"
#include "db/dbdenomination.h"
#include "db/dbcassadenomination.h"
#include "db/dbshoppingcart.h"
#include "db/dbpatientcard.h"

class cCassa
{
    //Q_OBJECT

public:
    cCassa();
    ~cCassa();

    bool                    loadOpenCassa( unsigned int p_uiUserId );
    bool                    loadOpenCassa();
    bool                    loadLatestCassa( unsigned int p_uiUserId );
    bool                    loadLatestCassaWithCash();

    void                    createNew( unsigned int p_uiUserId, int p_inBalance=0 );
    void                    cassaContinue();
    void                    cassaContinue( unsigned int p_uiUserId );
    void                    cassaReOpen();
    void                    cassaReOpen( unsigned int p_uiCassaId );
    void                    cassaClose();

    unsigned int            cassaProcessPatientCardSell( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, bool p_bNewCard, int p_inPayType );
    unsigned int            cassaProcessPatientCardRefill( const cDBPatientCard &p_DBPatientCard, const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType );
    unsigned int            cassaProcessProductStorageChange( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, bool p_bGlobalCassa = false );
    unsigned int            cassaProcessDeviceUse( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType, QString p_qsPanelTitle );
    void                    cassaProcessProductSell( const cDBShoppingCart &p_obDBShoppingCart, QString p_qsComment, int p_inPayType );
    void                    cassaConnectLedgerWithLedgerDevice( unsigned int p_uiLedgerId, unsigned int p_uiLedgerDeviceId );
    void                    cassaProcessRevokeDeviceUse( unsigned int p_uiLedgerId );
    void                    cassaProcessRevokeCassaAction( unsigned int p_uiCassaHistoryId );
    void                    cassaProcessCashExpense( int p_nMoney, QString p_qsComment );

    void                    cassaIncreaseMoney( int p_nMoney, QString p_qsComment = "" );
    void                    cassaDecreaseMoney( int p_nMoney, QString p_qsComment = "" );
    void                    cassaDecreaseMoney( unsigned int p_uiUserId, int p_nMoney, QString p_qsComment = "" );
    void                    cassaAddMoneyAction( int p_nCash, int p_nCard, unsigned int p_uiLedgerId, QString p_qsComment = "", unsigned int p_uiParentId = 0, bool p_bGlobalCassa = false );
//    void                    cassaAddGlobalMoneyAction( int p_nMoney, unsigned int p_uiLedgerId, QString p_qsComment = "", unsigned int p_uiParentId = 0 );

    void                    setEnabled();
    void                    setDisabled();
    bool                    isCassaEnabled();
    bool                    isCassaClosed();

    QString                 cassaOwnerStr();
    unsigned int            cassaOwnerId();
    int                     cassaBalance();
    int                     cassaGlobalBalance();
    unsigned int            cassaId();
    QString                 cassaOpenDate();
    QString                 cassaCloseDate();

private:
    cDBCassa                *m_pCassa;
    cDBDenomination         *m_pDenomination;
    cDBCassaDenomination    *m_pCassaDenomination;

    bool                     m_bCassaEnabled;

};

#endif // CASSA_H
