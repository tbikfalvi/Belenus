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
    void            load( const QString &p_qsName )                             throw( cSevException );
    unsigned int    getPatientCount( const QString &p_qsName )                  throw( cSevException );
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
    unsigned int    illnessGroupId() const                                      throw();
    void            setIllnessGroupId( const unsigned int p_nIllnessGroupId )   throw();
    QString         name() const                                                throw();
    void            setName( const QString &p_qsName )                          throw();
    int             gender() const                                              throw();
    void            setGender( const int p_nGender )                            throw();
    QString         dateBirth() const                                           throw();
    void            setDateBirth( const QString &p_qsDateBirth )                throw();
    QString         uniqueId() const                                            throw();
    void            setUniqueId( const QString &p_qsUniqueId )                  throw();
    QString         country() const                                             throw();
    void            setCountry( const QString &p_qsCountry )                    throw();
    QString         region() const                                              throw();
    void            setRegion( const QString &p_qsRegion )                      throw();
    QString         city() const                                                throw();
    void            setCity( const QString &p_qsCity )                          throw();
    QString         zip() const                                                 throw();
    void            setZip( const QString &p_qsZip )                            throw();
    QString         address() const                                             throw();
    void            setAddress( const QString &p_qsAddress )                    throw();
    QString         email() const                                               throw();
    void            setEmail( const QString &p_qsEmail )                        throw();
    QString         phone() const                                               throw();
    void            setPhone( const QString &p_qsPhone )                        throw();
    int             weight() const                                              throw();
    void            setWeight( const int p_nWeight )                            throw();
    int             height() const                                              throw();
    void            setHeight( const int p_nHeight )                            throw();
    QString         medicineCurrent()                                           throw();
    void            setMedicineCurrent( const QString &p_qsMedicineC )          throw();
    QString         medicineAllergy()                                           throw();
    void            setMedicineAllergy( const QString &p_qsMedicineA )          throw();
    QString         comment() const                                             throw();
    void            setComment( const QString &p_qsComment )                    throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    QString         archive() const                                             throw();
    void            setArchive( const QString &p_qsArchive )                    throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPatientOriginId;
    unsigned int    m_uiReasonToVisitId;
    unsigned int    m_uiIllnessGroupId;
    QString         m_qsName;
    int             m_nGender;
    QString         m_qsDateBirth;
    QString         m_qsUniqueId;
    QString         m_qsCountry;
    QString         m_qsRegion;
    QString         m_qsCity;
    QString         m_qsZip;
    QString         m_qsAddress;
    QString         m_qsEmail;
    QString         m_qsPhone;
    int             m_nWeight;
    int             m_nHeight;
    QString         m_qsMedicineCurrent;
    QString         m_qsMedicineAllergy;
    QString         m_qsComment;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPatientOriginId = 0,
               const unsigned int p_uiReasonToVisitId = 0,
               const unsigned int p_uiIllnessGroupId = 0,
               const QString &p_qsName = "",
               const int p_nGender = 0,
               const QString &p_qsDateBirth = "",
               const QString &p_qsUniqueId = "",
               const QString &p_qsCountry = "",
               const QString &p_qsRegion = "",
               const QString &p_qsCity = "",
               const QString &p_qsZip = "",
               const QString &p_qsAddress = "",
               const QString &p_qsEmail = "",
               const QString &p_qsPhone = "",
               const int p_nWeight = 0,
               const int p_nHeight = 0,
               const QString &p_qsMedicineCurrent = "",
               const QString &p_qsMedicineAllergy = "",
               const QString &p_qsComment = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
