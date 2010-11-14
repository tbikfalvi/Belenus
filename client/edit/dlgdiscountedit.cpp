#include <QPushButton>
#include <QMessageBox>

#include "dlgdiscountedit.h"

cDlgDiscountEdit::cDlgDiscountEdit( QWidget *p_poParent, cDBDiscount *p_poDiscount )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Discount" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poDiscount = p_poDiscount;
    if( m_poDiscount->id() > 0 )
    {
        rbRegularCustomer->setEnabled( false );
        rbEmployee->setEnabled( false );
        rbService->setEnabled( false );
        rbCompany->setEnabled( false );
        rbHealthInsurance->setEnabled( false );
        rbDoctor->setEnabled( false );

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

        if( m_poDiscount->discountValue() > 0 )
        {
            rbDiscountValue->setChecked(true);
            ledDiscount->setText( QString::number(m_poDiscount->discountValue()) );
        }
        else
        {
            rbDiscountPercent->setChecked(true);
            ledDiscount->setText( QString::number(m_poDiscount->discountPercent()) );
        }
        ledDiscount->setFocus();
    }
    if( m_poDiscount->licenceId() == 0 && m_poDiscount->id() > 0 )
        checkIndependent->setChecked( true );

    if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
    {
        checkIndependent->setEnabled( false );
        if( m_poDiscount->licenceId() == 0 && m_poDiscount->id() > 0 )
        {
            rbRegularCustomer->setEnabled( false );
            rbEmployee->setEnabled( false );
            rbService->setEnabled( false );
            rbCompany->setEnabled( false );
            rbHealthInsurance->setEnabled( false );
            rbDoctor->setEnabled( false );
            cmbHCDList->setEnabled( false );
            rbDiscountValue->setEnabled( false );
            rbDiscountPercent->setEnabled( false );
            ledDiscount->setEnabled( false );
            pbSave->setEnabled( false );
        }
    }
    if( m_poDiscount->id() > 0 )
        checkIndependent->setEnabled( false );
}

cDlgDiscountEdit::~cDlgDiscountEdit()
{
}

void cDlgDiscountEdit::on_rbRegularCustomer_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::on_rbEmployee_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::on_rbService_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::on_rbHealthInsurance_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::on_rbCompany_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::on_rbDoctor_clicked()
{
    fillHCDComboList();
}

void cDlgDiscountEdit::accept ()
{
    bool  boCanBeSaved = true;

    if( rbRegularCustomer->isChecked() )
    {
        if( m_poDiscount->isRegularCustomerExists() )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for regular customer already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    else if( rbEmployee->isChecked() )
    {
        if( m_poDiscount->isEmployeeExists() )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for employee already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    else if( rbService->isChecked() )
    {
        if( m_poDiscount->isServiceExists() )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for service person already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    else if( rbCompany->isChecked() )
    {
        if( m_poDiscount->isCompanyExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for selected company already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    else if( rbHealthInsurance->isChecked() )
    {
        if( m_poDiscount->isHealthInsuranceExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for selected health insurance fund already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    else if( rbDoctor->isChecked() )
    {
        if( m_poDiscount->isDoctorExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            QMessageBox::warning( this,
                                  tr( "Warning" ),
                                  tr( "Discount for selected person already exists!\n"
                                      "Please modify the existing discount." ) );
        }
    }
    if( (rbCompany->isChecked() || rbHealthInsurance->isChecked() || rbDoctor->isChecked()) &&
        cmbHCDList->currentIndex() < 1 )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "One of the item from the list must be selected!" ) );
        cmbHCDList->setFocus();
    }
    bool    boConversion = true;
    ledDiscount->text().toInt( &boConversion );
    if( !boConversion )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Value of discount is invalid.\nPlease use only numbers." ) );
        ledDiscount->setFocus();
    }

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
            if( rbRegularCustomer->isChecked() )
            {
                m_poDiscount->setRegularCustomer( true );
                m_poDiscount->setName( tr("Regular customer") );
            }
            else if( rbEmployee->isChecked() )
            {
                m_poDiscount->setEmployee( true );
                m_poDiscount->setName( tr("Employee") );
            }
            else if( rbService->isChecked() )
            {
                m_poDiscount->setService( true );
                m_poDiscount->setName( tr("Service person") );
            }
            else if( rbCompany->isChecked() ||
                     rbHealthInsurance->isChecked() ||
                     rbDoctor->isChecked() )
            {
                if( rbCompany->isChecked() )
                {
                    m_poDiscount->setCompanyId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbHealthInsurance->isChecked() )
                {
                    m_poDiscount->setHealthInsuranceId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbDoctor->isChecked() )
                {
                    m_poDiscount->setDoctorId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                m_poDiscount->setName( cmbHCDList->itemText(cmbHCDList->currentIndex()) );
            }
            if( rbDiscountValue->isChecked() )
            {
                m_poDiscount->setDiscountValue( ledDiscount->text().toInt() );
                m_poDiscount->setDiscountPercent( 0 );
            }
            else
            {
                m_poDiscount->setDiscountValue( 0 );
                m_poDiscount->setDiscountPercent( ledDiscount->text().toInt() );
            }
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
    QSqlQuery *poQuery = NULL;

    cmbHCDList->setEnabled( true );
    cmbHCDList->clear();

    if( rbRegularCustomer->isChecked() ||
        rbEmployee->isChecked() ||
        rbService->isChecked() )
    {
        cmbHCDList->setEnabled( false );
    }
    else if( rbCompany->isChecked() )
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT companyId, name FROM companies WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->companyId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    else if( rbHealthInsurance->isChecked() )
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT healthInsuranceId, name FROM healthInsurances WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->healthInsuranceId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    else if( rbDoctor->isChecked() )
    {
        poQuery = g_poDB->executeQTQuery( QString("SELECT doctorId, doctorLicence, name FROM doctors WHERE active=1 AND archive<>\"DEL\" ORDER BY doctorLicence") );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            QString qsDoc = QString( "%1 (%2)" ).arg( poQuery->value( 1 ).toString() ).arg( poQuery->value( 2 ).toString() );
            cmbHCDList->addItem( qsDoc, poQuery->value( 0 ) );
            if( m_poDiscount->doctorId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    if( poQuery ) delete poQuery;
}

