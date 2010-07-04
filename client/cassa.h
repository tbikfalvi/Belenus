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

    void                    init();
    bool                    isCassaExists();
    void                    createNew( unsigned int p_uiUserId );
    bool                    isCassaClosed();
    unsigned int            cassaOwner();
    void                    cassaReOpen();
    void                    cassaClose();
    void                    setEnabled();
    void                    setDisabled();
    bool                    isCassaEnabled();
    unsigned int            cassaId();
    void                    cassaIncreaseMoney( int p_nMoney, QString p_qsComment = "" );
    void                    cassaDecreaseMoney( int p_nMoney, QString p_qsComment = "" );

private:
    cDBCassa                *m_pCassa;
    cDBDenomination         *m_pDenomination;
    cDBCassaDenomination    *m_pCassaDenomination;

    bool                     m_bCassaEnabled;

};

#endif // CASSA_H
