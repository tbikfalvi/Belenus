#include <QDateTime>
#include <iostream>
#include "qtlogger.h"



void ConsoleWriter::_writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m) {
    cerr << ts.toString(Qt::ISODate).toStdString() << " ";
    cerr << cSeverity::toStr( sev );
    if( !m.isEmpty() )
        cerr << " " << m.toStdString();
    cerr << endl << flush;
}



LogMessage::LogMessage(cSeverity::teSeverity sev, cQTLogger *logger)
    : m_logger(logger),
      m_enNextSeverityLevel(sev),
      m_string(""),
      m_ssMessage(&m_string)
{
}


LogMessage::LogMessage(const LogMessage &lm)
    : m_logger(lm.m_logger),
      m_enNextSeverityLevel(lm.m_enNextSeverityLevel),
      m_string(lm.m_string),
      m_ssMessage(&m_string)
{
}



LogMessage::~LogMessage() {
    if ( !m_string.isEmpty() && m_logger )
        m_logger->logMessage( m_enNextSeverityLevel, m_string );
}



LogMessage &LogMessage::operator <<( const int p_inParam ) {
    m_ssMessage << p_inParam;
    return *this;
}



LogMessage &LogMessage::operator <<( const QString &p_inParam ) {
    m_ssMessage << p_inParam;
    return *this;
}



LogMessage &LogMessage::operator <<( const cQTLogger::teLoggerManip p_enManip ) {
    switch( p_enManip )
    {
        case cQTLogger::EOM:
            if ( m_logger )
                m_logger->logMessage( m_enNextSeverityLevel, m_string );
            // There's no 'break' here because the EOM manipulator
            // needs to do a 'CLEAR' as well
        case cQTLogger::CLEAR:
            m_string = "";
            m_ssMessage.setString(&m_string);
            break;
        default: ;
    }
    return *this;
}






cQTLogger::cQTLogger()
    : m_uiAppUser(0)
{
}



cQTLogger::~cQTLogger()
{
}



void cQTLogger::setAppUser( const unsigned int p_uiUser )
{
    m_uiAppUser = p_uiUser;
}



unsigned int cQTLogger::getAppUser( void ) const
{
    return m_uiAppUser;
}



void cQTLogger::logMessage( const cSeverity::teSeverity  p_enLevel,
                            const QString               &p_stMessage )
{
    try
    {
        QDateTime curr = QDateTime::currentDateTime();
        Writers::const_iterator it;
        for (it = m_writers.begin(); it!=m_writers.end(); ++it)
            if ( (*it) )
                (*it)->_write(p_enLevel, curr, p_stMessage);
    }
    catch( cSevException &e )
    {
        // i dont knwo yet what to do
    }
}



void cQTLogger::attachWriter(const QString name, LogWriter *w)
{
    m_writers.insert(name, w);
}



void cQTLogger::detachWriter(const QString name)
{
    Writers::iterator it = m_writers.find(name);
    if ( it!=m_writers.end() )
        m_writers.erase(it);
}



void cQTLogger::setMinimumSeverity(const QString name, const cSeverity::teSeverity sev)
{
    Writers::iterator it = m_writers.find(name);
    if ( it!=m_writers.end() )
        (*it)->setMinimumSeverity(sev);
}



LogMessage cQTLogger::operator () ( const cSeverity::teSeverity p_enLevel ) {
    return LogMessage(p_enLevel, this);
}


/*

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
*/