#ifndef TRACER_H
#define TRACER_H

#include <QString>
#include <QTextStream>



class cTracer
{
public:
    static unsigned int m_uiIndent;

    cTracer( const QString p_stFuncName, const QString p_stInParams = "" );
    ~cTracer();

    cTracer &operator <<( const int p_inParam ) {
        m_ssOutParams << p_inParam << ' ';
        return *this;
    }
    cTracer &operator <<( const QString & p_stParam ) {
        m_ssOutParams << p_stParam << ' ';
        return *this;
    }

private:
    QString      m_stFuncName;
    QString      m_string;
    QTextStream  m_ssOutParams;
};

#endif
