#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include "dlgpatientedit.h"
#include "../framework/sevexception.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgPatientEdit::cDlgPatientEdit" );

    setupUi( this );
    setWindowIcon( QIcon("./resources/40x40_patient.gif") );

    pbSave->setIconSize(        QSize(20,20) );
    pbCancel->setIconSize(      QSize(20,20) );
    pbFinishLater->setIconSize( QSize(20,20) );
    pbAttendances->setIconSize( QSize(20,20) );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.gif") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.gif") );
    pbAttendances->setIcon( QIcon("./resources/40x40_attendance.gif") );
}

