#ifndef DB_GUEST_H
#define DB_GUEST_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBGuest
{
public:
    cDBGuest();
    ~cDBGuest();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            load( const QString &p_qsName )                                 throw( cSevException );
    unsigned int    getGuestCount( const QString &p_qsName )                      throw( cSevException );
    int             getDiscountPrice( const int p_inPriceTotal )                    throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    patientOriginId() const                                         throw();
    void            setGuestOriginId( const unsigned int p_nGuestOriginId )     throw();
    unsigned int    reasonToVisitId() const                                         throw();
    void            setReasonToVisitId( const unsigned int p_nReasonToVisitId )     throw();
    unsigned int    illnessGroupId() const                                          throw();
    void            setIllnessGroupId( const unsigned int p_nIllnessGroupId )       throw();
    unsigned int    healthInsuranceId() const                                       throw();
    void            setHealthInsuranceId( const unsigned int p_nHealthInsuranceId ) throw();
    unsigned int    companyId() const                                               throw();
    void            setCompanyId( const unsigned int p_nCompanyId )                 throw();
    unsigned int    doctorId() const                                                throw();
    void            setDoctorId( const unsigned int p_nDoctorId )                   throw();
    QString         dateCreated() const                                             throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    int             gender() const                                                  throw();
    void            setGender( const int p_nGender )                                throw();
    QString         dateBirth() const                                               throw();
    void            setDateBirth( const QString &p_qsDateBirth )                    throw();
    QString         uniqueId() const                                                throw();
    void            setUniqueId( const QString &p_qsUniqueId )                      throw();
    QString         email() const                                                   throw();
    void            setEmail( const QString &p_qsEmail )                            throw();
    QString         phone() const                                                   throw();
    void            setPhone( const QString &p_qsPhone )                            throw();
    int             weight() const                                                  throw();
    void            setWeight( const int p_nWeight )                                throw();
    int             height() const                                                  throw();
    void            setHeight( const int p_nHeight )                                throw();
    QString         illnesses()                                                     throw();
    void            setIllnesses( const QString &p_qsIllnesses )                    throw();
    QString         symptoms()                                                      throw();
    void            setSymptoms( const QString &p_qsSymptoms )                      throw();
    QString         medicineCurrent()                                               throw();
    void            setMedicineCurrent( const QString &p_qsMedicineC )              throw();
    QString         medicineAllergy()                                               throw();
    void            setMedicineAllergy( const QString &p_qsMedicineA )              throw();
    bool            regularCustomer() const                                         throw();
    void            setRegularCustomer( const bool p_bRegularCustomer )             throw();
    bool            employee() const                                                throw();
    void            setEmployee( const bool p_bEmployee )                           throw();
    bool            service() const                                                 throw();
    void            setService( const bool p_bService )                             throw();
    bool            healthInsurance() const                                         throw();
    void            setHealthInsurance( const bool p_bHealthInsurance )             throw();
    bool            company() const                                                 throw();
    void            setCompany( const bool p_bCompany )                             throw();
    bool            doctorProposed() const                                          throw();
    void            setDoctorProposed( const bool p_bDoctorProposed )               throw();
    int             discountType() const                                            throw();
    void            setDiscountType( const int p_inDiscountType )                   throw();
    QString         comment() const                                                 throw();
    void            setComment( const QString &p_qsComment )                        throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiGuestOriginId;
    unsigned int    m_uiReasonToVisitId;
    unsigned int    m_uiIllnessGroupId;
    unsigned int    m_uiHealthInsuranceId;
    unsigned int    m_uiCompanyId;
    unsigned int    m_uiDoctorId;
    QString         m_qsDateCreated;
    QString         m_qsName;
    int             m_nGender;
    QString         m_qsDateBirth;
    QString         m_qsUniqueId;
    QString         m_qsEmail;
    QString         m_qsPhone;
    int             m_nWeight;
    int             m_nHeight;
    QString         m_qsIllnesses;
    QString         m_qsSymptoms;
    QString         m_qsMedicineCurrent;
    QString         m_qsMedicineAllergy;
    bool            m_bRegularCustomer;
    bool            m_bEmployee;
    bool            m_bService;
    bool            m_bHealthInsurance;
    bool            m_bCompany;
    bool            m_bDoctorProposed;
    int             m_inDiscountType;
    QString         m_qsComment;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiGuestOriginId = 0,
               const unsigned int p_uiReasonToVisitId = 0,
               const unsigned int p_uiIllnessGroupId = 0,
               const unsigned int p_uiHealthInsuranceId = 0,
               const unsigned int p_uiCompanyId = 0,
               const unsigned int p_uiDoctorId = 0,
               const QString &p_qsDateCreated = "",
               const QString &p_qsName = "",
               const int p_nGender = 0,
               const QString &p_qsDateBirth = "",
               const QString &p_qsUniqueId = "",
               const QString &p_qsEmail = "",
               const QString &p_qsPhone = "",
               const int p_nWeight = 0,
               const int p_nHeight = 0,
               const QString &p_qsIllnesses = "",
               const QString &p_qsSymptoms = "",
               const QString &p_qsMedicineCurrent = "",
               const QString &p_qsMedicineAllergy = "",
               const bool p_bRegularCustomer = false,
               const bool p_bEmployee = false,
               const bool p_bService = false,
               const bool p_bHealthInsurance = false,
               const bool p_bCompany = false,
               const bool p_bDoctorProposed = false,
               const int p_inDiscountType = 2,
               const QString &p_qsComment = "",
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
