#ifndef DB_SENDMAIL_H
#define DB_SENDMAIL_H

#include <QSqlRecord>
#include <QString>

#include "../../framework/sevexception.h"

class cDBSendMail
{
public:
    cDBSendMail();
    ~cDBSendMail();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    unsigned int    mailTypeId() const                                  throw();
    void            setMailTypeId( const unsigned int p_nMailTypeId )   throw();
    QString         dateSend() const                                    throw();
    void            setDateSend( const QString &p_qsDateSend )          throw();
    QString         recipients() const                                  throw();
    void            setRecipients( const QString &p_qsRecipients )      throw();
    QString         subject() const                                     throw();
    void            setSubject( const QString &p_qsSubject )            throw();
    QString         message() const                                     throw();
    void            setMessage( const QString &p_qsMessage )            throw();
    QString         varName() const                                     throw();
    void            setVarName( const QString &p_qsVarName )            throw();
    QString         varBardcode() const                                 throw();
    void            setVarBardcode( const QString &p_qsVarBardcode )    throw();
    QString         varCardInfo() const                                 throw();
    void            setVarCardInfo( const QString &p_qsVarCardInfo )    throw();
    QString         varUnitCount() const                                throw();
    void            setVarUnitCount( const QString &p_qsVarUnitCount )  throw();
    QString         varDateTime() const                                 throw();
    void            setVarDateTime( const QString &p_qsVarDateTime )    throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_stArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiMailTypeId;
    QString         m_qsDateSend;
    QString         m_qsRecipients;
    QString         m_qsSubject;
    QString         m_qsMessage;
    QString         m_qsVarName;
    QString         m_qsVarBarcode;
    QString         m_qsVarCardInfo;
    QString         m_qsVarUnitCount;
    QString         m_qsVarDateTime;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               unsigned int p_uiMailTypeId = 0,
               QString p_qsDateSend = "",
               QString p_qsRecipients = "",
               QString p_qsSubject = "",
               QString p_qsMessage = "",
               QString p_qsVarName = "",
               QString p_qsVarBarcode = "",
               QString p_qsVarCardInfo = "",
               QString p_qsVarUnitCount = "",
               QString p_qsVarDateTime = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                 throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
