#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include <QMutex>
#include "../qtlogger.h"



class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter(const cSeverity::teSeverity sev = cSeverity::WARNING) : LogWriter(sev) {}
protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m, const void * threadId);

    QMutex _coutLogGuard;
};


#endif

