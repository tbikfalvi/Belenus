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
        g_obLogger(cSeverity::DEBUG) << "[cQTMySQLConnection::executeQuery] " << p_stQuery.c_str() << cQTLogger::EOM;
    }

    QSqlQuery  obQuery;
    if( !obQuery.exec( QString::fromStdString( p_stQuery ) ) )
    {
        throw cSevException( cSeverity::ERROR,
                "Error executing query: " + obQuery.lastError().text().toStdString() );
    }
}

QSqlQuery* cQTMySQLConnection::executeQTQuery( const QString &p_qsQuery )
                               throw( cSevException )
{
    g_obLogger(cSeverity::DEBUG) << p_qsQuery << cQTLogger::EOM;

    if( !isOpen() )
        throw cSevException( cSeverity::ERROR,
                "Error executing query: Database is not open" );

    QSqlQuery*  poQuery = new QSqlQuery;
    if( !poQuery->exec( p_qsQuery ) )
    {
        QString  qsError = poQuery->lastError().text();
        delete poQuery;

        throw cSevException( cSeverity::ERROR,
                "Error executing query: " + qsError.toStdString() );
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
