#ifndef DB_ADVERTISEMENTS_H
#define DB_ADVERTISEMENTS_H

#include <QSqlRecord>
#include <QString>

#include "../../framework/sevexception.h"

class cDBAdvertisements
{
public:
    cDBAdvertisements();
    ~cDBAdvertisements();

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
    QString         caption() const                                     throw();
    void            setCaption( const QString &p_qsCaption )            throw();
    QString         backgroundColor() const                             throw();
    void            setBackgroundColor( const QString &p_qsColor )      throw();
    QString         path() const                                        throw();
    void            setPath( const QString &p_qsPath )                  throw();
    QString         filenames() const                                   throw();
    void            setFilenames( const QString &p_qsFilenames )        throw();
    unsigned int    timerLength() const                                 throw();
    void            setTimerLength( const unsigned int p_nTimerLength ) throw();
    bool            transparent() const                                 throw();
    void            setTransparent( const bool p_bTransparent )         throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_stArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    QString         m_qsCaption;
    QString         m_qsBackgroundColor;
    QString         m_qsPath;
    QString         m_qsFilenames;
    unsigned int    m_nTimerLength;
    bool            m_bTransparent;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsCaption = "",
               const QString &p_qsBackgroundColor = "",
               const QString &p_qsPath = "",
               const QString &p_qsFilenames = "",
               const unsigned int p_uiTimerLength = 0,
               const bool p_bTransparent = false,
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                 throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
