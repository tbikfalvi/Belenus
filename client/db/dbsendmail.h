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
    QString         variables() const                                   throw();
    void            setVariables( const QString &p_qsVariables )        throw();
    QString         dateSend() const                                    throw();
    void            setDateSend( const QString &p_qsDateSend )          throw();
    QString         recipients() const                                  throw();
    void            setRecipients( const QString &p_qsRecipients )      throw();
    QString         subject() const                                     throw();
    void            setSubject( const QString &p_qsSubject )            throw();
    QString         message() const                                     throw();
    void            setMessage( const QString &p_qsMessage )            throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    QString         archive() const                                     throw();
    void            setArchive( const QString &p_stArchive )            throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiMailTypeId;
    QString         m_qsVariables;
    QString         m_qsDateSend;
    QString         m_qsRecipients;
    QString         m_qsSubject;
    QString         m_qsMessage;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               unsigned int p_uiMailTypeId = 0,
               QString p_qsVariables = "",
               QString p_qsDateSend = "",
               QString p_qsRecipients = "",
               QString p_qsSubject = "",
               QString p_qsMessage = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                 throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
