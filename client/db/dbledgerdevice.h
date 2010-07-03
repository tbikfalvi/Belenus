#ifndef DB_LEDGERDEVICE_H
#define DB_LEDGERDEVICE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBLedgerDevice
{
public:
    cDBLedgerDevice();
    ~cDBLedgerDevice();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    userId() const                                                  throw();
    void            setUserId( const unsigned int p_uiUserId )                      throw();
    unsigned int    panelId() const                                                 throw();
    void            setPanelId( const unsigned int p_nPanelId )                     throw();
    unsigned int    patientCardId() const                                           throw();
    void            setPatientCardId( const unsigned int p_nPatientCardId )         throw();
    int             units() const                                                   throw();
    void            setUnits( const int p_inUnits )                                 throw();
    int             cash() const                                                    throw();
    void            setCash( const int p_inCash )                                   throw();
    int             timeReal() const                                                throw();
    void            setTimeReal( const int p_inTimeReal )                           throw();
    int             timeLeft() const                                                throw();
    void            setTimeLeft( const int p_inTimeLeft )                           throw();
    int             timeCard() const                                                throw();
    void            setTimeCard( const int p_inTimeCard )                           throw();
    int             timeCash() const                                                throw();
    void            setTimeCash( const int p_inTimeCash )                           throw();
    string          ledgerTime() const                                              throw();
    void            setLedgerTime( const string &p_stLedgerTime )                   throw();
    string          comment() const                                                 throw();
    void            setComment( const string &p_stComment )                         throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    string          archive() const                                                 throw();
    void            setArchive( const string &p_stArchive )                         throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiUserId;
    unsigned int    m_uiPanelId;
    unsigned int    m_uiPatientCardId;
    int             m_inUnits;
    int             m_inCash;
    int             m_inTimeReal;
    int             m_inTimeLeft;
    int             m_inTimeCard;
    int             m_inTimeCash;
    string          m_stLedgerTime;
    string          m_stComment;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiUserId = 0,
               const unsigned int p_uiPanelId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const int p_inUnits = 0,
               const int p_inCash = 0,
               const int p_inTimeReal = 0,
               const int p_inTimeLeft = 0,
               const int p_inTimeCard = 0,
               const int p_inTimeCash = 0,
               const string &p_stLedgerTime = "",
               const string &p_stComment = "",
               const bool p_bActive = false,
               const string &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
