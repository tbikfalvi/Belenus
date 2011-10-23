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
    void            load( const QString &p_qsName )                                 throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    int             netPrice() const                                                throw();
    void            setNetPrice( const int p_nNetPrice )                            throw();
    int             vatPercent() const                                              throw();
    void            setVatPercent( const int p_nVatPercent )                        throw();
    QStringList     productTypes() const                                            throw();
    void            setProductTypes( const QStringList &p_qslProductTypes )         throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();
    bool            isProductTypeLinked( const unsigned int p_uiProductTypeId ) const throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    int             m_nNetPrice;
    int             m_nVatPercent;
    QStringList     m_qslProductTypes;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const int p_nNetPrice = 0,
               const int p_nVatPercent = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
