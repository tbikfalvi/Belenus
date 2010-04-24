#ifndef DBPRODUCTTYPE_H
#define DBPRODUCTTYPE_H

#include <QSqlRecord>

#include "../framework/sevexception.h"

class cDBProductType
{
public:
    cDBProductType();
    ~cDBProductType();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id()                                                                throw();
    string          name()                                                              throw();
    void            setName( const string &p_stName )                                   throw();

private:
    unsigned int    m_uiId;
    string          m_stName;

    void init( const unsigned int p_uiId = 0,
               const string &p_stName = "" )           throw();
    void init( const QSqlRecord &p_obRecord )          throw();

};

#endif // DBPRODUCTTYPE_H
