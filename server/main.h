#ifndef _MAIN_H
#define _MAIN_H


#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();

public slots:
    void execute();
    void connectionAvailable();

protected:
    QTcpServer _tcpServer;
};



#endif

