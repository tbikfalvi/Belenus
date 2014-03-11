
#include <QPushButton>
#include <QMessageBox>

#include "dlgchangepcvalidity.h"

cDlgChangePCValidity::cDlgChangePCValidity( QWidget *p_poParent ) : QDialog( p_poParent )
{
    m_selValidity   = SV_NONE;

    setupUi( this );

    setWindowTitle( tr("Change validity") );
    setWindowIcon( QIcon("./resources/40x40_clock.png") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    connect( rbOneDay, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbOneWeek, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbTwoWeek, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbOneMonth, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbThreeMonths, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbHalfYear, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbCustom, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );

    deCustom->setDate( QDate::currentDate() );
}

cDlgChangePCValidity::selValidity cDlgChangePCValidity::selectionValidity(QDate *p_qdDate)
{
    if( m_selValidity == SV_CUSTOM )
    {
        p_qdDate->setDate( deCustom->date().year(), deCustom->date().month(), deCustom->date().day() );
    }
    return m_selValidity;
}

void cDlgChangePCValidity::selectionChanged()
{
    deCustom->setEnabled( false );

    if( rbOneDay->isChecked() )
        m_selValidity = SV_ONEDAY;
    else if( rbOneWeek->isChecked() )
        m_selValidity = SV_ONEWEEK;
    else if( rbTwoWeek->isChecked() )
        m_selValidity = SV_TWOWEEKS;
    else if( rbOneMonth->isChecked() )
        m_selValidity = SV_ONEMONTH;
    else if( rbThreeMonths->isChecked() )
        m_selValidity = SV_THREEMONTHS;
    else if( rbHalfYear->isChecked() )
        m_selValidity = SV_HALFYEAR;
    else if( rbCustom->isChecked() )
    {
        deCustom->setEnabled( true );
        m_selValidity = SV_CUSTOM;
    }
    pbOk->setEnabled( true );
}

void cDlgChangePCValidity::on_pbOk_clicked()
{
    if( m_selValidity == SV_CUSTOM && deCustom->date() < QDate::currentDate() )
    {
        QMessageBox::warning( this, tr( "Warning" ),
                              tr( "Incorrect validation date (%1).\n"
                                  "Validation of an active patientcard could not end in the past." ).arg( deCustom->date().toString("yyyy-MM-dd") ) );
        return;
    }

    QDialog::accept();
}
