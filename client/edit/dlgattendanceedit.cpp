#include <QPushButton>
#include <QMessageBox>

#include "dlgattendanceedit.h"
#include "db/dbpostponed.h"

cDlgAttendanceEdit::cDlgAttendanceEdit( QWidget *p_poParent, cDBAttendance *p_poAttendance, cDBPostponed *p_poPostponed )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Attendance" ) );
    setWindowIcon( QIcon("./resources/40x40_attendance.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.png") );

    m_poAttendance = p_poAttendance;
    m_poPostponed = p_poPostponed;

    if( m_poAttendance )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT name, uniqueId FROM patients WHERE patientId=%1" ).arg(p_poAttendance->patientId()) );
        poQuery->first();

        ledName->setText( poQuery->value(0).toString() );
        ledUniqueId->setText( poQuery->value(1).toString() );
        deDate->setDate( QDate::fromString(m_poAttendance->date(),"yyyy-MM-dd") );
        teLength->setTime( QTime::fromString(m_poAttendance->lengthStr(),"hh:mm:ss") );
        ledBPStart->setText( QString::number(m_poAttendance->bloodPressureStart()) );
        ledPulseStart->setText( QString::number(m_poAttendance->pulseStart()) );
        ledBPStop->setText( QString::number(m_poAttendance->bloodPressureStop()) );
        ledPulseStop->setText( QString::number(m_poAttendance->pulseStop()) );

        if( m_poAttendance->id() > 0 )
        {
            pbFinishLater->setEnabled( false );
        }
    }
}

cDlgAttendanceEdit::~cDlgAttendanceEdit()
{
}


void cDlgAttendanceEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    bool  boSkipErrorMessages = false;

    if( (ledBPStart->text() == "" || ledBPStart->text().toInt() < 1) &&
        !boSkipErrorMessages)
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Starting blood pressure must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( (ledPulseStart->text() == "" || ledPulseStart->text().toInt() < 1) &&
        !boSkipErrorMessages)
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Starting pulse value must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( (ledBPStop->text() == "" || ledBPStop->text().toInt() < 1) &&
        !boSkipErrorMessages)
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Ending blood pressure must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( (ledPulseStop->text() == "" || ledPulseStop->text().toInt() < 1) &&
        !boSkipErrorMessages )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Ending pulse value must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }

    if( boCanBeSaved )
    {
        if( SaveAttendanceData() )
            QDialog::accept();
    }
    else
    {
        if( QMessageBox::question( this, tr("Question"),
                                   tr("Do you want to add new attendance to database "
                                      "\nand fill mandatory data later?"),
                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
        {
            on_pbFinishLater_clicked();
        }
    }
}

void cDlgAttendanceEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgAttendanceEdit::on_pbFinishLater_clicked()
{
    QSqlQuery  *poQuery = NULL;
    try
    {
        if( SaveAttendanceData() )
        {            
            if( m_poPostponed == NULL )
            {
                m_poPostponed = new cDBPostponed();

                m_poPostponed->removeAttendance( m_poAttendance->id() );

                m_poPostponed->createNew();
                m_poPostponed->setAttendanceId( m_poAttendance->id() );
                m_poPostponed->save();
                delete m_poPostponed;
            }
            QDialog::reject();
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }
}

bool cDlgAttendanceEdit::SaveAttendanceData()
{
    bool bRet = false;

    if( m_poAttendance->patientId() == 0 )
        m_poAttendance->setPatientId( g_obPatient.id() );

    try
    {
        m_poAttendance->setLicenceId( g_poPrefs->getLicenceId() );
        m_poAttendance->setPatientId( m_poAttendance->patientId() );
        m_poAttendance->setDate( deDate->date().toString("yyyy-MM-dd") );
        m_poAttendance->setLength( teLength->time().hour()*60+teLength->time().minute() );
        m_poAttendance->setBloodPressureStart( ledBPStart->text().toFloat() );
        m_poAttendance->setPulseStart( ledPulseStart->text().toFloat() );
        m_poAttendance->setBloodPressureStop( ledBPStop->text().toFloat() );
        m_poAttendance->setPulseStop( ledPulseStop->text().toFloat() );

        m_poAttendance->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
    }

    return bRet;
}
