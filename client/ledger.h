#ifndef LEDGER_H
#define LEDGER_H

#include "../framework/sevexception.h"
#include "db/dbledger.h"
#include "db/dbledgerdevice.h"

class cLedger
{
public:
    cLedger();
    ~cLedger();

    void                createNewLedger();
    void                sellingDeviceUsage( unsigned int p_uiPanelId );
    void                sellingPatientCard( unsigned int p_uiPatientCardId );
    void                refillPatientCard( unsigned int p_uiPatientCardId );
    void                sellingProduct( unsigned int p_uiProductId );
    void                prepareLedger( int p_inNetPrice, int p_inVatpercent, QString p_qsComment );
    void                prepareLedger( QString p_qsComment );
    void                saveLedger();

    void                createNewLedgerDevice( unsigned int p_uiPanelId );
    void                prepareLedgerDevice( unsigned int p_uiPatientCardId,
                                             int p_inUnits,
                                             int p_inCash,
                                             int p_inTimeLength );
    void                addLedgerDevice( int p_inUnits,
                                         int p_inCash,
                                         int p_inTimeLength );
    void                finishLedgerDevice( int p_inTimeLength,
                                            int p_inTimeLeft = 0 );
    void                setLedgerDeviceComment( QString p_qsComment );
    void                saveLedgerDevice();

private:
    cDBLedger           *m_pLedger;
    cDBLedgerDevice     *m_pLedgerDevice;
    QString             m_qsActionComment;

};

#endif // LEDGER_H
