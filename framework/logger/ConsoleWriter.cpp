
#include "ConsoleWriter.h"
#include <iostream>


void ConsoleWriter::_writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m) {
    cerr << ts.toString(Qt::ISODate).toStdString() << " ";
    cerr << cSeverity::toStr( sev );
    if( !m.isEmpty() )
        cerr << " " << m.toStdString();
    cerr << endl << flush;
}
