#ifndef DBWRITER_H
#define DBWRITER_H

#include "../qtlogger.h"
#include "../qtmysqlconnection.h"


class DatabaseWriter : public LogWriter {
public:
    DatabaseWriter(const cSeverity::teSeverity minSev = cSeverity::WARNING) : LogWriter(minSev), m_uiAppUser(0), m_poDB(0) {}
    void setAppUser( const unsigned int p_uiUser ) { m_uiAppUser = p_uiUser; }
    void setDBConnection(cQTMySQLConnection * db) { m_poDB = db; }

protected:
    virtual void _writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m);

    unsigned int           m_uiAppUser;
    cQTMySQLConnection    *m_poDB;
};

#endif

