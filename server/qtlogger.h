#ifndef QTLOGGER_H
#define QTLOGGER_H

#include <string>
#include <QString>
#include <QTextStream>
#include "../framework/sevexception.h"

using namespace std;

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

    void setMinSeverityLevels( const cSeverity::teSeverity  p_enConsoleLevel ) throw();
    void getMinSeverityLevels( cSeverity::teSeverity *p_poConsoleLevel ) const throw();
    void         setAppUser( const unsigned int p_uiUser ) throw();
    unsigned int getAppUser( void )  const                 throw();
    void logMessage( const cSeverity::teSeverity  p_enLevel, const QString &p_stMessage )  throw();
    void logMessage( const cSeverity::teSeverity  p_enLevel, const string &p_stMessage )  throw();


    cQTLogger &operator ()( const cSeverity::teSeverity p_enSev ) {
        m_enNextSeverityLevel = p_enSev;
        return *this;
    }

    cQTLogger &operator <<( const int p_inParam ) {
        m_ssMessage << p_inParam;
        return *this;
    }


    cQTLogger &operator <<( const string p_stParam ) {
        m_ssMessage << p_stParam.c_str();
        return *this;
    }


    cQTLogger &operator <<( const cSeverity::teSeverity p_enSev ) {
        m_enNextSeverityLevel = p_enSev;
        return *this;
    }

    cQTLogger &operator <<( const teLoggerManip p_enManip ) {
        switch( p_enManip )
        {
            case EOM:
                logMessage( m_enNextSeverityLevel, *m_ssMessage.string() );
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
    cSeverity::teSeverity  m_enMinConsoleSeverityLevel;
    cSeverity::teSeverity  m_enNextSeverityLevel;
    int                    m_uiAppUser;
    QTextStream            m_ssMessage;
    QString                m_string;

    void init( void ) throw ();
    void logToConsole( const cSeverity::teSeverity  p_enLevel,
                       const QString               &p_stMessage )
            throw();
};

#endif

