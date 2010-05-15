#ifndef DB_PATIENTCARDTYPE_H
#define DB_PATIENTCARDTYPE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientCardType
{
public:
    cDBPatientCardType();
    ~cDBPatientCardType();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            load( const string &p_stName )                      throw( cSevException );
    void            save()                                              throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    string          name() const                                        throw();
    void            setName( const string &p_stName )                   throw();
    float           price() const                                       throw();
    void            setPrice( const float p_fPrice )                    throw();
    int             units() const                                       throw();
    void            setUnits( const int p_nUnits )                      throw();
    string          validDateFrom() const                               throw();
    void            setValidDateFrom( const string &p_stVDFrom )        throw();
    string          validDateTo() const                                 throw();
    void            setValidDateTo( const string &p_stVDTo )            throw();
    int             validDays() const                                   throw();
    void            setValidDays( const int p_nValidDays )              throw();
    int             unitTime() const                                    throw();
    void            setUnitTime( const int p_nUnitTime )                throw();
    string          archive() const                                     throw();
    void            setArchive( const string &p_stArchive )             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    string          m_stName;
    float           m_fPrice;
    int             m_nUnits;
    string          m_stValidDateFrom;
    string          m_stValidDateTo;
    int             m_nValidDays;
    int             m_nUnitTime;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const string &p_stName = "",
               const float p_fPrice = 0,
               const int p_nUnits = 0,
               const string &p_stValidDateFrom = "",
               const string &p_stValidDateTo = "",
               const int p_nValidDays = 0,
               const int p_nUnitTime = 0,
               const string &p_stArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
