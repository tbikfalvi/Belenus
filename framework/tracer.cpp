#include "tracer.h"
#include "qtframework.h"

unsigned int cTracer::m_uiIndent = 0;

cTracer::cTracer( const string &p_stFuncName,
                  const string &p_stInParams )
        : m_stFuncName( p_stFuncName )
{
    string  stMessage( m_uiIndent++, ' ');
    stMessage += p_stFuncName;
    stMessage += "( ";
    stMessage += p_stInParams;
    stMessage += " ) >";

    g_obLogger.logMessage( cSeverity::DEBUG, stMessage );
}

cTracer::~cTracer()
{
    string stMessage( --m_uiIndent, ' ' );
    stMessage += m_stFuncName;
    stMessage += "( ";
    stMessage += m_ssOutParams.str();
    stMessage += " ) <";

    g_obLogger.logMessage( cSeverity::DEBUG, stMessage );
}
