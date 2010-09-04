#include "DatabaseWriter.h"

void DatabaseWriter::_writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m)
{
    if( m_poDB && m_poDB->isOpen() )
    {
        try {
            QString  ssSql;
            ssSql = QString("INSERT INTO logs (userId, severity, message,date) VALUES('%1', '%4', '%2', '%3');").arg(m_uiAppUser).arg(m).arg(ts.toString("yyyy-MM-dd hh:mm:ss")).arg(sev);
            m_poDB->executeQTQuery( ssSql );
        } catch(cSevException &e)
        {
            // do nothing
            // cout << e.what() << endl;
        }
    }
}

