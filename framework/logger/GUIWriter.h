#ifndef GUIWRITER_H
#define GUIWRITER_H

#include "../qtlogger.h"


class GUIWriter : public LogWriter {
public:
    GUIWriter(const cSeverity::teSeverity minSev) : LogWriter(sev) {}
protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m);
};

#endif

