#ifndef DB_PRODUCTACTIONTYPE_H
#define DB_PRODUCTACTIONTYPE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBProductActionType
{
public:
    cDBProductActionType();
    ~cDBProductActionType();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            load( const QString &p_qsName )                             throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_nLicenceId )             throw();
    QString         name() const                                                throw();
    void            setName( const QString &p_qsName )                          throw();
    bool            increaseProductCount() const                                throw();
    void            setIncreaseProductCount( const bool p_bIncrease )           throw();
    bool            decreaseProductCount() const                                throw();
    void            setDecreaseProductCount( const bool p_bDecrease )           throw();
    QString         cassaActionIndication() const                               throw();
    void            setCassaActionIndication( const QString &p_qsIndication )   throw();
    QString         modified() const                                            throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    QString         archive() const                                             throw();
    void            setArchive( const QString &p_qsArchive )                    throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    bool            m_bIncreaseProductCount;
    bool            m_bDecreaseProductCount;
    QString         m_qsCassaActionIndication;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const bool p_bIncrease = false,
               const bool p_bDecrease = false,
               const QString &p_qsIndication = "",
               const QString &p_qsModified = QObject::tr("Positive"),
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                     throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
