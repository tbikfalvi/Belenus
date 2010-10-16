#include <QPushButton>
#include <QMessageBox>

#include "dlgdiscountedit.h"

cDlgDiscountEdit::cDlgDiscountEdit( QWidget *p_poParent, cDBDiscount *p_poDiscount )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Patient origin" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poDiscount = p_poDiscount;
    if( m_poDiscount )
    {
        if( m_poDiscount->regularCustomer() > 0 )
        {
            rbRegularCustomer->setChecked(true);
        }
        else if( m_poDiscount->employee() > 0 )
        {
            rbEmployee->setChecked( true );
        }
        else if( m_poDiscount->service() > 0 )
        {
            rbService->setChecked( true );
        }
        else if( m_poDiscount->companyId() > 0 )
        {
            rbCompany->setChecked( true );
        }
        else if( m_poDiscount->healthInsuranceId() > 0 )
        {
            rbHealthInsurance->setChecked( true );
        }
        else if( m_poDiscount->doctorId() > 0 )
        {
            rbDoctor->setChecked( true );
        }
        fillHCDComboList();

//        ledName->setText( m_poDiscount->name() );
        if( m_poDiscount->licenceId() == 0 && m_poDiscount->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poDiscount->licenceId() == 0 && m_poDiscount->id() > 0 )
            {
//                ledName->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
    }
}

cDlgDiscountEdit::~cDlgDiscountEdit()
{
}

void cDlgDiscountEdit::accept ()
{
    bool  boCanBeSaved = true;
/*    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Patient origin cannot be empty." ) );
    }*/

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poDiscount->setLicenceId( 0 );
            }
            else
            {
                m_poDiscount->setLicenceId( g_poPrefs->getLicenceId() );
            }
//            m_poDiscount->setName( ledName->text() );
            m_poDiscount->setActive( true );
            m_poDiscount->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgDiscountEdit::fillHCDComboList()
{

}
