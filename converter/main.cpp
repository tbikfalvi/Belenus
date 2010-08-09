//====================================================================================
//
// Belenus Converter alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QApplication>
#include <iostream>

//====================================================================================

#include "BelenusConverter.h"

//====================================================================================
void log( QString p_qsMsg )
{
    std::cout << p_qsMsg.toStdString() << std::endl;
}

//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    log( "\nStart Belenus Converter application - version 1.0.0\n\n" );

    return apMainApp.exec();
}
