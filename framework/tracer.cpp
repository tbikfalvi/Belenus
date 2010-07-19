#include "tracer.h"
#include "qtframework.h"

unsigned int cTracer::m_uiIndent = 0;

cTracer::cTracer( const QString p_stFuncName,
                  const QString p_stInParams )
        : m_stFuncName( p_stFuncName ),
          m_string(""),
          m_ssOutParams(&m_string)
{
    QString stMessage( m_uiIndent++, ' ');
    stMessage += QString("%1( %2 ) >").arg(p_stFuncName).arg(p_stInParams);
    g_obLogger.logMessage( cSeverity::DEBUG, stMessage );
}



cTracer::~cTracer()
{
    QString stMessage( --m_uiIndent, ' ' );
    stMessage += QString("%1( %2 ) <").arg(m_stFuncName).arg(m_string);
    g_obLogger.logMessage( cSeverity::DEBUG, stMessage );
}
