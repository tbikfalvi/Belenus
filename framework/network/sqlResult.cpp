
#include <QTextStream>
#include <QStringList>
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
    if (orientation == Qt::Horizontal && section<=_headers.size() && role==Qt:: DisplayRole) {
        QVariant val = _headers.value(section);
        if (val.isValid())
            return val;
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}



int SqlResult::rowCount(const QModelIndex & /* parent */) const
{
    return _data.size();
}



int SqlResult::columnCount(const QModelIndex & /* parent */ ) const
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

    if ( q->lastError().isValid() ) {
        _valid = false;
        return false;
    }
    _valid = true;

    if ( q->isActive() && q->isSelect() ) {

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



QString SqlResult::toStringStream()
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
            if (j>0) result << ',';
            if ( _data.value(i).value(j).type()!=QVariant::Int &&
                 _data.value(i).value(j).type()!=QVariant::UInt &&
                 _data.value(i).value(j).type()!=QVariant::Double ) {
                QString str = _data.value(i).value(j).toString();
                str.replace('"', "\\\"").replace("\n", "\\\n");
                result << '"' << str << '"';
            } else
                result << _data.value(i).value(j).toString();
        }
    }

    return s;
}



bool SqlResult::fromStringStream(QString str)
{
    QTextStream stream(&str, QIODevice::ReadOnly);

    QString headLine = stream.readLine();
    if ( headLine.isEmpty() )
        return false;

    // read headers
    _headers.clear();
    QStringList fields = headLine.split(',', QString::KeepEmptyParts);
    _headers.reserve(fields.size());
    QStringList::const_iterator it;
    for ( it=fields.constBegin(); it!=fields.end(); ++it )
        _headers.push_back((*it).trimmed());

    // read data
    _data.clear();
    int startPos = -1;
    int lc = 0;

    while ( !stream.atEnd() ) {

        int pos = 0;
        int field = 0;

        QString line = stream.readLine();

        _data.resize(lc+1);
        _data[lc].reserve(_headers.size()+1);

        // here we need to parse the lines for values
        // first, startpos should be min(pos(,),pos(")). If this is the first elemetn, pos(,)=0
        // then if at startpos there is ", search for a " which does not have \ before
        //      otherwise search for a ,
        // that will be end pos. Between startpos-endpos there is our value. trim it, and if starts and ends with "
        // remove them and replace all \" with "
        bool inQuote = false;
        for(pos=0; pos<=line.length(); ++pos) {

            if ( pos<line.length() ) {
                if ( startPos<0 && (line.at(pos)==' ' || line.at(pos)=='\t') )
                    continue;

                if ( startPos<0 ) {
                    startPos = pos;
                    if ( line.at(pos)=='"' )
                        inQuote = true;
                    continue;
                }


                if ( inQuote && line.at(pos)=='"' && line.at(pos-1)!='\\' ) {
                    inQuote=false;
                    continue;
                }
            }

            // pos == line.length means we are after the string. This method prevents me to duplicate the following code after the FOR
            if ( pos==line.length() || (!inQuote && line.at(pos)==',')  ) {
                int e = pos, s = startPos;
                if ( e>=line.length() )
                    --e;

                if ( line.at(startPos)=='"' )
                    ++s;
                if ( line.at(e)==',' )
                    --e;
                if ( line.at(startPos)=='"' && !inQuote ) {
                    while (line.at(e)!='"' && e>s)
                        --e;
                    if ( line.at(e)=='"' )
                        --e;
                }

                QString res = line.mid(s, e-s+1);
                // replace \" and \n
                if ( line.at(startPos)=='"' )
                    res.replace("\\\"", "\"").replace("\\\n", "\n");
                else
                    res = res.trimmed();

                // store value
                _data[lc][field] = res;
                ++field;

                // reinit for new element
                startPos = -1;
                inQuote = false;
            }
        }

        ++lc;
    }

    return true;
}
