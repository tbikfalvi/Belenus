#ifndef CQTMYSQLQUERYMODEL_H
#define CQTMYSQLQUERYMODEL_H

#include <QSqlQueryModel>

class cQTMySQLQueryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    cQTMySQLQueryModel( QObject   *p_poParent = 0 );
    virtual ~cQTMySQLQueryModel();

    virtual void setQuery( const QSqlQuery &p_obQuery );
    virtual void setQuery( const QString &p_qsQuery, const QSqlDatabase &p_obDB = QSqlDatabase() );
};

#endif
