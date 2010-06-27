#ifndef DB_CASSADENOMINATION_H
#define DB_CASSADENOMINATION_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBCassaDenomination
{
public:
    cDBCassaDenomination();
    ~cDBCassaDenomination();

    void            load( const unsigned int p_uiDenominationId,
                          const unsigned int p_uiCassaId,
                          const unsigned int p_uiLicenceId )                    throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            createNew()                                                 throw();
    unsigned int    denominationId() const                                      throw();
    void            setDenominationId( const unsigned int p_nDenominationId )   throw();
    unsigned int    cassaId() const                                             throw();
    void            setCassaId( const unsigned int p_nLicenceId )               throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    int             value() const                                               throw();
    void            setValue( const int p_inValue )                             throw();
    string          archive() const                                             throw();
    void            setArchive( const string &p_stArchive )                     throw();

private:
    unsigned int    m_uiDenominationId;
    unsigned int    m_uiCassaId;
    unsigned int    m_uiLicenceId;
    int             m_inValue;
    string          m_stArchive;
    bool            m_bNewRecord;

    void init( const unsigned int p_uiDenominationId = 0,
               const unsigned int p_uiCassaId = 0,
               const unsigned int p_uiLicenceId = 0,
               const int p_inValue = 0,
               const string &p_stArchive = "" )                                 throw();
    void init( const QSqlRecord &p_obRecord )                                   throw();
};

#endif
