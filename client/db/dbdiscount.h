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
    void            loadGuest( const unsigned int p_uiId )                              throw( cSevException );
    void            loadCompany( const unsigned int p_uiId )                            throw( cSevException );
    void            loadPaymentMethod( const unsigned int p_uiId )                      throw( cSevException );
    void            loadProduct( const unsigned int p_uiId )                            throw( cSevException );
    void            loadTimeZone()                                                      throw( cSevException );
    bool            isRegularCustomerExists()                                           throw( cSevException );
    bool            isEmployeeExists()                                                  throw( cSevException );
    bool            isServiceExists()                                                   throw( cSevException );
    bool            isGuestExists( const unsigned int p_uiId )                          throw( cSevException );
    bool            isCompanyExists( const unsigned int p_uiId )                        throw( cSevException );
    bool            isPaymentMethodExists( const unsigned int p_uiId )                  throw( cSevException );
    bool            isProductExists( const unsigned int p_uiId )                        throw( cSevException );
    bool            isCouponExists( const QString &p_qsName )                           throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            remove()                                                            throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id() const                                                          throw();
    unsigned int    licenceId() const                                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                     throw();
    unsigned int    guestId() const                                                     throw();
    void            setGuestId( const unsigned int p_uiGuestId )                        throw();
    unsigned int    companyId() const                                                   throw();
    void            setCompanyId( const unsigned int p_uiCompanyId )                    throw();
    unsigned int    paymentMethodId() const                                             throw();
    void            setPaymentMethodId( const unsigned int p_uiPaymentMethodId )        throw();
    unsigned int    productId() const                                                   throw();
    void            setProductId( const unsigned int p_uiPaymentMethodId )              throw();
    bool            regularCustomer() const                                             throw();
    void            setRegularCustomer( const bool p_bRegularCustomer )                 throw();
    bool            employee() const                                                    throw();
    void            setEmployee( const bool p_bEmployee )                               throw();
    bool            service() const                                                     throw();
    void            setService( const bool p_bService )                                 throw();
    QString         timezoneStart() const                                               throw();
    void            setTimezoneStart( const QString &p_qsTimezoneStart )                throw();
    QString         timezoneStop() const                                                throw();
    void            setTimezoneStop( const QString &p_qsTimezoneStop )                  throw();
    QString         name() const                                                        throw();
    void            setName( const QString &p_qsName )                                  throw();
    int             discountValue() const                                               throw();
    void            setDiscountValue( const int p_nDiscountValue )                      throw();
    int             discountPercent() const                                             throw();
    void            setDiscountPercent( const int p_nDiscountPercent )                  throw();
    QString         modified() const                                                    throw();
    bool            active() const                                                      throw();
    void            setActive( const bool p_bActive )                                   throw();
    QString         archive() const                                                     throw();
    void            setArchive( const QString &p_qsArchive )                            throw();

    int             discountedValue( int p_inValue );
    int             discount( int p_inValue );

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiGuestId;
    unsigned int    m_uiCompanyId;
    unsigned int    m_uiPaymentMethodId;
    unsigned int    m_uiProductId;
    bool            m_bRegularCustomer;
    bool            m_bEmployee;
    bool            m_bService;
    QString         m_qsTimezoneStart;
    QString         m_qsTimezoneStop;
    QString         m_qsName;
    int             m_inDiscountValue;
    int             m_inDiscountPercent;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiGuestId = 0,
               const unsigned int p_uiCompanyId = 0,
               const unsigned int p_uiPaymentMethodId = 0,
               const unsigned int p_uiProductId = 0,
               const bool p_bRegularCustomer = false,
               const bool p_bEmployee = false,
               const bool p_bService = false,
               const QString &p_qsTimezoneStart = "",
               const QString &p_qsTimezoneStop = "",
               const QString &p_qsName = "",
               const int p_inDiscountValue = 0,
               const int p_inDiscountPercent = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
