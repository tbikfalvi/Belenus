#include "qtframework.h"
#include "qtmysqlconnection.h"

cQTMySQLConnection::cQTMySQLConnection()
{
    m_poDB = new QSqlDatabase( QSqlDatabase::addDatabase( "QMYSQL" ) );
}

cQTMySQLConnection::~cQTMySQLConnection()
{
    delete m_poDB;
}

void cQTMySQLConnection::open( void ) throw( cSevException )
{
    if( isOpen() ) close();

    if( m_poDB->open() )
    {
        m_boOpen = true;
    }
    else
    {
        throw cSevException( cSeverity::ERROR,
                "Error opening database: " + m_poDB->lastError().text().toStdString() );
    }
}

void cQTMySQLConnection::close( void ) throw( cSevException )
{
    if( isOpen() )
    {
        m_poDB->close();
        m_boOpen = false;
    }
}

void cQTMySQLConnection::executeQuery( const string &p_stQuery,
                                       const bool p_boLog )
                         throw( cSevException )
{
    if( !isOpen() )
        throw cSevException( cSeverity::ERROR,
                "Error executing query: Database is not open" );

    if( p_boLog )
    {
        g_obLogger(cSeverity::DEBUG) << "[cQTMySQLConnection::executeQuery] " << p_stQuery.c_str() << EOM;
    }

    QSqlQuery  obQuery;
    bool triedToReConnect = false;
    for(;;) {
        if( !obQuery.exec( QString::fromStdString( p_stQuery ) ) )
        {
            // in case server dropped connection because of idle time, try to reconnect and re-exec the query
            // if second attempt does not work, does not try again. Open() also will throw exception if error occurred
            if ( !triedToReConnect && (obQuery.lastError().number()==CR_SERVER_GONE_ERROR || obQuery.lastError().number()==CR_SERVER_LOST) ) {
                g_obLogger(cSeverity::DEBUG) << "[cQTMySQLConnection::executeQuery] server has gone. Trying to reconnect..." << EOM;
                m_boOpen = false;
                open();
                triedToReConnect = true;  // try to reconnect only once
                continue;
            }

            throw cSevException( cSeverity::ERROR, "Error executing query: " + obQuery.lastError().text().toStdString() );
        }
        break;  // if we are here, status is ok. So quit the FOR
    }
}

QSqlQuery* cQTMySQLConnection::executeQTQuery( const QString &p_qsQuery )
                               throw( cSevException )
{
    g_obLogger(cSeverity::DEBUG) << p_qsQuery << EOM;
    if( !isOpen() )
        throw cSevException( cSeverity::ERROR,
                "Error executing query: Database is not open" );

    QSqlQuery*  poQuery = new QSqlQuery;
    bool triedToReConnect = false;
    for(;;) {
        if( !poQuery->exec( p_qsQuery ) )
        {
            QString qsError = poQuery->lastError().text();
            // in case server dropped connection because of idle time, try to reconnect and re-exec the query
            // if second attempt does not work, does not try again. Open() also will throw exception if error occurred
            if ( !triedToReConnect && (poQuery->lastError().number()==CR_SERVER_GONE_ERROR || poQuery->lastError().number()==CR_SERVER_LOST) ) {
                g_obLogger(cSeverity::DEBUG) << "[cQTMySQLConnection::executeQuery] server has gone. Trying to reconnect..." << EOM;
                m_boOpen = false;
                open();
                triedToReConnect = true;  // try to reconnect only once
                continue;
            }

            delete poQuery;
            throw cSevException( cSeverity::ERROR, "Error executing query: " + qsError.toStdString() );
        }
        break;  // if we are here, status is ok. So quit the FOR
    }

    return poQuery;
}

void cQTMySQLConnection::setHostName( const QString &p_qsHost ) throw()
/* This function sets the Host Name for the MySQL database access.
 * If the database is already open, it will be closed first. */
{
    if( getHostName() != p_qsHost )
    {
        if( isOpen() ) close();
        m_poDB->setHostName( p_qsHost );
    }
}

QString cQTMySQLConnection::getHostName( void ) throw()
{
    return m_poDB->hostName();
}

void cQTMySQLConnection::setDatabaseName( const QString &p_qsDB ) throw()
/* This function sets the Database Name for the MySQL database access.
 * If the database is already open, it will be closed first. */
{
    if( getDatabaseName() != p_qsDB )
    {
        if( isOpen() ) close();
        m_poDB->setDatabaseName( p_qsDB );
    }
}

QString cQTMySQLConnection::getDatabaseName( void ) throw()
{
    return m_poDB->databaseName();
}

void cQTMySQLConnection::setUserName( const QString &p_qsUser ) throw()
/* This function sets the User Name for the MySQL database access.
 * If the database is already open, it will be closed first. */
{
    if( getUserName() != p_qsUser )
    {
        if( isOpen() ) close();
        m_poDB->setUserName( p_qsUser );
    }
}

QString cQTMySQLConnection::getUserName( void ) throw()
{
    return m_poDB->userName();
}

void cQTMySQLConnection::setPassword( const QString &p_qsPwd ) throw()
/* This function sets the Password for the MySQL database access.
 * If the database is already open, it will be closed first. */
{
    if( getPassword() != p_qsPwd )
    {
        if( isOpen() ) close();
        m_poDB->setPassword( p_qsPwd );
    }
}

QString cQTMySQLConnection::getPassword( void ) throw()
{
    return m_poDB->password();
}
