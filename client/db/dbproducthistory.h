#ifndef DB_PRODUCTHISTORY_H
#define DB_PRODUCTHISTORY_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBProductHistory
{
public:
    cDBProductHistory();
    ~cDBProductHistory();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )    throw();
    unsigned int    productId() const                                     throw();
    void            setProductId( const unsigned int p_uiProductId )        throw();
    unsigned int    productActionTypeId() const                                     throw();
    void            setPATypeId( const unsigned int p_uiPATypeId )        throw();
    unsigned int    userId() const                                      throw();
    void            setUserId( const unsigned int p_uiUserId )          throw();
    int             itemCount() const                                 throw();
    void            setItemCount( const int p_inItemCount )         throw();
    int             netPrice() const                                 throw();
    void            setNetPrice( const int p_inNetPrice )         throw();
    int             vatPercent() const                                 throw();
    void            setVatPercent( const int p_inVatPercent )         throw();
    QString         actionTime() const                                  throw();
    void            setActionTime( const QString &p_qsActionTime )      throw();
    QString         modified() const                                    throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_qsArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiProductId;
    unsigned int    m_uiPATypeId;
    unsigned int    m_uiUserId;
    int             m_inItemCount;
    int             m_inNetPrice;
    int             m_inVatPercent;
    QString         m_qsActionTime;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiProductId = 0,
               const unsigned int p_uiProductActionTypeId = 0,
               const unsigned int p_uiUserId = 0,
               const int p_inItemCount = 0,
               const int p_inNetPrice = 0,
               const int p_inVatPercent = 0,
               const QString &p_qsActionTime = "",
               const QString &p_qsComment = "",
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
