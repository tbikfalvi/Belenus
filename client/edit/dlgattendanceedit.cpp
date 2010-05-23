#include <QPushButton>
#include <QMessageBox>

#include "dlgattendanceedit.h"
#include "db/dbpostponed.h"

cDlgAttendanceEdit::cDlgAttendanceEdit( QWidget *p_poParent, cDBAttendance *p_poAttendance )
    : QDialog( p_poParent )
{
    setupUi( this );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.gif") );
    pbFinishLater->setIcon( QIcon("./resources/40x40_hourglass.gif") );

    m_poAttendance = p_poAttendance;
    if( m_poAttendance )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT name, uniqueId FROM patients WHERE patientId=%1" ).arg(p_poAttendance->patientId()) );
        poQuery->first();

        ledName->setText( poQuery->value(0).toString() );
        ledUniqueId->setText( poQuery->value(1).toString() );
        deDate->setDate( QDate::fromString(QString::fromStdString(m_poAttendance->date()),"yyyy-MM-dd") );
        teLength->setTime( QTime::fromString(QString::fromStdString(m_poAttendance->length()),"hh:mm:ss") );
        ledHeight->setText( QString::number(m_poAttendance->height()) );
        ledWeight->setText( QString::number(m_poAttendance->weight()) );
        ptMedicineCurrent->setPlainText( QString::fromStdString(m_poAttendance->medicineCurrent()) );
        ptMedicineAllergy->setPlainText( QString::fromStdString(m_poAttendance->medicineAllergy()) );
        ptComment->setPlainText( QString::fromStdString(m_poAttendance->comment()) );
        ledBPStart->setText( QString::number(m_poAttendance->bloodPressureStart()) );
        ledPulseStart->setText( QString::number(m_poAttendance->pulseStart()) );
        ledBPStop->setText( QString::number(m_poAttendance->bloodPressureStop()) );
        ledPulseStop->setText( QString::number(m_poAttendance->pulseStop()) );
    }
}

cDlgAttendanceEdit::~cDlgAttendanceEdit()
{
}


void cDlgAttendanceEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;
    bool  boSkipErrorMessages = false;

    if( ledBPStart->text() == "" )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Starting blood pressure must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( ledPulseStart->text() == "" )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Starting pulse value must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( (teLength->time().hour() > 0 ||
         teLength->time().minute() > 0 ||
         teLength->time().second() > 0) &&
        ledBPStop->text() == "" )
    {
        boCanBeSaved = false;
        if( QMessageBox::critical( this, tr( "Error" ), tr( "Ending blood pressure must be set.\n\nPress Ignore to skip other error messages." ), QMessageBox::Ok, QMessageBox::Ignore ) == QMessageBox::Ignore )
        {
            boSkipErrorMessages = true;
        }
    }
    if( (teLength->time().hour() > 0 ||
         teLength->time().minute() > 0 ||
         teLength->time().second() > 0) &&
        ledPulseStop->text() == "" )
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
            cDBPostponed    *pDBPostponed = new cDBPostponed();

            pDBPostponed->createNew();
            pDBPostponed->setAttendanceId( m_poAttendance->id() );
            pDBPostponed->save();
            delete pDBPostponed;
/*            QString qsQuery = QString( "INSERT INTO toBeFilled (attendanceId, patientId) VALUES (\"%1\", 0)" ).arg( m_poAttendance->id() );
            poQuery = g_poDB->executeQTQuery( qsQuery );
            delete poQuery;*/

            QDialog::accept();
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;

        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }
}

bool cDlgAttendanceEdit::SaveAttendanceData()
{
    bool bRet = false;

    try
    {
        m_poAttendance->setLicenceId( g_poPrefs->getLicenceId() );
        m_poAttendance->setPatientId( g_obPatient.id() );
        m_poAttendance->setDate( deDate->date().toString("yyyy-MM-dd").toStdString() );
        m_poAttendance->setLength( teLength->time().toString("hh:mm:ss").toStdString() );
        m_poAttendance->setHeight( ledHeight->text().toInt() );
        m_poAttendance->setWeight( ledWeight->text().toInt() );
        m_poAttendance->setMedicineCurrent( ptMedicineCurrent->toPlainText().toStdString() );
        m_poAttendance->setMedicineAllergy( ptMedicineAllergy->toPlainText().toStdString() );
        m_poAttendance->setComment( ptComment->toPlainText().toStdString() );
        m_poAttendance->setBloodPressureStart( ledBPStart->text().toFloat() );
        m_poAttendance->setPulseStart( ledPulseStart->text().toFloat() );
        m_poAttendance->setBloodPressureStop( ledBPStop->text().toFloat() );
        m_poAttendance->setPulseStop( ledPulseStop->text().toFloat() );

        m_poAttendance->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }

    return bRet;
}
