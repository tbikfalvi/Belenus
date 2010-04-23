#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <string>
#include "sevexception.h"

using namespace std;

class cDBConnection
{
public:
    cDBConnection();
    virtual ~cDBConnection();

    virtual void open()                                      throw( cSevException );
    virtual void close()                                     throw( cSevException );
    virtual bool isOpen()                                    throw();
    virtual void executeQuery( const string &p_stQuery,
                               const bool p_boLog = false )  throw( cSevException ) = 0;

protected:
    bool         m_boOpen;
};

#endif

