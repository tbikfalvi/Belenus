#ifndef DB_PATIENT_H
#define DB_PATIENT_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatient
{
public:
    cDBPatient();
    ~cDBPatient();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            load( const string &p_stName )                              throw( cSevException );
    unsigned int    getPatientCount( const string &p_stName )                   throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    unsigned int    patientOriginId() const                                     throw();
    void            setPatientOriginId( const unsigned int p_nPatientOriginId ) throw();
    unsigned int    reasonToVisitId() const                                     throw();
    void            setReasonToVisitId( const unsigned int p_nReasonToVisitId ) throw();
    string          name() const                                                throw();
    void            setName( const string &p_stName )                           throw();
    int             gender() const                                              throw();
    void            setGender( const int p_nGender )                            throw();
    string          dateBirth() const                                           throw();
    void            setDateBirth( const string &p_stDateBirth )                 throw();
    string          uniqueId() const                                            throw();
    void            setUniqueId( const string &p_stUniqueId )                   throw();
    string          country() const                                             throw();
    void            setCountry( const string &p_stCountry )                     throw();
    string          region() const                                              throw();
    void            setRegion( const string &p_stRegion )                       throw();
    string          city() const                                                throw();
    void            setCity( const string &p_stCity )                           throw();
    string          zip() const                                                 throw();
    void            setZip( const string &p_stZip )                             throw();
    string          address() const                                             throw();
    void            setAddress( const string &p_stAddress )                     throw();
    string          email() const                                               throw();
    void            setEmail( const string &p_stEmail )                         throw();
    string          phone() const                                               throw();
    void            setPhone( const string &p_stPhone )                         throw();
    string          comment() const                                             throw();
    void            setComment( const string &p_stComment )                     throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    string          archive() const                                             throw();
    void            setArchive( const string &p_stArchive )                     throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientOriginId;
    unsigned int    m_uiReasonToVisitId;
    string          m_stName;
    int             m_nGender;
    string          m_stDateBirth;
    string          m_stUniqueId;
    string          m_stCountry;
    string          m_stRegion;
    string          m_stCity;
    string          m_stZip;
    string          m_stAddress;
    string          m_stEmail;
    string          m_stPhone;
    string          m_stComment;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientOriginId = 0,
               const unsigned int p_uiReasonToVisitId = 0,
               const string &p_stName = "",
               const int p_nGender = 0,
               const string &p_stDateBirth = "",
               const string &p_stUniqueId = "",
               const string &p_stCountry = "",
               const string &p_stRegion = "",
               const string &p_stCity = "",
               const string &p_stZip = "",
               const string &p_stAddress = "",
               const string &p_stEmail = "",
               const string &p_stPhone = "",
               const string &p_stComment = "",
               const bool p_bActive = true,
               const string &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
