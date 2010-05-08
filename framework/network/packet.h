#ifndef _packet_h
#define _packet_h

#include <QString>


class Packet {
public:
    static const qint16 MAGIC = 0x326f;

    enum Message {
        MSG_HELLO,
        MSG_LOGON_CHALLENGE,
        MSG_LOGON_RESPONSE,
        MSG_LOGON_ADMIN_RESPONSE,
        MSG_LOGON_OK,
        MSG_DISCONNECT,
        MSG_VERSION_MISMATCH,
        MSG_REGISTER_LICENSE_KEY,
        MSG_REGISTER_LICENSE_KEY_DONE,
    };


    typedef struct {
        Message packetId;
        qint16 magic;
        qint16 length;
    } PacketHeader;


    Packet();
    Packet(PacketHeader hdr, const char *data);
    Packet(Message);
    ~Packet();

    Message getId() const {
        return _header->packetId;
    }
    qint64 getLength() const {
        return _header->length;
    }
    QByteArray & getPacket();

    static Packet * createPacket(QByteArray &buffer);


    Packet &operator <<( const char *param );
    Packet &operator <<( char *param );

    template<typename T>
    Packet & operator <<( const T param ) {
        int size = _data.size();
        _data.resize(size + sizeof(T) );
        memcpy(_data.data() + size, &param, sizeof(T));
        return *this;
    }

protected:
    PacketHeader *_header;
    QByteArray _data;
};



#endif
