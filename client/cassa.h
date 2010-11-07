#ifndef CASSA_H
#define CASSA_H

#include <QWidget>
#include <QString>

#include "../framework/sevexception.h"
#include "db/dbcassa.h"
#include "db/dbdenomination.h"
#include "db/dbcassadenomination.h"

class cCassa
{
    //Q_OBJECT

public:
    cCassa();
    ~cCassa();

    bool                    loadOpenCassa( unsigned int p_uiUserId );
    bool                    loadOpenCassa();
    bool                    loadLatestCassa();

    void                    createNew( unsigned int p_uiUserId, int p_inBalance=0 );
    void                    cassaContinue();
    void                    cassaContinue( unsigned int p_uiUserId );
    void                    cassaReOpen();
    void                    cassaReOpen( unsigned int p_uiCassaId );
    void                    cassaClose();

    void                    cassaIncreaseMoney( int p_nMoney, QString p_qsComment = "" );
    void                    cassaDecreaseMoney( int p_nMoney, QString p_qsComment = "" );
    void                    cassaAddMoneyAction( int p_nMoney, QString p_qsComment = "" );

    void                    setEnabled();
    void                    setDisabled();
    bool                    isCassaEnabled();

    QString                 cassaOwnerStr();
    int                     cassaBalance();
    unsigned int            cassaId();

private:
    cDBCassa                *m_pCassa;
    cDBDenomination         *m_pDenomination;
    cDBCassaDenomination    *m_pCassaDenomination;

    bool                     m_bCassaEnabled;

};

#endif // CASSA_H
