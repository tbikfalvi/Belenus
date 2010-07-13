#ifndef QTLOGGER_H
#define QTLOGGER_H


#include <QTextStream>
#include <QString>
#include <QMap>

#include "sevexception.h"
#include "dbconnection.h"





class LogWriter {
    friend class cQTLogger;

public:
    LogWriter(const cSeverity::teSeverity minSeverity) : _minSeverity(minSeverity) {}
    virtual void setMinimumSeverity(const cSeverity::teSeverity minSeverity) { _minSeverity = minSeverity; }

protected:
    virtual void _write(const cSeverity::teSeverity sev, const QString &msg)    { if (sev<=_minSeverity) _writeLog(sev,msg); }
    virtual void _writeLog(const cSeverity::teSeverity severity, const QString &msg) = 0;
    cSeverity::teSeverity _minSeverity;
};






class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter(const cSeverity::teSeverity sev) : LogWriter(sev) {}
protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QString &m);
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


    /*
     * Operators
     */

    cQTLogger &operator ()( const cSeverity::teSeverity p_enSev ) {
        m_enNextSeverityLevel = p_enSev;
        return *this;
    }

    cQTLogger &operator <<( const cSeverity::teSeverity p_enSev ) {
        m_enNextSeverityLevel = p_enSev;
        return *this;
    }


    cQTLogger &operator <<( const int p_inParam ) {
        m_ssMessage << p_inParam;
        return *this;
    }



    cQTLogger &operator <<( const QString &p_inParam ) {
        m_ssMessage << p_inParam;
        return *this;
    }


    cQTLogger &operator <<( const teLoggerManip p_enManip ) {
        switch( p_enManip )
        {
            case EOM:
                logMessage( m_enNextSeverityLevel, m_string );
                // There's no 'break' here because the EOM manipulator
                // needs to do a 'CLEAR' as well
            case CLEAR:
                m_enNextSeverityLevel = cSeverity::DEBUG;
                m_string = "";
                m_ssMessage.setString(&m_string);
                break;
            default: ;
        }
        return *this;
    }




private:
    typedef QMap<QString, LogWriter*> Writers;

    Writers                m_writers;
    cSeverity::teSeverity  m_enNextSeverityLevel;
    QString                m_string;
    QTextStream            m_ssMessage;
    unsigned int           m_uiAppUser;
};

#endif

