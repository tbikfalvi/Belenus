#ifndef DB_PATIENT_ORIGIN_H
#define DB_PATIENT_ORIGIN_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBDiscount
{
public:
    cDBDiscount();
    ~cDBDiscount();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            load( const QString &p_qsName )                                     throw( cSevException );
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
    int             regularCustomer() const                                             throw();
    void            setRegularCustomer( const int p_inRegularCustomer )                 throw();
    int             employee() const                                                    throw();
    void            setEmployee( const int p_inEmployee )                               throw();
    int             service() const                                                     throw();
    void            setService( const int p_inService )                                 throw();
    QString         name() const                                                        throw();
    void            setName( const QString &p_qsName )                                  throw();
    int             discountValue() const                                               throw();
    void            setDiscountValue( const int p_nDiscountValue )                      throw();
    int             discountPercent() const                                             throw();
    void            setDiscountPercent( const int p_nDiscountPercent )                  throw();
    bool            active() const                                                      throw();
    void            setActive( const bool p_bActive )                                   throw();
    QString         archive() const                                                     throw();
    void            setArchive( const QString &p_qsArchive )                            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiHealthInsuranceId;
    QString         m_qsName;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
