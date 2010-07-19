#ifndef _sql_result_h_
#define _sql_result_h_

#include <QVector>
#include <QHash>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QByteArray>
#include <QVariant>
#include <QAbstractTableModel>
#include <QTextStream>


class SqlResult : public QAbstractTableModel {
    Q_OBJECT

public:
    SqlResult(QObject *parent = 0);
    virtual ~SqlResult();
    bool copy(QSqlQuery*);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);

    void setValid(bool v) { _valid = v; }
    bool isValid() const { return _valid; }
    QString toStringStream();
    bool fromStringStream(QString);

protected:
    typedef QVector<QHash<int, QVariant> > DataTable;

    bool _valid;
    QVector<QString> _headers;
    DataTable _data;
};


#endif
