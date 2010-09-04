#include "GUIWriter.h"
#include <QMessageBox>


void GUIWriter::_writeLog(const cSeverity::teSeverity sev, const QDateTime ts, const QString &m) {
    QMessageBox::Icon  enIcon  = QMessageBox::NoIcon;
    QString            obTitle = "";

    switch( sev )
    {
        case cSeverity::ERROR:   enIcon  = QMessageBox::Critical;
                                 obTitle = "Error";
                                 break;
        case cSeverity::WARNING: enIcon  = QMessageBox::Warning;
                                 obTitle = "Warning";
                                 break;
        case cSeverity::INFO:    enIcon  = QMessageBox::Information;
                                 obTitle = "Information";
                                 break;
        case cSeverity::DEBUG:   enIcon  = QMessageBox::Information;
                                 obTitle = "Debug message";
                                 break;
        default:                 enIcon  = QMessageBox::NoIcon;
                                 obTitle = "Message";
    }

    QMessageBox obMsg( enIcon, obTitle,
                       m,
                       QMessageBox::Ok );
    obMsg.exec();
}

