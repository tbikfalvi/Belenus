#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include "dlgpatientedit.h"
#include "../framework/sevexception.h"

cDlgPatientEdit::cDlgPatientEdit( QWidget *p_poParent, cDBPatient *p_poPatient )
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

    m_poPatient = p_poPatient;
    if( m_poPatient )
    {
        if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( "root" ) && !g_obUser.isInGroup( "system" ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatient->licenceId() == 0 && m_poPatient->id() > 0 )
            {
                gbIdentification->setEnabled( false );
                gbAddress->setEnabled( false );
                gbVisit->setEnabled( false );
                gbAdditionalData->setEnabled( false );

                pbSave->setEnabled( false );
                pbFinishLater->setEnabled( false );
                pbAttendances->setEnabled( false );
            }
        }
    }
}

cDlgPatientEdit::~cDlgPatientEdit()
{
    cTracer obTrace( "cDlgPatientEdit::~cDlgPatientEdit" );
}

