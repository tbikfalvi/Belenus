#include <QPushButton>
#include <QMessageBox>

#include "dlgattendanceedit.h"

cDlgAttendanceEdit::cDlgAttendanceEdit( QWidget *p_poParent, cDBAttendance *p_poAttendance )
    : QDialog( p_poParent )
{
    setupUi( this );

    m_poAttendance = p_poAttendance;
    if( m_poAttendance )
    {
    }
}

cDlgAttendanceEdit::~cDlgAttendanceEdit()
{
}

