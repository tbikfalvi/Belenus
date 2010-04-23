#include "dbconnection.h"

cDBConnection::cDBConnection()
{
    m_boOpen = false;
}

cDBConnection::~cDBConnection()
{
    if( isOpen() ) close();
}

void cDBConnection::open() throw( cSevException )
{
    m_boOpen = true;
}

void cDBConnection::close() throw( cSevException )
{
    m_boOpen = false;
}

bool cDBConnection::isOpen() throw()
{
    return m_boOpen;
}
