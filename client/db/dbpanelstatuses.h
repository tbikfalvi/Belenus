#ifndef DB_PANELSTATUSES_H
#define DB_PANELSTATUSES_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanelStatuses
{
public:
    cDBPanelStatuses();
    ~cDBPanelStatuses();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            load( const string &p_stName )                                  throw( cSevException );
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
    string          name() const                                                    throw();
    void            setName( const string &p_stName )                               throw();
    unsigned int    length() const                                                  throw();
    void            setLength( const unsigned int p_uiLength )                      throw();
    unsigned int    activateCommand() const                                         throw();
    void            setActivateCommand( const unsigned int p_uiActivateCommand )    throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    string          archive() const                                                 throw();
    void            setArchive( const string &p_stArchive )                         throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPanelTypeId;
    unsigned int    m_uiSequenceNumber;
    string          m_stName;
    unsigned int    m_uiLength;
    unsigned int    m_uiActivateCommand;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPanelTypeId = 0,
               const unsigned int p_uiSequenceNumber = 0,
               const string &p_stName = "",
               const unsigned int p_uiLength = 0,
               const unsigned int p_uiActivateCommand = 0,
               const bool p_bActive = true,
               const string &p_stArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
