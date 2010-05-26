
#include <sstream>
#include "packet.h"


const char * Packet::_packetNames[] = {
    "MSG_HELLO",
    "MSG_LOGON_CHALLENGE",
    "MSG_LOGON_RESPONSE",
    "MSG_LOGON_ADMIN_RESPONSE",
    "MSG_LOGON_OK",
    "MSG_DISCONNECT",
    "MSG_REGISTER_LICENSE_KEY",
    "MSG_REGISTER_LICENSE_KEY_RESPONSE",
    "MSG_SQL_QUERY",
    "MSG_SQL_RESULT"
};



Packet::Packet(PacketHeader hdr, const char *data)
    : _data(hdr.length + sizeof(PacketHeader), 0)
{
    _header.packetId = hdr.packetId;
    _header.length = hdr.length;
    _header.magic = hdr.magic;
    memcpy(_data.data()+sizeof(PacketHeader), data, hdr.length);

    _readPos = sizeof(PacketHeader);    // setting reading position to the beginging of data
}



Packet::Packet(Message msgId)
    :   _data(sizeof(PacketHeader), 0)
{
    _header.packetId = msgId;
    _header.magic = Packet::MAGIC;
    _header.length = 0;
    _readPos = sizeof(PacketHeader);    // setting reading position
}



Packet::~Packet()
{}



QByteArray & Packet::getRawPacket()
{
    // copy header into the data array
    if ( (unsigned int)_data.size() < sizeof(PacketHeader) )
        throw ProtocolException(cSeverity::ERROR, PROTOCOL_INTERNAL_ERROR, "not enough size for header");
    memcpy(_data.data(), &_header, sizeof(PacketHeader));
    return _data;
}



/**
  Creates a Packet object from the provided byte array
  if data is available. Otherwise it returns NULL.
  The parsed data of packet will be removed from the buffer,
  by copying the rest of the data to the begining
 */
Packet *Packet::createPacket(QByteArray &buffer)
{
    if ( buffer.size() < static_cast<int>(sizeof(PacketHeader)) )
        return 0;

    PacketHeader hdr;
    memcpy(&hdr, buffer.data(), sizeof(hdr));

    if ( hdr.packetId >= _NO_PACKETS_ )
        throw ProtocolException(cSeverity::ERROR, PROTOCOL_CORRUPT_PACKET, "Unkown packet");

    if ( hdr.magic != Packet::MAGIC )
        throw ProtocolException(cSeverity::ERROR, PROTOCOL_CORRUPT_PACKET, "Invalid magic");

    int size = sizeof(hdr) + hdr.length;
    if ( buffer.size() < size )
        return 0;     // not all data is available yet

    Packet *packet = new Packet( hdr, buffer.data() + sizeof(hdr));

    // copy the remaining data to the front of buffer
    if ( buffer.size()-size )
        memcpy(buffer.data(), buffer.data()+size, buffer.size()-size);

    buffer.resize(buffer.size() - size);

    return packet;
}



QString Packet::dump()
{
    QString msg;
    QByteArray & data = getRawPacket();
    msg.reserve( _header.length*3 + 30 );

    msg += QString("Length: %1.\n").arg(_header.length);
    for (int i=0; i<data.size(); ++i) {
        if (i>0 && i%4==0)
            msg += " ";
        msg += QString("%1 ").arg( (unsigned char)data.at(i), (int)2, (int)16, (QChar)'0' );
    }

    if ( data.size()>0 ) {
        msg += "|";
        for (int i=0; i<data.size(); ++i) {
            if (i>0 && i%4==0) msg += " ";
            if ( data.at(i)>32 )
                msg += QString("%1").arg( (char)data.at(i) );
            else
                msg += ".";
        }
    }


    return msg;
}



Packet &Packet::operator <<( const char *param )
{
    return this->operator <<(const_cast<char*>(param));
}



Packet &Packet::operator <<( string param )
{
    return this->operator <<(param.c_str());
}



Packet &Packet::operator <<( QString param )
{
    return this->operator << (param.toStdString().c_str());
}



Packet &Packet::operator <<( char *param )
{
    char *p = param;
    while ( *(p++) );
    int size = _data.size();
    _data.resize(_data.size() + p - param );
    _header.length = _data.size() - sizeof(PacketHeader);
    memcpy(_data.data() + size, param, p - param);
    return *this;
}



Packet &Packet::operator >>( char* &param )
{
    param = _data.data() + _readPos;
    char *p = param;
    unsigned int size = _header.length + sizeof(_header);

    while ( _readPos<size && *p ) {
        ++p;
        ++_readPos;
    }

    if ( *p==0 ) {
        ++_readPos; // we stopped on \0 so stand on the next element
    } else {
        // what we read is not a correct string as there is no ending \0.
        // So return a NULL. _readPos already reached the limit
        param = 0;
        throw ProtocolException(cSeverity::WARNING, PROTOCOL_CORRUPT_PACKET, QString("No trailing \\0 was found for string").toStdString());
    }
    return *this;
}


