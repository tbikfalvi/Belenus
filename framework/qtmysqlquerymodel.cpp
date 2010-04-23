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
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << p_obQuery.lastQuery().toStdString() << cQTLogger::EOM;

    QSqlQueryModel::setQuery( p_obQuery );

    if( lastError().type() != QSqlError::NoError )
    {
        string  stError = "Database error: ";
        stError += lastError().text().toStdString();
        g_obLogger << cSeverity::ERROR;
        g_obLogger << stError << cQTLogger::EOM;
    }
}

void cQTMySQLQueryModel::setQuery( const QString &p_qsQuery, const QSqlDatabase &p_obDB )
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << p_qsQuery.toStdString() << cQTLogger::EOM;

    QSqlQueryModel::setQuery( p_qsQuery, p_obDB );

    if( lastError().type() != QSqlError::NoError )
    {
        string  stError = "Database error: ";
        stError += lastError().text().toStdString();
        g_obLogger << cSeverity::ERROR;
        g_obLogger << stError << cQTLogger::EOM;
    }
}
