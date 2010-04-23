//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_main_dialog.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// TCP szerver osztaly leirasa
//====================================================================================

#ifndef BS_SERVER_H
#define BS_SERVER_H

//====================================================================================
#include <QTcpServer>
#include <QLabel>

//====================================================================================
class cBelenusTcpServer : public QTcpServer
{
    Q_OBJECT

public slots:
   void connectionAvailable();

public:
    cBelenusTcpServer( QObject *parent = 0 );

protected:

private:

};
//====================================================================================

#endif
