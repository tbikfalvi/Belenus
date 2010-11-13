#ifndef DB_HEALTHINSURANCE_H
#define DB_HEALTHINSURANCE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBHealthInsurance
{
public:
    cDBHealthInsurance();
    ~cDBHealthInsurance();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const QString &p_qsName )                 throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    unsigned int    id() const                                      throw();
    unsigned int    licenceId() const                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId ) throw();
    QString         name() const                                    throw();
    void            setName( const QString &p_qsName )              throw();
    QString         country() const                                                 throw();
    void            setCountry( const QString &p_qsCountry )                        throw();
    QString         region() const                                                  throw();
    void            setRegion( const QString &p_qsRegion )                          throw();
    QString         city() const                                                    throw();
    void            setCity( const QString &p_qsCity )                              throw();
    QString         zip() const                                                     throw();
    void            setZip( const QString &p_qsZip )                                throw();
    QString         address() const                                                 throw();
    void            setAddress( const QString &p_qsAddress )                        throw();
    QString         contactPerson() const                                           throw();
    void            setContactPerson( const QString &p_qsContactPerson )            throw();
    QString         contractId() const                                              throw();
    void            setContractId( const QString &p_qsContractId )                  throw();
    QString         validDateFrom() const                                           throw();
    void            setValidDateFrom( const QString &p_qsValidDateFrom )            throw();
    QString         validDateTo() const                                             throw();
    void            setValidDateTo( const QString &p_qsValidDateTo )                throw();
    int             discountType() const                                            throw();
    void            setDiscountType( const int p_nDiscountType )                    throw();
    int             discount() const                                                throw();
    void            setDiscount( const int p_nDiscount )                            throw();
    QString         modified() const                                                throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_bActive )               throw();
    QString         archive() const                                 throw();
    void            setArchive( const QString &p_qsArchive )        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    QString         m_qsCountry;
    QString         m_qsRegion;
    QString         m_qsCity;
    QString         m_qsZip;
    QString         m_qsAddress;
    QString         m_qsContactPerson;
    QString         m_qsContractId;
    QString         m_qsValidDateFrom;
    QString         m_qsValidDateTo;
    int             m_nDiscountType;
    int             m_nDiscount;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsCountry = "",
               const QString &p_qsRegion = "",
               const QString &p_qsCity = "",
               const QString &p_qsZip = "",
               const QString &p_qsAddress = "",
               const QString &p_qsContactPerson = "",
               const QString &p_qsContractId = "",
               const QString &p_qsValidDateFrom = "",
               const QString &p_qsValidDateTo = "",
               const int p_nDiscountType = 0,
               const int p_nDiscount = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
