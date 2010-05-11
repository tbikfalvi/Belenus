#ifndef DB_PATIENTCARD_H
#define DB_PATIENTCARD_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientCard
{
public:
    cDBPatientCard();
    ~cDBPatientCard();

    void          load( const unsigned int p_uiId )                 throw( cSevException );
    void          load( const string &p_stName )                    throw( cSevException );
    void          save()                                            throw( cSevException );
    void          createNew()                                       throw();
    unsigned int  id() const                                        throw();
    unsigned int  licenceId() const                                 throw();
    void          setLicenceId( const unsigned int p_nLicenceId )   throw();
    string        name() const                                      throw();
    void          setName( const string &p_stName )                 throw();
    string        archive() const                                   throw();
    void          setArchive( const string &p_stArchive )           throw();

private:
    unsigned int  m_uiId;
    unsigned int  m_uiLicenceId;
    string        m_stName;
    string        m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const string &p_stName = "",
               const string &p_stArchive = "NEW" )                   throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
