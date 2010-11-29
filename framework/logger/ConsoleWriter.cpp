
#include "ConsoleWriter.h"
#include <iostream>


void ConsoleWriter::_writeLog(const cSeverity::teSeverity /*sev*/, const QDateTime /*ts*/, const QString &/*m*/, const void * /*threadId*/) {
/*    _coutLogGuard.lock();
    cerr << ts.toString(Qt::ISODate).toStdString() << " ";
    cerr << " [" << threadId << "] ";
    cerr << cSeverity::toStr( sev );
    if( !m.isEmpty() )
        cerr << " " << m.toStdString();
    cerr << endl << flush;
    _coutLogGuard.unlock();*/
}
