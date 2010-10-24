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
    unsigned int    patientId() const                                               throw();
    void            setPatientId( const unsigned int p_nPatientId )                 throw();
    unsigned int    paymentMethod() const                                           throw();
    void            setPaymentMethod( const unsigned int p_nPaymentMethod )         throw();
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
    QString         ledgerTime() const                                              throw();
    void            setLedgerTime( const QString &p_qsLedgerTime )                  throw();
    QString         comment() const                                                 throw();
    void            setComment( const QString &p_qsComment )                        throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiUserId;
    unsigned int    m_uiPanelId;
    unsigned int    m_uiPatientId;
    unsigned int    m_uiPaymentMethod;
    int             m_inUnits;
    int             m_inCash;
    int             m_inTimeReal;
    int             m_inTimeLeft;
    int             m_inTimeCard;
    int             m_inTimeCash;
    QString         m_qsLedgerTime;
    QString         m_qsComment;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiUserId = 0,
               const unsigned int p_uiPanelId = 0,
               const unsigned int p_uiPatientId = 0,
               const unsigned int p_uiPaymentMethodId = 0,
               const int p_inUnits = 0,
               const int p_inCash = 0,
               const int p_inTimeReal = 0,
               const int p_inTimeLeft = 0,
               const int p_inTimeCard = 0,
               const int p_inTimeCash = 0,
               const QString &p_qsLedgerTime = "",
               const QString &p_qsComment = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
