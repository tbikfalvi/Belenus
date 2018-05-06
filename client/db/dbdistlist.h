#ifndef DB_DISTLIST_H
#define DB_DISTLIST_H

#include <QSqlRecord>
#include <QString>

#include "../../framework/sevexception.h"

class cDBDistList
{
public:
    cDBDistList();
    ~cDBDistList();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            load( const QString &p_stName )                     throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    QString         name() const                                        throw();
    void            setName( const QString &p_qsName )                  throw();
    QString         description() const                                 throw();
    void            setDescription( const QString &p_qsDescription )    throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_stArchive )            throw();
    void            addPatients( const QString &p_qsPatientIds )        throw();
    void            removePatients()                                    throw();
    QStringList     patients() const                                    throw();
    void            setPatients( const QStringList &p_qslPatients )     throw();
    QString         recipients()                                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    QString         m_qsDescription;
    bool            m_bActive;
    QString         m_qsArchive;
    QStringList     m_qslPatients;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsDescription = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                 throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
