#ifndef DB_PATIENTCARD_H
#define DB_PATIENTCARD_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPatientCard
{
public:
    cDBPatientCard();
    ~cDBPatientCard();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    bool            isPatientCardTypeLinked( const unsigned int p_PCTId )       throw();
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    unsigned int    patientCardTypeId() const                                   throw();
    void            setPatientCardTypeId( const unsigned int p_uiPCardTypeId )  throw();
    unsigned int    patientId() const                                           throw();
    void            setPatientId( const unsigned int p_uiPatientId )            throw();
    string          barcode() const                                             throw();
    void            setBarcode( const string &p_stBarcode )                     throw();
    string          comment() const                                             throw();
    void            setComment( const string &p_stComment )                     throw();
    int             units() const                                               throw();
    void            setUnits( const int p_nUnits )                              throw();
    string          timeLeft() const                                            throw();
    void            setTimeLeft( const string &p_stTimeLeft )                   throw();
    string          validDate() const                                           throw();
    void            setValidDate( const string &p_stValidDate )                 throw();
    string          pincode() const                                             throw();
    void            setPincode( const string &p_stPincode )                     throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    string          archive() const                                             throw();
    void            setArchive( const string &p_stArchive )                     throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientCardTypeId;
    unsigned int    m_uiPatientId;
    string          m_stBarcode;
    string          m_stComment;
    int             m_nUnits;
    string          m_stTimeLeft;
    string          m_stValidDate;
    string          m_stPincode;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientCardTypeId = 0,
               const unsigned int p_uiPatientId = 0,
               const string p_stBarcode = "",
               const string p_stComment = "",
               const int p_nUnits = 0,
               const string p_stTimeLeft = "",
               const string p_stValidDate = "",
               const string p_stPincode = "",
               const bool p_bActive = false,
               const string &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
