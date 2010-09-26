#ifndef DB_LEDGER_H
#define DB_LEDGER_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBLedger
{
public:
    cDBLedger();
    ~cDBLedger();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    ledgerTypeId() const                                            throw();
    void            setLedgerTypeId( const unsigned int p_nLedgerTypeId )           throw();
    unsigned int    userId() const                                                  throw();
    void            setUserId( const unsigned int p_uiUserId )                      throw();
    unsigned int    productId() const                                               throw();
    void            setProductId( const unsigned int p_nProductId )                 throw();
    unsigned int    patientCardTypeId() const                                       throw();
    void            setPatientCardTypeId( const unsigned int p_nPatientCardTypeId ) throw();
    unsigned int    patientCardId() const                                           throw();
    void            setPatientCardId( const unsigned int p_nPatientCardId )         throw();
    unsigned int    panelId() const                                                 throw();
    void            setPanelId( const unsigned int p_nPanelId )                     throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    int             netPrice() const                                                throw();
    void            setNetPrice( const int p_nNetPrice )                            throw();
    int             vatpercent() const                                              throw();
    void            setVatpercent( const int p_nVatpercent )                        throw();
    int             totalPrice() const                                              throw();
    void            setTotalPrice( const int p_nTotalPrice )                        throw();
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
    unsigned int    m_uiLedgerTypeId;
    unsigned int    m_uiUserId;
    unsigned int    m_uiProductId;
    unsigned int    m_uiPatientCardTypeId;
    unsigned int    m_uiPatientCardId;
    unsigned int    m_uiPanelId;
    QString         m_qsName;
    int             m_nNetPrice;
    int             m_nVatpercent;
    int             m_nTotalPrice;
    QString         m_qsLedgerTime;
    QString         m_qsComment;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiLedgerTypeId = 0,
               const unsigned int p_uiUserId = 0,
               const unsigned int p_uiProductId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const unsigned int p_uiPanelId = 0,
               const QString &p_qsName = "",
               const int p_nNetPrice = 0,
               const int p_nVatpercent = 0,
               const int p_nTotalPrice = 0,
               const QString &p_qsLedgerTime = "",
               const QString &p_qsComment = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
