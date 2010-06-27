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
public:
    cCassa();
    ~cCassa();

    void                    init( QWidget *parent );
    bool                    isCassaExists();
    void                    createNew( unsigned int p_uiUserId );
    bool                    isCassaClosed();
    unsigned int            cassaOwner();
    void                    cassaReOpen();

private:
    cDBCassa                *m_pCassa;
    cDBDenomination         *m_pDenomination;
    cDBCassaDenomination    *m_pCassaDenomination;

};

#endif // CASSA_H
