#ifndef DB_PRODUCT_H
#define DB_PRODUCT_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBProduct
{
public:
    cDBProduct();
    ~cDBProduct();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            loadName( const QString &p_qsName )                             throw( cSevException );
    void            loadBarcode( const QString &p_qsBarcode )                       throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    QString         barcode() const                                                 throw();
    void            setBarcode( const QString &p_qsBarcode )                        throw();
    int             netPriceBuy() const                                             throw();
    void            setNetPriceBuy( const int p_nNetPriceBuy )                      throw();
    int             vatPercentBuy() const                                           throw();
    void            setVatPercentBuy( const int p_nVatPercentBuy )                  throw();
    int             netPriceSell() const                                            throw();
    void            setNetPriceSell( const int p_nNetPriceSell )                    throw();
    int             vatPercentSell() const                                          throw();
    void            setVatPercentSell( const int p_nVatPercentSell )                throw();
    int             productCount() const                                            throw();
    void            setProductCount( const int p_nProductCount )                    throw();
    void            increaseProductCount( const int p_nCount )                      throw();
    void            decreaseProductCount( const int p_nCount )                      throw();
    QStringList     productTypes() const                                            throw();
    void            setProductTypes( const QStringList &p_qslProductTypes )         throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();
    bool            isProductLinkedToProductType() const                            throw();
    int             getDiscountedPrice( const int p_inPriceTotal )                  throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    QString         m_qsBarcode;
    int             m_nNetPriceBuy;
    int             m_nVatPercentBuy;
    int             m_nNetPriceSell;
    int             m_nVatPercentSell;
    int             m_nProductCount;
    QStringList     m_qslProductTypes;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsBarcode = "",
               const int p_nNetPriceBuy = 0,
               const int p_nVatPercentBuy = 0,
               const int p_nNetPriceSell = 0,
               const int p_nVatPercentSell = 0,
               const int p_nProductCount = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
