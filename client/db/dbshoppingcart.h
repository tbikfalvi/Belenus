#ifndef DB_SHOPPINGCART_H
#define DB_SHOPPINGCART_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBShoppingCart
{
public:

    cDBShoppingCart();
    ~cDBShoppingCart();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_nLicenceId )         throw();
    unsigned int    guestId() const                                         throw();
    void            setGuestId( const unsigned int p_uiGuestId )            throw();
    unsigned int    productId() const                                       throw();
    void            setProductId( const unsigned int p_uiProductId )        throw();
    unsigned int    patientCardId() const                                   throw();
    void            setPatientCardId( const unsigned int p_uiPatientCardId ) throw();
    unsigned int    panelId() const                                         throw();
    void            setPanelId( const unsigned int p_uiPanelId )            throw();
    unsigned int    ledgerTypeId() const                                    throw();
    void            setLedgerTypeId( const unsigned int p_nLedgerTypeId )   throw();
    QString         itemName() const                                        throw();
    void            setItemName( const QString &p_qsItemName )              throw();
    QString         comment() const                                         throw();
    void            setComment( const QString &p_qsComment )                throw();
    int             itemCount() const                                       throw();
    void            setItemCount( const int p_nItemCount )                  throw();
    int             itemNetPrice() const                                    throw();
    void            setItemNetPrice( const int p_nItemNetPrice )            throw();
    int             itemVAT() const                                         throw();
    void            setItemVAT( const int p_nItemVAT )                      throw();
    int             itemDiscount() const                                    throw();
    void            setItemDiscount( const int p_nItemDiscount )            throw();
    int             itemSumPrice() const                                    throw();
    void            setItemSumPrice( const int p_nItemSumPrice )            throw();
    QString         modified() const                                        throw();
    QString         archive() const                                         throw();
    void            setArchive( const QString &p_qsArchive )                throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiGuestId;
    unsigned int    m_uiProductId;
    unsigned int    m_uiPatientCardId;
    unsigned int    m_uiPanelId;
    unsigned int    m_uiLedgerTypeId;
    QString         m_qsItemName;
    QString         m_qsComment;
    int             m_nItemCount;
    int             m_nItemNetPrice;
    int             m_nItemVAT;
    int             m_nItemDiscount;
    int             m_nItemSumPrice;
    QString         m_qsModified;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiGuestId = 0,
               const unsigned int p_uiProductId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const unsigned int p_uiPanelId = 0,
               const unsigned int p_uiLedgerTypeId = 0,
               const QString &p_qsItemName = "",
               const QString p_qsComment = "",
               const int p_nItemCount = 0,
               const int p_nItemNetPrice = 0,
               const int p_nItemVAT = 0,
               const int p_nItemDiscount = 0,
               const int p_nItemSumPrice = 0,
               const QString &p_qsModified = "",
               const QString &p_qsArchive = "NEW" )                         throw();
    void init( const QSqlRecord &p_obRecord )                               throw();
};

#endif
