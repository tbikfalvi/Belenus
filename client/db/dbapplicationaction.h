#ifndef DB_APPLICATIONACTION_H
#define DB_APPLICATIONACTION_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBApplicationAction
{
public:

    enum teAppAction {
        APPACT_EXITAPP = 1,
        APPACT_LOGOFF,
        APPACT_APP_SETTINGS,
        APPACT_GUEST_NEW,
        APPACT_GUEST_SELECT,
        APPACT_GUEST_RELEASE,
        APPACT_GUEST_EDIT,
        APPACT_DEVICE_USECARD,
        APPACT_DEVICE_USETIME,
        APPACT_DEVICE_PAYCASH,
        APPACT_DEVICE_START,
        APPACT_DEVICE_SKIP,
        APPACT_DEVICE_CLEAN,
        APPACT_DEVICE_SETTINGS,
        APPACT_DEVICE_STOP,
        APPACT_SELL_CARD,
        APPACT_SELL_PRODUCT,
        APPACT_CASSA_SHOW,
        APPACT_CASSA_STORNO,
        APPACT_CASSA_CASHIN,
        APPACT_CASSA_CASHOUT,
        APPACT_CART
    };

    cDBApplicationAction();
    ~cDBApplicationAction();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const QString &p_stName )                 throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    unsigned int    id() const                                      throw();
    unsigned int    licenceId() const                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId ) throw();
    QString         name() const                                    throw();
    void            setName( const QString &p_stName )              throw();
    QString         modified() const                                                throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_bActive )               throw();
    QString         archive() const                                 throw();
    void            setArchive( const QString &p_stArchive )        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_stName;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_stName = "",
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
