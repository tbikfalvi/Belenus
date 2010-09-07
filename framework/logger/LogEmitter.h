#ifndef _LOG_EMITTER_H
#define _LOG_EMITTER_H


#include <QObject>
#include <QString>
#include "../qtlogger.h"



class LogEmitter : public QObject, public LogWriter {
    Q_OBJECT

public:
    LogEmitter(const cSeverity::teSeverity sev) : QObject(), LogWriter(sev) {}

signals:
    void logMessage(QString);

protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &msg, const void * threadId) {
        emit logMessage(QString("[%1] %2").arg(ts.toString("hh:mm:ss.zzz")).arg(msg));
    }
};



#endif