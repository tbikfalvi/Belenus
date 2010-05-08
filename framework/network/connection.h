#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>
#include "packet.h"


/**
 * Class Connection
 *
 * Handles a QTcpSocket, reads and writes based on protocol of Belenus.
 *
 * Uses:
 *  1) server listens on a port, on every connection it creates a QTcpSocket and creates a new thread to handle data
 *  2) client starts a connection to server host.
 */
class Connection : public QThread {
    Q_OBJECT

public:

    Connection();
    virtual ~Connection();

    void connectTo(QTcpSocket*);    /* server implementation */
    void connectTo(QHostAddress, qint16);   /* client implementation */
    void abortConnection();
    void send(Packet&);

    bool isConnected();

protected slots:
    void _connected();
    void _disconnected();
    void _error(QAbstractSocket::SocketError);
    void _recv();


signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void packetReceived(Packet&);


protected:
    /* inherited */
    void run();

    /* own methods */
    virtual void _connectSignalsToSocket();
    virtual void _handlePacket(Packet &);

    /* protocol */
    virtual void _initialize() {};              /* called when connection established to initialize protocol */
    virtual void _handleHello(Packet&) {};

    QList<Packet::Message> _allowedPackets;     /* list of packets which are allowed to be received. all other will cause connection to be dropped */
    QTcpSocket *_socket;
    QByteArray _buffer;
};



#endif // CONNECTION_H
