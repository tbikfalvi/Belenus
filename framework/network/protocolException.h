#ifndef _PROTOCOLEXCEPTION_H
#define _PROTOCOLEXCEPTION_H


#include "../sevException.h"


enum ProtocolError {
    PROTOCOL_PACKET_SIZE_MISMATCH,
    PROTOCOL_UNEXPECTED_PACKET,
    PROTOCOL_CORRUPT_PACKET,
    PROTOCOL_INTERNAL_ERROR,
};



class ProtocolException : public cSevException
{

public:
    ProtocolException(cSeverity::teSeverity sev, ProtocolError err, const string message)
        : cSevException(sev, message), _err(err)
    {}

    const char* what() const throw()
    {
        string msg("Protocol Exception: ");
        switch (_err) {
            case PROTOCOL_PACKET_SIZE_MISMATCH: msg += "Packet size mismatch"; break;
            case PROTOCOL_UNEXPECTED_PACKET: msg += "Unexpected packet received"; break;
            case PROTOCOL_CORRUPT_PACKET: msg += "Corrupt packet received"; break;
            case PROTOCOL_INTERNAL_ERROR: msg += "Internal error"; break;
            default:
                msg += "Protocol error";
                break;
        }

        if ( !m_stMsg.empty() )
            msg += ". " + m_stMsg;

        return msg.c_str();
    }


protected:
    ProtocolError _err;
};


#endif
