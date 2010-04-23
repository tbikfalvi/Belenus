#include <QPushButton>
#include <QMessageBox>

#include "dlgusercardedit.h"

cDlgUserCardEdit::cDlgUserCardEdit( QWidget *p_poParent, cDBUserCard *p_poUserCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poUserCard = p_poUserCard;
    if( m_poUserCard->id() )
    {
        ledBarCode->setText( QString::fromStdString( m_poUserCard->barCode() ) );
        ledComment->setText( QString::fromStdString( m_poUserCard->comment() ) );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT usercardtypeid, name FROM usercardtypes ORDER BY name" ) );
        while( poQuery->next() )
        {
            cmbUserCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poUserCard->userCardTypeId() == poQuery->value( 0 ) )
                cmbUserCardType->setCurrentIndex( cmbUserCardType->count()-1 );
        }

        ledUnits->setText( QString::number( m_poUserCard->units() ) );
        dtpValidDate->setDate( QDate( m_poUserCard->validDateYear(),
                                      m_poUserCard->validDateMonth(),
                                      m_poUserCard->validDateDay() ) );
        ledPinCode->setText( QString::number( m_poUserCard->pinCode() ) );
    }
    else
    {
        dtpValidDate->setDate( QDate::currentDate().addYears(1) );
    }

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
}

cDlgUserCardEdit::~cDlgUserCardEdit()
{
}

void cDlgUserCardEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( boCanBeSaved )
    {
        try
        {
            m_poUserCard->setBarCode( ledBarCode->text().toStdString() );
            m_poUserCard->setComment( ledComment->text().toStdString() );
            m_poUserCard->setUserCardTypeId( cmbUserCardType->itemData( cmbUserCardType->currentIndex() ).toInt() );
            m_poUserCard->setUnits( ledUnits->text().toUInt() );
            m_poUserCard->setValidDateYear( dtpValidDate->date().year() );
            m_poUserCard->setValidDateMonth( dtpValidDate->date().month() );
            m_poUserCard->setValidDateDay( dtpValidDate->date().day() );
            m_poUserCard->setPinCode( ledPinCode->text().toInt() );

            m_poUserCard->save();
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }

        QDialog::accept();
    }
}

