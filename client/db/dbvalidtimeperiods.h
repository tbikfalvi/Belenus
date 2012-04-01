#ifndef DB_VALIDTIMEPERIOD_H
#define DB_VALIDTIMEPERIOD_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBValidTimePeriod
{
public:
    cDBValidTimePeriod();
    ~cDBValidTimePeriod();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    QStringList     loadPeriods( const unsigned int p_uiId )                        const throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            saveList( const QStringList &m_qslPeriods )                     throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            removePatienCardTypes( const unsigned int p_uiId )              throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    patientCardTypeId() const                                       throw();
    void            setPatientCardTypeId( const unsigned int p_uiPCardTypeId )      throw();
    QString         startTime() const                                               throw();
    void            setStartTime( const QString &p_qsStartTime )                    throw();
    QString         stopTime() const                                                throw();
    void            setStopTime( const QString &p_qsStopTime )                      throw();
    QString         modified() const                                                throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardTypeId;
    QString         m_qsStartTime;
    QString         m_qsStopTime;
    QString         m_qsModified;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const QString &p_qsStartTime = "",
               const QString &p_qsStopTime = "",
               const QString &p_qsModified = "",
               const QString &p_qsArchive = "NEW" )                                 throw();
    void init( const QSqlRecord &p_obRecord )                                       throw();
};

#endif
