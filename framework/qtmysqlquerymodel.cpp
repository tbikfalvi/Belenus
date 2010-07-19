#include "qtmysqlquerymodel.h"
#include "qtframework.h"

cQTMySQLQueryModel::cQTMySQLQueryModel( QObject *p_poParent )
    : QSqlQueryModel( p_poParent )
 {
 }

cQTMySQLQueryModel::~cQTMySQLQueryModel()
{
}

void cQTMySQLQueryModel::setQuery( const QSqlQuery &p_obQuery )
{
    g_obLogger(cSeverity::DEBUG) << p_obQuery.lastQuery() << cQTLogger::EOM;

    QSqlQueryModel::setQuery( p_obQuery );

    if( lastError().type() != QSqlError::NoError )
    {
        g_obLogger(cSeverity::ERROR) << "Database error: " << lastError().text() << cQTLogger::EOM;
    }
}

void cQTMySQLQueryModel::setQuery( const QString &p_qsQuery, const QSqlDatabase &p_obDB )
{
    g_obLogger(cSeverity::DEBUG) << p_qsQuery << cQTLogger::EOM;

    QSqlQueryModel::setQuery( p_qsQuery, p_obDB );

    if( lastError().type() != QSqlError::NoError )
    {
        g_obLogger(cSeverity::ERROR) << "Database error: " << lastError().text() << cQTLogger::EOM;
    }
}
