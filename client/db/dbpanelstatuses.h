#ifndef DB_PANELSTATUSES_H
#define DB_PANELSTATUSES_H

#include <QString>
#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanelStatuses
{
public:
    cDBPanelStatuses();
    ~cDBPanelStatuses();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            load( const QString &p_qsName )                                 throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    panelTypeId() const                                             throw();
    void            setPanelTypeId( const unsigned int p_uiPanelTypeId )            throw();
    unsigned int    sequenceNumber() const                                          throw();
    void            setSequenceNumber( const unsigned int p_uiSequenceNumber )      throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    unsigned int    length() const                                                  throw();
    void            setLength( const unsigned int p_uiLength )                      throw();
    unsigned int    activateCommand() const                                         throw();
    void            setActivateCommand( const unsigned int p_uiActivateCommand )    throw();
    int             skipLevel() const                                               throw();
    void            setSkipLevel( const int p_nSkipLevel )                          throw();
    bool            allowedToSkip() const                                           throw();
    void            setAllowedToSkip( const bool p_bAllowedToSkip )                 throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPanelTypeId;
    unsigned int    m_uiSequenceNumber;
    QString         m_qsName;
    unsigned int    m_uiLength;
    unsigned int    m_uiActivateCommand;
    int             m_nSkipLevel;
    bool            m_bAllowedToSkip;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPanelTypeId = 0,
               const unsigned int p_uiSequenceNumber = 0,
               const QString &p_qsName = "",
               const unsigned int p_uiLength = 0,
               const unsigned int p_uiActivateCommand = 0,
               const int m_nSkipLevel = 4,
               const bool m_bAllowedToSkip = false,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                     throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
