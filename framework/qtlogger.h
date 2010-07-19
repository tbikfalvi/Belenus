#ifndef QTLOGGER_H
#define QTLOGGER_H

#include <QDateTime>
#include <QTextStream>
#include <QString>
#include <QMap>

#include "sevexception.h"
#include "dbconnection.h"


class LogMessage;



class LogWriter {
    friend class cQTLogger;

public:
    LogWriter(const cSeverity::teSeverity minSeverity) : _minSeverity(minSeverity) {}
    virtual void setMinimumSeverity(const cSeverity::teSeverity minSeverity) { _minSeverity = minSeverity; }

protected:
    virtual void _write(const cSeverity::teSeverity sev, const QDateTime ts, const QString &msg)    { if (sev<=_minSeverity) _writeLog(sev,ts,msg); }
    virtual void _writeLog(const cSeverity::teSeverity severity, const QDateTime ts, const QString &msg) = 0;
    cSeverity::teSeverity _minSeverity;
};





class cQTLogger
{

public:

    enum teLoggerManip
    {
        MIN = 0,
        EOM,
        CLEAR,
        MAX
    };

    cQTLogger();
    ~cQTLogger();

    void         setAppUser( const unsigned int p_uiUser );
    unsigned int getAppUser( void ) const;

    void logMessage( const cSeverity::teSeverity p_enLevel, const QString &p_stMessage );

    void attachWriter( const QString name, LogWriter* );
    void detachWriter( const QString name );
    void setMinimumSeverity( const QString name, const cSeverity::teSeverity sev );

    LogMessage operator () ( const cSeverity::teSeverity p_enLevel );

private:
    typedef QMap<QString, LogWriter*> Writers;

    Writers                m_writers;
    unsigned int           m_uiAppUser;
};






class LogMessage {
public:
    LogMessage(const cSeverity::teSeverity sev, cQTLogger *logger);
    LogMessage(const LogMessage &lm);
    virtual ~LogMessage();

    LogMessage &operator <<( const cSeverity::teSeverity p_enSev );
    LogMessage &operator <<( const int p_inParam );
    LogMessage &operator <<( const QString &p_inParam );
    LogMessage &operator <<( const cQTLogger::teLoggerManip p_enManip );


protected:
    cQTLogger             *m_logger;
    cSeverity::teSeverity  m_enNextSeverityLevel;
    QString                m_string;
    QTextStream            m_ssMessage;
};







class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter(const cSeverity::teSeverity sev) : LogWriter(sev) {}
protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m);
};

#endif

