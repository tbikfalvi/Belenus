#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>
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
    enum AssertType {
        EXACT, MINIMUM
    };

    enum Result {
        RESULT_OK,
        RESULT_UNKOWN,
        RESULT_INVALID_VERSION,
        RESULT_INVALID_LICENSE_KEY,
        RESULT_INVALID_SECOND_ID,
        RESULT_AUTHENTICATION_FAILED,
        RESULT_ALREADY_REGISTERED,
    };


    /* inherited */
    void run();

    /* own methods */
    virtual void _connectSignalsToSocket();
    virtual void _handlePacket(Packet &);
    void _assertSize(unsigned int size, Packet &, AssertType type = EXACT);

    /* protocol */
    virtual void _initialize() {};              /* called when connection established to initialize protocol */
    virtual void _handleHello(int /*version*/) {};
    virtual void _handleLogonChallenge() {};
    virtual void _handleLogonAdminResponse(const char* /*username*/, const char* /*password*/) {};
    virtual void _handleLogonResponse(const char* /*code1*/, const char* /*code2*/) {};
    virtual void _handleLogonOk() {};
    virtual void _handleDisconnect(Result /*reason*/) {};
    virtual void _handleRegisterKey(const char* /*key*/) {};
    virtual void _handleRegisterKeyResponse(Result /*result*/) {};
    virtual void _handleSqlQuery(int /*queryId*/, const char* /*query*/) {}
    virtual void _handleSqlQueryResult(int /*queryId*/, SqlResult *);

    virtual void _sendHello();
    virtual void _sendDisconnect(Result reason);
    virtual void _sendLogonChallenge();
    virtual void _sendLogonResponse(const char* code1, const char *code2);
    virtual void _sendLogonAdminResponse(const char* username, const char* password);
    virtual void _sendLogonOk();
    virtual void _sendRegisterKey(const char* key);
    virtual void _sendRegisterKeyResponse(Result result);
    virtual void _sendSqlQuery(int queryId, const char *query);
    virtual void _sendSqlQueryResult(int queryId, SqlResult &);

    QList<Packet::Message> _allowedPackets;     /* list of packets which are allowed to be received. all other will cause connection to be dropped */
    QTcpSocket *_socket;
    QByteArray _buffer;
    QHostAddress _addr;
    qint16 _port;
};



#endif // CONNECTION_H
