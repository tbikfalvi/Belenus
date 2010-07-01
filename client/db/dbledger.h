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
    unsigned int    userId() const                                                  throw();
    void            setUserId( const unsigned int p_uiUserId )                      throw();
    unsigned int    productId() const                                               throw();
    void            setProductId( const unsigned int p_nProductId )                 throw();
    unsigned int    patientCardTypeId() const                                       throw();
    void            setPatientCardTypeId( const unsigned int p_nPatientCardTypeId ) throw();
    unsigned int    panelId() const                                                 throw();
    void            setPanelId( const unsigned int p_nPanelId )                     throw();
    string          name() const                                                    throw();
    void            setName( const string &p_stName )                               throw();
    int             netPrice() const                                                throw();
    void            setNetPrice( const int p_nNetPrice )                            throw();
    int             vatpercent() const                                              throw();
    void            setVatpercent( const int p_nVatpercent )                        throw();
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
    unsigned int    m_uiProductId;
    unsigned int    m_uiPatientCardTypeId;
    unsigned int    m_uiPanelId;
    string          m_stName;
    int             m_nNetPrice;
    int             m_nVatpercent;
    string          m_stLedgerTime;
    string          m_stComment;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiUserId = 0,
               const unsigned int p_uiProductId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const unsigned int p_uiPanelId = 0,
               const string &p_stName = "",
               const int p_nNetPrice = 0,
               const int p_nVatpercent = 0,
               const string &p_stLedgerTime = "",
               const string &p_stComment = "",
               const bool p_bActive = false,
               const string &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
