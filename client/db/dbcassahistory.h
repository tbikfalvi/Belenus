#ifndef DB_CASSAHISTORY_H
#define DB_CASSAHISTORY_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBCassaHistory
{
public:
    cDBCassaHistory();
    ~cDBCassaHistory();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            loadByLedger( const unsigned int p_uiId )           throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            revoke()                                            throw( cSevException );
    void            createNew()                                         throw();
    bool            isRevokeEnabled( const unsigned int p_uiId ) const  throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )    throw();
    unsigned int    parentId() const                                    throw();
    void            setParentId( const unsigned int p_nParentId )       throw();
    unsigned int    cassaId() const                                     throw();
    void            setCassaId( const unsigned int p_uiCassaId )        throw();
    unsigned int    ledgerId() const                                    throw();
    void            setLedgerId( const unsigned int p_uiLedgerId )      throw();
    unsigned int    userId() const                                      throw();
    void            setUserId( const unsigned int p_uiUserId )          throw();
    unsigned int    patientId() const                                   throw();
    void            setPatientId( const unsigned int p_uiPatientId )    throw();
    int             actionValue() const                                 throw();
    void            setActionValue( const int p_inActionValue )         throw();
    int             actionCard() const                                  throw();
    void            setActionCard( const int p_inActionCard )           throw();
    int             actionCash() const                                  throw();
    void            setActionCash( const int p_inActionCash )           throw();
    int             actionBalance() const                               throw();
    void            setActionBalance( const int p_inActionBalance )     throw();
    QString         actionTime() const                                  throw();
    void            setActionTime( const QString &p_qsActionTime )      throw();
    QString         comment() const                                     throw();
    void            setComment( const QString &p_qsComment )            throw();
    QString         modified() const                                    throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_qsArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiParentId;
    unsigned int    m_uiCassaId;
    unsigned int    m_uiLedgerId;
    unsigned int    m_uiUserId;
    unsigned int    m_uiPatientId;
    int             m_inActionValue;
    int             m_inActionCard;
    int             m_inActionCash;
    int             m_inActionBalance;
    QString         m_qsActionTime;
    QString         m_qsComment;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiParentId = 0,
               const unsigned int p_uiCassaId = 0,
               const unsigned int p_uiLedgerId = 0,
               const unsigned int p_uiUserId = 0,
               const unsigned int p_uiPatientId = 0,
               const int p_inActionValue = 0,
               const int p_inActionCard = 0,
               const int p_inActionCash = 0,
               const int p_inActionBalance = 0,
               const QString &p_qsActionTime = "",
               const QString &p_qsComment = "",
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                     throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
