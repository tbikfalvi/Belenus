#ifndef CQTMYSQLCONNECTION_H
#define CQTMYSQLCONNECTION_H

#include <QtSql>
#include <QString>

#include "dbconnection.h"

class cQTMySQLConnection : public cDBConnection
{
public:
    cQTMySQLConnection();
    virtual ~cQTMySQLConnection();

    virtual void open( void )                                     throw( cSevException );
    virtual void close( void )                                    throw( cSevException );
    virtual void executeQuery( const string &p_stQuery,
                               const bool p_boLog = false )       throw( cSevException );
    virtual QSqlQuery* executeQTQuery( const QString &p_qsQuery ) throw( cSevException );

    void setHostName( const QString &p_qsHost )                   throw();
    QString  getHostName( void )                                  throw();
    void setDatabaseName( const QString &p_qsDB )                 throw();
    QString  getDatabaseName( void )                              throw();
    void setUserName( const QString &p_qsUser )                   throw();
    QString getUserName( void )                                   throw();
    void setPassword( const QString &p_qsPwd )                    throw();
    QString getPassword( void )                                   throw();

private:
    QSqlDatabase *m_poDB;
};

#endif
