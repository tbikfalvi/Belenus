#ifndef TRACER_H
#define TRACER_H

#include <string>
#include <sstream>

using namespace std;

class cTracer
{
public:
    static unsigned int m_uiIndent;
    
    cTracer( const string &p_stFuncName, const string &p_stInParams = "" );
    ~cTracer();
    
    cTracer &operator <<( int p_inParam ) {
        m_ssOutParams << p_inParam;
        return *this;
    }
    cTracer &operator <<( string p_stParam ) {
        m_ssOutParams << p_stParam;
        return *this;
    }
    
private:
    string        m_stFuncName;
    stringstream  m_ssOutParams;
};

#endif
