#ifndef DB_ADDRESS_H
#define DB_ADDRESS_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBAddress
{
public:
    cDBAddress();
    ~cDBAddress();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            load( const QString &p_qsName )                                 throw( cSevException );
    void            loadPatientSelected( const unsigned int p_uiId )                throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    patientId() const                                               throw();
    void            setPatientId( const unsigned int p_nPatientId )                 throw();
    unsigned int    publicPlaceId() const                                           throw();
    void            setPublicPlaceId( const unsigned int p_nPublicPlaceId )         throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    QString         country() const                                                 throw();
    void            setCountry( const QString &p_qsCountry )                        throw();
    QString         region() const                                                  throw();
    void            setRegion( const QString &p_qsRegion )                          throw();
    QString         city() const                                                    throw();
    void            setCity( const QString &p_qsCity )                              throw();
    QString         zip() const                                                     throw();
    void            setZip( const QString &p_qsZip )                                throw();
    QString         street() const                                                  throw();
    void            setStreet( const QString &p_qsStreet )                          throw();
    QString         streetNumber() const                                            throw();
    void            setStreetNumber( const QString &p_qsStreetNumber )              throw();
    QString         floor() const                                                   throw();
    void            setFloor( const QString &p_qsFloor )                            throw();
    QString         door() const                                                    throw();
    void            setDoor( const QString &p_qsDoor )                              throw();
    bool            primaryAddress() const                                          throw();
    void            setPrimaryAddress( const bool p_bPrimaryAddress )               throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientId;
    unsigned int    m_uiPublicPlaceId;
    QString         m_qsName;
    QString         m_qsCountry;
    QString         m_qsRegion;
    QString         m_qsCity;
    QString         m_qsZip;
    QString         m_qsStreet;
    QString         m_qsStreetNumber;
    QString         m_qsFloor;
    QString         m_qsDoor;
    bool            m_bPrimaryAddress;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientId = 0,
               const unsigned int p_uiPublicPlaceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsCountry = "",
               const QString &p_qsRegion = "",
               const QString &p_qsCity = "",
               const QString &p_qsZip = "",
               const QString &p_qsStreet = "",
               const QString &p_qsStreetNumber = "",
               const QString &p_qsFloor = "",
               const QString &p_qsDoor = "",
               const bool p_bPrimaryAddress = false,
               const bool p_bActive = false,
               const QString &p_qsArchive = "NEW" )                 throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
