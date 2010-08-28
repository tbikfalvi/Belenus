#ifndef DB_PANELS_H
#define DB_PANELS_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanel
{
public:
    cDBPanel();
    ~cDBPanel();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )        throw();
    unsigned int    panelTypeId() const                                     throw();
    void            setPanelTypeId( const unsigned int p_uiPanelTypeId )    throw();
    QString         title() const                                           throw();
    void            setTitle( const QString &p_qsTitle )                    throw();
    unsigned int    workTime() const                                        throw();
    void            setWorkTime( const unsigned int p_uiWorkTime)           throw();
    unsigned int    maxWorkTime() const                                     throw();
    void            setMaxWorkTime( const unsigned int p_uiMaxWorkTime)     throw();
    bool            active() const                                          throw();
    void            setActive( const bool p_bActive )                       throw();
    QString         archive() const                                         throw();
    void            setArchive( const QString &p_qsArchive )                throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPanelTypeId;
    QString         m_qsTitle;
    unsigned int    m_uiWorkTime;
    unsigned int    m_uiMaxWorkTime;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPanelTypeId = 0,
               const QString &p_qsTitle = "",
               const unsigned int p_uiWorkTime = 0,
               const unsigned int p_uiMaxWorkTime = 0,
               const bool p_bActive = false,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
