#include <QPushButton>
#include <QMessageBox>

#include "dlgvalidtimeperiodedit.h"

cDlgValidTimePeriodEdit::cDlgValidTimePeriodEdit( QWidget *p_poParent, const QString &p_qsValidTimePeriod )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Valid time period" ) );
    setWindowIcon( QIcon("./resources/40x40_clock.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_qsValidTimePeriod = p_qsValidTimePeriod.left(14);

    teFrom->setTime( QTime::fromString( m_qsValidTimePeriod.left(5), "HH:mm" ) );
    teTo->setTime( QTime::fromString( m_qsValidTimePeriod.right(5), "HH:mm" ) );

    chkMonday->setChecked( p_qsValidTimePeriod.contains(tr("Mon"), Qt::CaseInsensitive) );
    chkTuesday->setChecked( p_qsValidTimePeriod.contains(tr("Tue"), Qt::CaseInsensitive) );
    chkWednesday->setChecked( p_qsValidTimePeriod.contains(tr("Wed"), Qt::CaseInsensitive) );
    chkThursday->setChecked( p_qsValidTimePeriod.contains(tr("Thu"), Qt::CaseInsensitive) );
    chkFriday->setChecked( p_qsValidTimePeriod.contains(tr("Fri"), Qt::CaseInsensitive) );
    chkSaturday->setChecked( p_qsValidTimePeriod.contains(tr("Sat"), Qt::CaseInsensitive) );
    chkSunday->setChecked( p_qsValidTimePeriod.contains(tr("Sun"), Qt::CaseInsensitive) );
}

cDlgValidTimePeriodEdit::~cDlgValidTimePeriodEdit()
{
}

QString cDlgValidTimePeriodEdit::validPeriod() const
{
    return m_qsValidTimePeriod;
}

void cDlgValidTimePeriodEdit::accept ()
{
    chkMonday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkTuesday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkWednesday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkThursday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkFriday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkSaturday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkSunday->setStyleSheet( "QCheckBox {font: normal;}" );

    if( !chkMonday->isChecked() &&
        !chkTuesday->isChecked() &&
        !chkWednesday->isChecked() &&
        !chkThursday->isChecked() &&
        !chkFriday->isChecked() &&
        !chkSaturday->isChecked() &&
        !chkSunday->isChecked() )
    {
        chkMonday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkTuesday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkWednesday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkThursday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkFriday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkSaturday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkSunday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );

        QMessageBox::critical( this, tr( "Error" ), tr( "One of the weekdays must be selected." ) );
        return;
    }

    m_qsValidTimePeriod = QString( "%1 => %2" ).arg( teFrom->time().toString("HH:mm") ).arg( teTo->time().toString("HH:mm") );

    if( chkMonday->isChecked() )    m_qsValidTimePeriod.append( tr(" Mon") );
    if( chkTuesday->isChecked() )   m_qsValidTimePeriod.append( tr(" Tue") );
    if( chkWednesday->isChecked() ) m_qsValidTimePeriod.append( tr(" Wed") );
    if( chkThursday->isChecked() )  m_qsValidTimePeriod.append( tr(" Thu") );
    if( chkFriday->isChecked() )    m_qsValidTimePeriod.append( tr(" Fri") );
    if( chkSaturday->isChecked() )  m_qsValidTimePeriod.append( tr(" Sat") );
    if( chkSunday->isChecked() )    m_qsValidTimePeriod.append( tr(" Sun") );

    QDialog::accept();
}
