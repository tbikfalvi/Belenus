#ifndef DB_WAITLIST_H
#define DB_WAITLIST_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBWaitlist
{
public:
    cDBWaitlist();
    ~cDBWaitlist();

    void            load( const unsigned int p_uiId )                                   throw( cSevException );
    void            load( const QString &p_qsName )                                     throw( cSevException );
    void            save()                                                              throw( cSevException );
    void            remove()                                                            throw( cSevException );
    void            createNew()                                                         throw();
    unsigned int    id() const                                                          throw();
    unsigned int    licenceId() const                                                   throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )                    throw();
    unsigned int    guestId() const                                                     throw();
    void            setGuestId( const unsigned int p_uiGuestId )                        throw();
    unsigned int    PatientCardId() const                                               throw();
    void            setPatientCardId( const unsigned int p_uiPatientCardId )            throw();
    unsigned int    LedgerId() const                                                    throw();
    void            setLedgerId( const unsigned int p_uiLedgerId )                      throw();
    unsigned int    ShoppingCartItemId() const                                          throw();
    void            setShoppingCartItemId( const unsigned int p_uiShoppingCartItemId )  throw();
    unsigned int    PanelTypeId() const                                                 throw();
    void            setPanelTypeId( const unsigned int p_uiPanelTypeId )                throw();
    unsigned int    PayType() const                                                     throw();
    void            setPayType( const unsigned int p_uiPayType )                        throw();
    QString         Barcode() const                                                     throw();
    void            setBarcode( const QString &p_qsBarcode )                            throw();
    QString         UnitIds() const                                                     throw();
    void            setUnitIds( const QString &p_qsUnitIds )                            throw();
    int             LengthCash() const                                                  throw();
    void            setLengthCash( const int p_nLengthCash )                            throw();
    int             LengthCard() const                                                  throw();
    void            setLengthCard( const int p_nLengthCard )                            throw();
    unsigned int    UseTime() const                                                     throw();
    void            setUseTime( const unsigned int p_uiUseTime )                        throw();
    unsigned int    UsePrice() const                                                    throw();
    void            setUsePrice( const unsigned int p_uiUsePrice )                      throw();
    QString         Comment() const                                                     throw();
    void            setComment( const QString &p_qsComment )                            throw();
    QString         modified() const                                                    throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiGuestId;
    unsigned int    m_uiPatientCardId;
    unsigned int    m_uiLedgerId;
    unsigned int    m_uiShoppingCartItemId;
    unsigned int    m_uiPanelTypeId;
    unsigned int    m_uiPayType;
    QString         m_qsBarcode;
    QString         m_qsUnitIds;
    int             m_nLengthCash;
    int             m_nLengthCard;
    unsigned int    m_uiUseTime;
    unsigned int    m_uiUsePrice;
    QString         m_qsComment;
    QString         m_qsModified;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiGuestId = 0,
               const unsigned int p_uiPatientCardId = 0,
               const unsigned int p_uiLedgerId = 0,
               const unsigned int p_uiShoppingCartItemId = 0,
               const unsigned int p_uiPanelTypeId = 0,
               const unsigned int p_uiPayType = 0,
               const QString &p_qsBarcode = "",
               const QString &p_qsUnitIds = "",
               const int p_nLengthCash = 0,
               const int p_nLengthCard = 0,
               const unsigned int p_uiUseTime = 0,
               const unsigned int p_uiUsePrice = 0,
               const QString &p_qsComment = "",
                const QString &p_qsModified = ""
              )                                                         throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
