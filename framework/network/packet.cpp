
#include <sstream>
#include "packet.h"



Packet::Packet()
    : _data(sizeof(PacketHeader), 0)
{
    _header = reinterpret_cast<PacketHeader*>(_data.data());
}



Packet::Packet(PacketHeader hdr, const char *data)
    : _data(hdr.length + sizeof(PacketHeader), 0)
{
    _header = reinterpret_cast<PacketHeader*>(_data.data());
    memcpy(_header, &hdr, sizeof(PacketHeader));
    memcpy(_data.data()+sizeof(PacketHeader), data, hdr.length);
}



Packet::Packet(Message msgId)
    :   _data(sizeof(PacketHeader), 0)
{
    _header = reinterpret_cast<PacketHeader*>(_data.data());
    _header->packetId = msgId;
    _header->magic = Packet::MAGIC;
    _header->length = 0;
}



Packet::~Packet()
{}



QByteArray & Packet::getPacket()
{
    _header->length = _data.size() - sizeof(PacketHeader);
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
    if ( hdr.magic != Packet::MAGIC )
        throw 0;

    int size = sizeof(hdr) + hdr.length;
    if ( buffer.size() < size )
        return 0;     // not all data is available yet

    Packet *packet = new Packet( hdr, buffer.data() + sizeof(hdr));

    // copy the remaining data to the front of buffer
    memcpy(buffer.data(), buffer.data()+size, buffer.size()-size);
    buffer.resize(buffer.size() - size);

    return packet;
}


