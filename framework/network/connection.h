#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>
#include <QWaitCondition>
#include <QMutex>
#include "packet.h"
#include "protocolException.h"
#include "sqlResult.h"



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
    static const int VERSION = 1;

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
    void sqlResultReady(int id, SqlResult*);

protected:



    /* inherited */
    void run();


    QList<Packet::Message> _allowedPackets;     /* list of packets which are allowed to be received. all other will cause connection to be dropped */
    QTcpSocket *_socket;
    QByteArray _buffer;
    QHostAddress _addr;
    qint16 _port;
};



#endif // CONNECTION_H
