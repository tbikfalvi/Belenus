#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <QMessageBox>

#include "qtlogger.h"

cQTLogger::cQTLogger()
{
    init();
}

cQTLogger::cQTLogger( cDBConnection *p_poDB )
{
    init();

    m_poDB = p_poDB;
}

cQTLogger::~cQTLogger()
{
}

void cQTLogger::init( void ) throw ()
{
    m_enMinConsoleSeverityLevel = cSeverity::NONE;
    m_enMinDBSeverityLevel      = cSeverity::NONE;
    m_enMinGUISeverityLevel     = cSeverity::NONE;
    m_enNextSeverityLevel       = cSeverity::NONE;
    m_uiAppUser                 = 0;
}

void cQTLogger::setDBConnection( cDBConnection * const p_poDB )
{
    m_poDB = p_poDB;
}

void cQTLogger::setMinSeverityLevels(
    const cSeverity::teSeverity p_enConsoleLevel = cSeverity::DEBUG,
    const cSeverity::teSeverity p_enDBLevel = cSeverity::DEBUG,
    const cSeverity::teSeverity p_enGUILevel = cSeverity::DEBUG )
    throw()
{
    m_enMinConsoleSeverityLevel = p_enConsoleLevel;
    m_enMinDBSeverityLevel      = p_enDBLevel;
    m_enMinGUISeverityLevel     = p_enGUILevel;
}

void cQTLogger::getMinSeverityLevels(
    cSeverity::teSeverity *p_poConsoleLevel,
    cSeverity::teSeverity *p_poDBLevel,
    cSeverity::teSeverity *p_poGUILevel ) const
    throw()
{
    *p_poConsoleLevel = m_enMinConsoleSeverityLevel;
    *p_poDBLevel      = m_enMinDBSeverityLevel;
    *p_poGUILevel     = m_enMinGUISeverityLevel;
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
                            const string                &p_stMessage )
     throw()
{
    try
    {
        if( p_enLevel <= m_enMinConsoleSeverityLevel )
            logToConsole( p_enLevel, p_stMessage );
        if( p_enLevel <= m_enMinDBSeverityLevel )
            logToDB( p_enLevel, p_stMessage );
        if( p_enLevel <= m_enMinGUISeverityLevel )
            logToGUI( p_enLevel, p_stMessage );
    }
    catch( cSevException &e )
    {
        logToConsole( e.severity(), e.what() );
    }
}

void cQTLogger::logToConsole( const cSeverity::teSeverity  p_enLevel,
                              const string                &p_stMessage )
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
    if( p_stMessage != "" ) cerr << " " << p_stMessage;
    cerr << endl << flush;
}

void cQTLogger::logToDB( const cSeverity::teSeverity  p_enLevel,
                         const string                &p_stMessage )
     throw( cSevException )
{
    if( m_poDB && m_poDB->isOpen() )
    {
        stringstream  ssSql;
        ssSql << "INSERT INTO logs ( ";
        if( m_uiAppUser ) ssSql << "`userId`, ";
        ssSql << "`severity`";
        if( p_stMessage != "" ) ssSql << ", `message`";
        ssSql << " ) VALUES ( ";
        if( m_uiAppUser ) ssSql << m_uiAppUser << ", ";
        ssSql << (int)p_enLevel;
        if( p_stMessage != "" ) ssSql << ", \"" << p_stMessage << "\"";
        ssSql << " )";

        m_poDB->executeQuery( ssSql.str() );
    }
}

void cQTLogger::logToGUI( const cSeverity::teSeverity  p_enLevel,
                        const string                &p_stMessage )
                    throw()
{
    QMessageBox::Icon  enIcon  = QMessageBox::NoIcon;
    QString            obTitle = "";
    switch( p_enLevel )
    {
        case cSeverity::ERROR:   enIcon  = QMessageBox::Critical;
                                 obTitle = "Error";
                                 break;
        case cSeverity::WARNING: enIcon  = QMessageBox::Warning;
                                 obTitle = "Warning";
                                 break;
        case cSeverity::INFO:    enIcon  = QMessageBox::Information;
                                 obTitle = "Information";
                                 break;
        case cSeverity::DEBUG:   enIcon  = QMessageBox::Information;
                                 obTitle = "Debug message";
                                 break;
        default:                 enIcon  = QMessageBox::NoIcon;
                                 obTitle = "Message";
    }

    QMessageBox obMsg( enIcon, obTitle,
                       QString::fromStdString( p_stMessage ),
                       QMessageBox::Ok );
    obMsg.exec();
}
