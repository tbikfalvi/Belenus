#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>
#include <QTextStream>
#include <QMutex>
#include "../qtlogger.h"



class FileWriter : public LogWriter {
public:
    FileWriter(const QString filename, const cSeverity::teSeverity sev = cSeverity::WARNING);
    void setFilename(const QString filename);

protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m, const void * threadId);

    bool _enabled;
    QMutex _mutex;
    QString _filename;
};


#endif

