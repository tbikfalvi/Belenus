//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_main_dialog.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo dialogus ablak kezelese
//====================================================================================

#include <QtGui>
#include <QtNetwork>

#include "belenusserver.h"
#include "bs_main_dialog.h"
#include "bs_server.h"

//====================================================================================
MainDialog::MainDialog(QWidget *parent) : QDialog(parent)
{
   //---------------------------------------------------------------------------------
   // Alap beallitasok a dialogus ablakhoz
   //---------------------------------------------------------------------------------
   setWindowTitle( tr("Belenus Server Application") );

   QString  strLabel = QString( "" );

   captionLabel = new QLabel;
   statusLabel = new QLabel;
   g_msgLabel = new QLabel;

   //---------------------------------------------------------------------------------
   // TCP szerver inditasa
   //---------------------------------------------------------------------------------
   if( (belenusServer = new cBelenusTcpServer( this )) != NULL )
   {
      if( belenusServer->listen( QHostAddress(g_poPrefs->getServerAddress()), g_poPrefs->getServerPort() ) )
      {
         strLabel += tr( "Server started on address: %1:" ).arg( belenusServer->serverAddress().toString() );
         strLabel += tr( "%1\n" ).arg( belenusServer->serverPort() );
      }
      else
      {
         strLabel += tr( "Unable to initialize server socket communication with settings:\n" );
         strLabel += tr( "Address: %1:" ).arg( belenusServer->serverAddress().toString() );
         strLabel += tr( "%1\n" ).arg( belenusServer->serverPort() );
      }
   }
   else
   {
      strLabel += tr( "SYSTEM ERROR: Unable to create server.\n" );
   }

   //---------------------------------------------------------------------------------
   // Dialogus ablak kinezetenek, ablakban megjeleno informaciok beallitasas
   //---------------------------------------------------------------------------------
   captionLabel->setText( tr("Belenus Server Application") );

   statusLabel->setText( strLabel );
   g_msgLabel->setText( QString(tr("There are %1 client(s) connected to the server")).arg(g_nConnectionCount) );

   QHBoxLayout *captionLayout = new QHBoxLayout;
   captionLayout->addStretch(1);
   captionLayout->addWidget( captionLabel );
   captionLayout->addStretch(1);

   QVBoxLayout *mainLayout = new QVBoxLayout;
   mainLayout->addLayout( captionLayout );
   mainLayout->addWidget( statusLabel );
   mainLayout->addWidget( g_msgLabel );
   mainLayout->addStretch( 1 );

   setLayout(mainLayout);
}
//====================================================================================
MainDialog::~MainDialog()
{
    delete captionLabel;
    delete statusLabel;
    delete g_msgLabel;

    if( belenusServer != NULL )
    {
        belenusServer->close();
    }
}
//====================================================================================
