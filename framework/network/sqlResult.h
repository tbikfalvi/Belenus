#ifndef _sql_result_h_
#define _sql_result_h_

#include <QVector>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QByteArray>
#include <QVariant>
#include <QAbstractTableModel>


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

    bool isValid() const { return _valid; }
    const char* getCSV();

protected:
    typedef QVector<QHash<int, QVariant> > DataTable;

    bool _valid;
    QVector<QString> _headers;
    DataTable _data;
};


#endif
