
#include <QDate>
#include <QTime>
#include "FileWriter.h"



FileWriter::FileWriter(const QString filename, const cSeverity::teSeverity sev)
    : LogWriter(sev),
      _enabled(false),
      _filename("")
{
    setFilename(filename);
}



void FileWriter::_writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m, const void * threadId) {
    if ( !_enabled )
        return;

    _mutex.lock();
    QFile f(_filename);
    if ( !f.open(QIODevice::WriteOnly | QIODevice::Append) ) {
        _enabled = false;
        _mutex.unlock();
        return;
    }

    char s = '-';
    switch(sev) {
        case cSeverity::DEBUG: s = 'D'; break;
        case cSeverity::ERROR: s = 'E'; break;
        case cSeverity::INFO: s = 'I'; break;
        case cSeverity::WARNING: s = 'W'; break;
        default: s = '-';
    }

    QTextStream stream(&f);
    stream << ts.toString(Qt::ISODate) << " [" << threadId << "] " << s << ' ' << m << "\n";
    f.close();

    _mutex.unlock();
}



void FileWriter::setFilename(const QString file)
{
    _filename = file.arg( QDate::currentDate().toString("yyyyMMdd") ).arg( QTime::currentTime().toString("hhmm") );
    _enabled = true;
}