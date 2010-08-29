#ifndef DB_PATIENTCARDHISTORY_H
#define DB_PATIENTCARDHISTORY_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientCardHistory
{
public:
    cDBPatientCardHistory();
    ~cDBPatientCardHistory();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    unsigned int    patientCardId() const                                       throw();
    void            setPatientCardId( const unsigned int p_uiPCardId )          throw();
    QString         dateTime() const                                            throw();
    void            setDateTime( const QString &p_qsDateTime )                  throw();
    int             units() const                                               throw();
    void            setUnits( const int p_nUnits )                              throw();
    QString         time() const                                                throw();
    void            setTime( const QString &p_qsTime )                          throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    QString         archive() const                                             throw();
    void            setArchive( const QString &p_qsArchive )                    throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardId;
    QString         m_qsDateTime;
    int             m_nUnits;
    QString         m_qsTime;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const QString p_qsDateTime = "",
               const int p_nUnits = 0,
               const QString p_qsTime = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                             throw();
    void init( const QSqlRecord &p_obRecord )                                   throw();
};

#endif
