#ifndef DB_DISCOUNT_H
#define DB_DISCOUNT_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBDiscount
{
public:
    cDBDiscount();
    ~cDBDiscount();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            load( const QString &p_qsName )                                     throw( cSevException );
    void            loadHealthInsurance( const unsigned int p_uiId )                    throw( cSevException );
    void            loadCompany( const unsigned int p_uiId )                            throw( cSevException );
    void            loadDoctor( const unsigned int p_uiId )                             throw( cSevException );
    bool            isRegularCustomerExists()                                           throw( cSevException );
    bool            isEmployeeExists()                                                  throw( cSevException );
    bool            isServiceExists()                                                   throw( cSevException );
    bool            isHealthInsuranceExists( const unsigned int p_uiId )                throw( cSevException );
    bool            isCompanyExists( const unsigned int p_uiId )                        throw( cSevException );
    bool            isDoctorExists( const unsigned int p_uiId )                         throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            remove()                                                            throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id() const                                                          throw();
    unsigned int    licenceId() const                                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                     throw();
    unsigned int    healthInsuranceId() const                                           throw();
    void            setHealthInsuranceId( const unsigned int p_uiHealthInsuranceId )    throw();
    unsigned int    companyId() const                                                   throw();
    void            setCompanyId( const unsigned int p_uiCompanyId )                    throw();
    unsigned int    doctorId() const                                                    throw();
    void            setDoctorId( const unsigned int p_uiDoctorId )                      throw();
    bool            regularCustomer() const                                             throw();
    void            setRegularCustomer( const bool p_bRegularCustomer )                 throw();
    bool            employee() const                                                    throw();
    void            setEmployee( const bool p_bEmployee )                               throw();
    bool            service() const                                                     throw();
    void            setService( const bool p_bService )                                 throw();
    QString         name() const                                                        throw();
    void            setName( const QString &p_qsName )                                  throw();
    int             discountValue() const                                               throw();
    void            setDiscountValue( const int p_nDiscountValue )                      throw();
    int             discountPercent() const                                             throw();
    void            setDiscountPercent( const int p_nDiscountPercent )                  throw();
    QString         modified() const                                                throw();
    bool            active() const                                                      throw();
    void            setActive( const bool p_bActive )                                   throw();
    QString         archive() const                                                     throw();
    void            setArchive( const QString &p_qsArchive )                            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiHealthInsuranceId;
    unsigned int    m_uiCompanyId;
    unsigned int    m_uiDoctorId;
    bool            m_bRegularCustomer;
    bool            m_bEmployee;
    bool            m_bService;
    QString         m_qsName;
    int             m_inDiscountValue;
    int             m_inDiscountPercent;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiHealthInsuranceId = 0,
               const unsigned int p_uiCompanyId = 0,
               const unsigned int p_uiDoctorId = 0,
               const bool p_bRegularCustomer = false,
               const bool p_bEmployee = false,
               const bool p_bService = false,
               const QString &p_qsName = "",
               const int p_inDiscountValue = 0,
               const int p_inDiscountPercent = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
