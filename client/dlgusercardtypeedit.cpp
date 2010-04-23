#include <QPushButton>
#include <QMessageBox>

#include "dlgusercardtypeedit.h"

cDlgUserCardTypeEdit::cDlgUserCardTypeEdit( QWidget *p_poParent, cDBUserCardType *p_poUserCardType ) : QDialog( p_poParent )
{
    setupUi( this );

    dtpDateFrom->setDate( QDate::currentDate() );
    dtpDateTo->setDate( QDate::currentDate() );

    m_poUserCardType = p_poUserCardType;
    if( m_poUserCardType )
    {
        ledName->setText( QString::fromStdString( m_poUserCardType->name() ) );
        ledPrice->setText( QString::number( m_poUserCardType->price() ) );
        ledUnits->setText( QString::number( m_poUserCardType->units() ) );
        ledUnitTime->setText( QString::number( m_poUserCardType->unitTime() ) );
        ledDays->setText( QString::number( m_poUserCardType->validDays() ) );

        if( m_poUserCardType->validDays() > 0 )
        {
            on_rbValidDays_clicked();
        }
        else
        {
            on_rbValidDates_clicked();
            dtpDateFrom->setDate( QDate( m_poUserCardType->validDateFromYear(),
                                         m_poUserCardType->validDateFromMonth(),
                                         m_poUserCardType->validDateFromDay() ) );
            dtpDateTo->setDate( QDate( m_poUserCardType->validDateToYear(),
                                       m_poUserCardType->validDateToMonth(),
                                       m_poUserCardType->validDateToDay() ) );
        }
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgUserCardTypeEdit::~cDlgUserCardTypeEdit()
{
}

void cDlgUserCardTypeEdit::on_rbValidDates_clicked()
{
    rbValidDates->setChecked( true );
    dtpDateFrom->setEnabled( true );
    dtpDateTo->setEnabled( true );

    rbValidDays->setChecked( false );
    ledDays->setEnabled( false );
}

void cDlgUserCardTypeEdit::on_rbValidDays_clicked()
{
    rbValidDates->setChecked( false );
    dtpDateFrom->setEnabled( false );
    dtpDateTo->setEnabled( false );

    rbValidDays->setChecked( true );
    ledDays->setEnabled( true );
}

void cDlgUserCardTypeEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Cardtype name cannot be empty." ) );
        ledName->setFocus();
        return;
    }
    if( ledPrice->text().toInt() < 0 )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Price of Cardtype cannot be negativ." ) );
        ledPrice->setFocus();
        return;
    }
    if( ledUnits->text().toInt() < 1 )
    {
        if( QMessageBox::question( this, tr( "Confirmation" ),
                                   tr( "Number of units for Usercard type should be greater than zero.\nAre you sure you want to use zero value?" ),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
        {
            boCanBeSaved = false;
            ledUnits->setFocus();
            return;
        }
    }
    if( ledUnitTime->text().toInt() < 1 )
    {
        if( QMessageBox::question( this, tr( "Confirmation" ),
                                   tr( "Time for units of Usercard type should be greater than zero.\nAre you sure you want to use zero value?" ),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
        {
            boCanBeSaved = false;
            ledUnitTime->setFocus();
            return;
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poUserCardType->setName( ledName->text().toStdString() );
            m_poUserCardType->setPrice( ledPrice->text().toInt() );
            m_poUserCardType->setUnits( ledUnits->text().toUInt() );
            m_poUserCardType->setUnitTime( ledUnitTime->text().toUInt() );
            if( rbValidDates->isChecked() )
            {
                m_poUserCardType->setValidDateFromYear( dtpDateFrom->date().year() );
                m_poUserCardType->setValidDateFromMonth( dtpDateFrom->date().month() );
                m_poUserCardType->setValidDateFromDay( dtpDateFrom->date().day() );

                m_poUserCardType->setValidDateToYear( dtpDateTo->date().year() );
                m_poUserCardType->setValidDateToMonth( dtpDateTo->date().month() );
                m_poUserCardType->setValidDateToDay( dtpDateTo->date().day() );

                m_poUserCardType->setValidDays( 0 );
            }
            else if( rbValidDays->isChecked() )
            {
                m_poUserCardType->setValidDays( ledDays->text().toUInt() );
            }

            m_poUserCardType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}
