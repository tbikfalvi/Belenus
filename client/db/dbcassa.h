#ifndef DB_CASSA_H
#define DB_CASSA_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBCassa
{
public:
    cDBCassa();
    ~cDBCassa();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    void            load()                                                  throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_nLicenceId )         throw();
    unsigned int    userId() const                                          throw();
    void            setUserId( const unsigned int p_nUserId )               throw();
    int             currentBalance() const                                  throw();
    void            setCurrentBalance( const int p_nCurrentBalance )        throw();
    QString         startDateTime() const                                   throw();
    void            setStartDateTime( const QString &p_qsStartDateTime )    throw();
    QString          stopDateTime() const                                   throw();
    void            setStopDateTime( const QString &p_qsStopDateTime )      throw();
    bool            active() const                                          throw();
    void            setActive( const bool p_bActive )                       throw();
    QString          archive() const                                        throw();
    void            setArchive( const QString &p_qsArchive )                throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiUserId;
    int             m_nCurrentBalance;
    QString         m_qsStartDateTime;
    QString         m_qsStopDateTime;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiUserId = 0,
               const int p_nCurrentBalance = 0,
               const QString p_qsStartDateTime = "",
               const QString p_qsStopDateTime = "",
               const bool p_bActive = false,
               const QString &p_qsArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
