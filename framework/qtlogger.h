#ifndef QTLOGGER_H
#define QTLOGGER_H

#include <string>
#include <sstream>
#include <QString>

#include "sevexception.h"
#include "dbconnection.h"

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
    cQTLogger( cDBConnection *p_poDB );
    ~cQTLogger();

    void setDBConnection( cDBConnection * const p_poDB );

    void setMinSeverityLevels( const cSeverity::teSeverity  p_enConsoleLevel,
                               const cSeverity::teSeverity  p_enDBLevel,
                               const cSeverity::teSeverity  p_enGUILevel )
            throw();
    void getMinSeverityLevels( cSeverity::teSeverity *p_poConsoleLevel,
                               cSeverity::teSeverity *p_poDBLevel,
                               cSeverity::teSeverity *p_poGUILevel ) const
            throw();

    void         setAppUser( const unsigned int p_uiUser ) throw();
    unsigned int getAppUser( void )  const                 throw();

    void logMessage( const cSeverity::teSeverity  p_enLevel,
                     const string                &p_stMessage )
            throw();

    cQTLogger &operator <<( const int p_inParam ) {
        m_ssMessage << p_inParam;
        return *this;
    }
    cQTLogger &operator <<( const string p_stParam ) {
        m_ssMessage << p_stParam;
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
                logMessage( m_enNextSeverityLevel, m_ssMessage.str() );
                // There's no 'break' here because the EOM manipulator
                // needs to do a 'CLEAR' as well
            case CLEAR:
                m_enNextSeverityLevel = cSeverity::DEBUG;
                m_ssMessage.str( "" );
                break;
            default: ;
        }
        return *this;
    }

private:
    cDBConnection         *m_poDB;
    cSeverity::teSeverity  m_enMinConsoleSeverityLevel;
    cSeverity::teSeverity  m_enMinDBSeverityLevel;
    cSeverity::teSeverity  m_enMinGUISeverityLevel;
    cSeverity::teSeverity  m_enNextSeverityLevel;
    stringstream           m_ssMessage;
    unsigned int           m_uiAppUser;

    void init( void ) throw ();
    void logToConsole( const cSeverity::teSeverity  p_enLevel,
                       const string                &p_stMessage )
            throw();
    void logToDB( const cSeverity::teSeverity  p_enLevel,
                  const string                &p_stMessage )
            throw( cSevException );
    void logToGUI( const cSeverity::teSeverity  p_enLevel,
                   const string                &p_stMessage )
            throw();
};

#endif

