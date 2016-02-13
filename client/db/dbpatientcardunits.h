#ifndef DB_PATIENTCARDUNIT_H
#define DB_PATIENTCARDUNIT_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientcardUnit
{
public:
    cDBPatientcardUnit();
    ~cDBPatientcardUnit();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    QStringList     loadPCId( const unsigned int p_uiId )                   throw( cSevException );
    void            replacePatientCard( const unsigned int p_uiId )         throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            removeLedgerUnits( const unsigned int p_uiId )          throw( cSevException );
    void            deactivateUnits( const unsigned int p_uiId )            throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_nLicenceId )         throw();
    unsigned int    patientCardId() const                                   throw();
    void            setPatientCardId( const unsigned int p_nPatientCardId ) throw();
    unsigned int    patientCardTypeId() const                                   throw();
    void            setPatientCardTypeId( const unsigned int p_nPatientCardTypeId ) throw();
    unsigned int    ledgerId() const                                        throw();
    void            setLedgerId( const unsigned int p_nLedgerId )           throw();
    unsigned int    panelId() const                                         throw();
    void            setPanelId( const unsigned int p_nPanelId )             throw();
    int             unitTime() const                                        throw();
    void            setUnitTime( const int p_nUnitTime )                    throw();
    int             unitPrice() const                                       throw();
    void            setUnitPrice( const int p_nUnitPrice )                  throw();
    QString         validDateFrom() const                                   throw();
    void            setValidDateFrom( const QString &p_qsVDFrom )           throw();
    QString         validDateTo() const                                     throw();
    void            setValidDateTo( const QString &p_qsVDTo )               throw();
    QString         dateTime() const                                        throw();
    void            setDateTime( const QString &p_qsDateTime )              throw();
    bool            prepared() const                                        throw();
    void            setPrepared( const bool p_bPrepared )                   throw();
    bool            active() const                                          throw();
    void            setActive( const bool p_bActive )                       throw();
    QString         archive() const                                         throw();
    void            setArchive( const QString &p_qsArchive )                throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardId;
    unsigned int    m_uiPatientCardTypeId;
    unsigned int    m_uiLedgerId;
    unsigned int    m_uiPanelId;
    int             m_nUnitTime;
    int             m_nUnitPrice;
    QString         m_qsValidDateFrom;
    QString         m_qsValidDateTo;
    QString         m_qsDateTime;
    bool            m_bPrepared;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const unsigned int p_uiLedgerId = 0,
               const unsigned int p_uiPanelId = 0,
               const int p_nUnitTime = 0,
               const int p_nUnitPrice = 0,
               const QString &p_qsValidDateFrom = "",
               const QString &p_qsValidDateTo = "",
               const QString &p_qsDateTime = "",
               const bool p_bPrepared = false,
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                         throw();
    void init( const QSqlRecord &p_obRecord )                               throw();
};

#endif
