#ifndef SEVEXCEPTION_H
#define SEVEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class cSeverity
{
public:
    enum teSeverity
    {
        MIN = 0,
        NONE,
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        MAX
    };

    static const char *toStr( teSeverity p_enSev ) {
        switch( p_enSev ) {
            case NONE:    return "NONE";    break;
            case ERROR:   return "ERROR";   break;
            case WARNING: return "WARNING"; break;
            case INFO:    return "INFO";    break;
            case DEBUG:   return "DEBUG";   break;
            default:      return "INVALID";
        }
    }
};

class cSevException : public exception
{
public:
    cSevException( const cSeverity::teSeverity  p_enSev,
                   const string                &p_stMsg )
        : m_enSeverity( p_enSev ), m_stMsg( p_stMsg ) { }
    virtual ~cSevException() throw() { }

    cSeverity::teSeverity severity() const throw() {
        return m_enSeverity;
    }
    virtual const char* what() const throw() { return m_stMsg.c_str(); }

private:
    cSeverity::teSeverity  m_enSeverity;
    string                 m_stMsg;
};

#endif

