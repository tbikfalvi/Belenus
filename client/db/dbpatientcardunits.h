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
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_nLicenceId )         throw();
    unsigned int    patientCardId() const                                   throw();
    void            setPatientCardId( const unsigned int p_nPatientCardId ) throw();
    int             unitTime() const                                        throw();
    void            setUnitTime( const int p_nUnitTime )                    throw();
    QString         validDateFrom() const                                   throw();
    void            setValidDateFrom( const QString &p_qsVDFrom )           throw();
    QString         validDateTo() const                                     throw();
    void            setValidDateTo( const QString &p_qsVDTo )               throw();
    QString         dateTime() const                                        throw();
    void            setDateTime( const QString &p_qsDateTime )              throw();
    bool            active() const                                          throw();
    void            setActive( const bool p_bActive )                       throw();
    QString         archive() const                                         throw();
    void            setArchive( const QString &p_qsArchive )                throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardId;
    int             m_nUnitTime;
    QString         m_qsValidDateFrom;
    QString         m_qsValidDateTo;
    QString         m_qsDateTime;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const int p_nUnitTime = 0,
               const QString &p_qsValidDateFrom = "",
               const QString &p_qsValidDateTo = "",
               const QString &p_qsDateTime = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                         throw();
    void init( const QSqlRecord &p_obRecord )                               throw();
};

#endif
