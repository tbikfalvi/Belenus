#ifndef _packet_h
#define _packet_h

#include <QString>
#include "protocolException.h"

#include <stdio.h>


/**
  * Handled packets
  *
  * HELLO (S|C)
  *   version
  * LOGON_CHALLANGE (S)
  * LOGON_RESPONSE (C)
  *   id1
  *   id2 - if id2 is NULL in the database, server will save it and expect the same next time a client logs in
  * ADMIN_LOGON_RESPONSE
  *   user
  *   password
  * LOGIN_OK (S)
  * DISCONNECT (S)
  *   reason - integer. See for REASON_ enums
  * REGISTER_LICENSE_KEY (C) - only by admins
  *   code - NOT SHA1 encoded
  * REGISTER_LICENSE_KEY_RESULT (S)
  *   result - (OK|ALREADY_REGISTERED)
  * SQL_QUERY (C)
  *   int id - this id will returned with the result
  *   sql-query - string containing the query
  * SQL_RESULT
  *   int id - id received from the query packet
  *   validity
  *   affectedRows
  *   ByteArray - the query
  */

class Packet {
public:
    static const qint16 MAGIC = 0x326f;

    static const char *_packetNames[];
    /* do not forget to modify _packetNames in packet.cpp if you modify this enum. Indexes here must be synched with the names */
    enum Message {
        MSG_HELLO,
        MSG_LOGON_CHALLENGE,
        MSG_LOGON_RESPONSE,
        MSG_LOGON_ADMIN_RESPONSE,
        MSG_LOGON_RESULT,
        MSG_DISCONNECT, /* 5 */
        MSG_REGISTER_LICENSE_KEY,
        MSG_REGISTER_LICENSE_KEY_RESPONSE,
        MSG_SQL_QUERY,
        MSG_SQL_RESULT,
        _NO_PACKETS_
    };


    typedef struct {
        Message packetId;
        qint16 magic;
        qint16 length;  // sizeof data in packet. Without header.
    } PacketHeader;


    Packet(PacketHeader hdr, const char *data);
    Packet(Message);
    ~Packet();

    Message getId() const { return _header.packetId; }
    qint64 getLength() const { return _header.length; }
    const char* getPacketName() const { if ( _header.packetId<_NO_PACKETS_ && _header.packetId>=0 ) return Packet::_packetNames[_header.packetId]; else return "#INVALID#"; }
    QString dump();
    QByteArray & getRawPacket();

    static Packet * createPacket(QByteArray &buffer);


    /*
     * Operators
     */
    Packet &operator <<( const char *param );
    Packet &operator <<( char *param );
    Packet &operator <<( string param );
    Packet &operator <<( QString param );

    template<typename T>
    Packet & operator <<( const T param ) {
        int size = _data.size();
        _data.resize(size + sizeof(T) );
        _header.length = _data.size() - sizeof(PacketHeader);
        memcpy(_data.data() + size, &param, sizeof(T));
        return *this;
    }


    /*
     * Readers
     */

    /*
     * returns a pointer to the the string. Pointer is inside the packet buffer.
     * After call you should copy its value in case you want it to use later, as
     * the pointer goes invalid if packet changes or became deleted
     */
    Packet &operator >>( char *&param );

    template<typename T>
    Packet & operator >>( T &param ) {
        if ( sizeof(T) > _data.size() - _readPos )
            throw ProtocolException(cSeverity::WARNING, PROTOCOL_CORRUPT_PACKET, QString("Tried to read %1 bytes but %2 is available").arg(sizeof(T)).arg((_data.size()-_readPos)).toStdString());
        T* p = reinterpret_cast<T*>(_data.data() + _readPos);
        param = *p;
        _readPos += sizeof(T);

        return *this;
    }
protected:
    PacketHeader _header;
    QByteArray _data;
    unsigned int _readPos;
};



#endif
