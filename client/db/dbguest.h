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
    unsigned int    getGuestCount( const QString &p_qsName )                        throw( cSevException );
    int             getDiscountedPrice( const int p_inPriceTotal )                  throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    companyId() const                                               throw();
    void            setCompanyId( const unsigned int p_nCompanyId )                 throw();
    QString         dateCreated() const                                             throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    int             gender() const                                                  throw();
    void            setGender( const int p_nGender )                                throw();
    int             ageType() const                                                 throw();
    void            setAgeType( const int p_nAgeType )                              throw();
    bool            isReturning() const                                             throw();
    void            setIsReturning( const bool p_bIsReturning )                     throw();
    QString         uniqueId() const                                                throw();
    void            setUniqueId( const QString &p_qsUniqueId )                      throw();
    QString         email() const                                                   throw();
    void            setEmail( const QString &p_qsEmail )                            throw();
    bool            regularCustomer() const                                         throw();
    void            setRegularCustomer( const bool p_bRegularCustomer )             throw();
    bool            employee() const                                                throw();
    void            setEmployee( const bool p_bEmployee )                           throw();
    bool            service() const                                                 throw();
    void            setService( const bool p_bService )                             throw();
    bool            company() const                                                 throw();
    void            setCompany( const bool p_bCompany )                             throw();
    int             discountType() const                                            throw();
    void            setDiscountType( const int p_inDiscountType )                   throw();
    QString         comment() const                                                 throw();
    void            setComment( const QString &p_qsComment )                        throw();
    int             loyaltyPoints() const                                           throw();
    void            setLoyaltyPoints( const int p_inLoyaltyPoints )                 throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiCompanyId;
    QString         m_qsDateCreated;
    QString         m_qsName;
    int             m_nGender;
    int             m_nAgeType;
    bool            m_bIsReturning;
    QString         m_qsUniqueId;
    QString         m_qsEmail;
    bool            m_bRegularCustomer;
    bool            m_bEmployee;
    bool            m_bService;
    bool            m_bCompany;
    int             m_inDiscountType;
    QString         m_qsComment;
    int             m_inLoyaltyPoints;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiCompanyId = 0,
               const QString &p_qsDateCreated = "",
               const QString &p_qsName = "",
               const int p_nGender = 0,
               const int p_nAgeType = 0,
               const bool p_bIsReturning = false,
               const QString &p_qsUniqueId = "",
               const QString &p_qsEmail = "",
               const bool p_bRegularCustomer = false,
               const bool p_bEmployee = false,
               const bool p_bService = false,
               const bool p_bCompany = false,
               const int p_inDiscountType = 2,
               const QString &p_qsComment = "",
               const int p_inLoyaltyPoints = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
