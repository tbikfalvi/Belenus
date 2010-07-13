#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "qtlogger.h"

cQTLogger::cQTLogger()
    : m_ssMessage(&m_string),
      m_string("")
{
    init();
}



cQTLogger::~cQTLogger()
{
}



void cQTLogger::init( void ) throw ()
{
    m_enMinConsoleSeverityLevel = cSeverity::NONE;
    m_enNextSeverityLevel       = cSeverity::NONE;
    m_uiAppUser = 0;
}



void cQTLogger::setMinSeverityLevels(
    const cSeverity::teSeverity p_enConsoleLevel = cSeverity::DEBUG )
    throw()
{
    m_enMinConsoleSeverityLevel = p_enConsoleLevel;
}



void cQTLogger::getMinSeverityLevels(
    cSeverity::teSeverity *p_poConsoleLevel ) const
    throw()
{
    *p_poConsoleLevel = m_enMinConsoleSeverityLevel;
}

void cQTLogger::setAppUser( const unsigned int p_uiUser )
     throw()
{
    m_uiAppUser = p_uiUser;
}

unsigned int cQTLogger::getAppUser( void ) const
       throw()
{
    return m_uiAppUser;
}



void cQTLogger::logMessage( const cSeverity::teSeverity  p_enLevel,
                            const string               &p_stMessage )
     throw()
{
    logMessage(p_enLevel, QString(p_stMessage.c_str()));
}



void cQTLogger::logMessage( const cSeverity::teSeverity  p_enLevel,
                            const QString               &p_stMessage )
     throw()
{
    try
    {
        if( p_enLevel <= m_enMinConsoleSeverityLevel )
            logToConsole( p_enLevel, p_stMessage );
    }
    catch( cSevException &e )
    {
        logToConsole( e.severity(), e.what() );
    }
}

void cQTLogger::logToConsole( const cSeverity::teSeverity  p_enLevel,
                              const QString               &p_stMessage )
     throw()
{
    time_t     ttTime;
    struct tm *poTm;
    time( &ttTime );
    poTm = localtime( &ttTime );

    cerr << setw(2) << setfill('0') << poTm->tm_mday << ".";
    cerr << setw(2) << setfill('0') << poTm->tm_mon + 1 << ".";
    cerr << poTm->tm_year + 1900 << " ";
    cerr << poTm->tm_hour << ":";
    cerr << setw(2) << setfill('0') << poTm->tm_min << ":";
    cerr << setw(2) << setfill('0') << poTm->tm_sec << " ";
    cerr << cSeverity::toStr( p_enLevel );
    if( m_uiAppUser ) cerr << " User: " << m_uiAppUser << " ";
    if( p_stMessage != "" ) cerr << " " << p_stMessage.toStdString();
    cerr << endl << flush;
}
