
#include <QTextStream>
#include "sqlResult.h"



SqlResult::SqlResult(QObject *parent)
    : QAbstractTableModel(parent),
      _valid(false)
{
}



SqlResult::~SqlResult()
{
}



bool SqlResult::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation != Qt::Horizontal || section < 0 || columnCount() <= section || role!=Qt::DisplayRole)
        return false;

    if (_headers.size() <= section)
        _headers.resize(qMax(section + 1, 16));
    _headers[section] = value.toString();

    return true;
}



QVariant SqlResult::headerData( int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        QVariant val = _headers.value(section);
        if (val.isValid())
            return val;
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}



int SqlResult::rowCount(const QModelIndex & parent) const
{
    return _data.size();
}



int SqlResult::columnCount(const QModelIndex & parent) const
{
    return _headers.size();
}



QVariant SqlResult::data(const QModelIndex & index, int role) const
{
    if ( role==Qt::DisplayRole && index.row()>=0 && index.row()<rowCount() && index.column()>=0 && index.column()<columnCount() )
        return _data.value(index.row()).value(index.column());
    return QVariant();
}



bool SqlResult::copy(QSqlQuery *q) {
    if (!q)
        return false;

    _headers.clear();
    _data.clear();
    if ( q->isActive() ) {

        // copy fieldnames
        _headers.resize(q->record().count());
        for(int i=0; i<q->record().count(); ++i) {
            _headers[i] = q->record().fieldName(i);
        }

        // copy data
        _data.resize(q->size());
        bool hasNext = q->first();
        int i = 0;
        while (hasNext) {
            // copy record fields
            for (i=0; i<q->record().count(); ++i)
                _data[q->at()][i] = q->value(i);
            hasNext = q->next();
        }
    }

    return true;
}



const char* SqlResult::getCSV()
{
    if (_headers.size()<0)
        return "";

    int i,j;
    QString s;
    QTextStream result(&s);

    for (i=0; i<_headers.size(); ++i) {
        if (i>0)
            result << ',';
        result << _headers.value(i);
    }

    for (i=0; i<_data.size(); ++i) {
        result << "\n";
        for (j=0; j<_data.value(i).size(); ++j) {
            printf("%s ", _data.value(i).value(j).typeName() );
            if (j>0) result << ',';
            if ( _data.value(i).value(j).type()==QVariant::String ) {
                QString str = _data.value(i).value(j).toString();
                str.replace('"', "\\\"");
                result << '"' << str << '"';
            } else
                result << _data.value(i).value(j).toString();
        }
    }

    return result.readAll().toStdString().c_str();
}
