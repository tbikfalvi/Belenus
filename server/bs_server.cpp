//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_server.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// TCP szerver mukodesenek kezelese
//====================================================================================

#include <iostream>
#include <iomanip>
#include <sstream>

//====================================================================================
#include <QtNetwork>

#include "bs_server.h"
#include "bs_thread.h"

//====================================================================================
cBelenusTcpServer::cBelenusTcpServer(QObject *parent) : QTcpServer(parent)
{
   connect(this, SIGNAL(newConnection()), this, SLOT(connectionAvailable()));
}
//====================================================================================
void cBelenusTcpServer::connectionAvailable()
{
   BelenusThread *connectionThread = new BelenusThread( nextPendingConnection(), this);
   if( connectionThread != NULL )
   {
      connect(connectionThread, SIGNAL(finished()), connectionThread, SLOT(deleteLater()));
      connectionThread->start();
   }
}
//====================================================================================
