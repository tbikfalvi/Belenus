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

    m_qsValidTimePeriod = p_qsValidTimePeriod;
    if( m_qsValidTimePeriod.length() == 14 )
    {
        teFrom->setTime( QTime::fromString( m_qsValidTimePeriod.left(5), "HH:mm" ) );
        teTo->setTime( QTime::fromString( m_qsValidTimePeriod.right(5), "HH:mm" ) );
    }
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
    m_qsValidTimePeriod = QString( "%1 => %2" ).arg( teFrom->time().toString("HH:mm") ).arg( teTo->time().toString("HH:mm") );

    QDialog::accept();
}
