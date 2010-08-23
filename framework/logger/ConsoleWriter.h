#ifndef CONSOLEWRITER_H
#define CONSOLEWRITER_H

#include "../qtlogger.h"



class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter(const cSeverity::teSeverity sev) : LogWriter(sev) {}
protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m);
};


#endif

