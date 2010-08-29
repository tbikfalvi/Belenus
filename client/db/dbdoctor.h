#ifndef DB_DOCTOR_H
#define DB_DOCTOR_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBDoctor
{
public:
    cDBDoctor();
    ~cDBDoctor();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const QString &p_qsName )                 throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    unsigned int    id() const                                      throw();
    unsigned int    licenceId() const                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId ) throw();
    QString         name() const                                    throw();
    void            setName( const QString &p_qsName )              throw();
    QString         licence() const                                 throw();
    void            setLicence( const QString &p_qsLicence )        throw();
    QString         data() const                                    throw();
    void            setData( const QString &p_qsData )              throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_bActive )               throw();
    QString         archive() const                                 throw();
    void            setArchive( const QString &p_qsArchive )        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    QString         m_qsLicence;
    QString         m_qsData;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsLicence = "",
               const QString &p_qsData = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
