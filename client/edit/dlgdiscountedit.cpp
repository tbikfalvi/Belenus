#include <QPushButton>
#include <QMessageBox>

#include "dlgdiscountedit.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgDiscountEdit::cDlgDiscountEdit( QWidget *p_poParent, cDBDiscount *p_poDiscount ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Discount" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    checkIndependent->setVisible( false );

    m_poDiscount = p_poDiscount;
    if( m_poDiscount->id() > 0 )
    {
        ledName->setText( m_poDiscount->name() );
        rbRegularCustomer->setEnabled( false );
        rbEmployee->setEnabled( false );
        rbService->setEnabled( false );
        rbGuest->setEnabled( false );
        rbCompany->setEnabled( false );
        rbProductType->setEnabled( false );
        rbProduct->setEnabled( false );

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
        else if( m_poDiscount->guestId() > 0 )
        {
            rbGuest->setChecked( true );
        }
        else if( m_poDiscount->companyId() > 0 )
        {
            rbCompany->setChecked( true );
        }
        else if( m_poDiscount->productTypeId() > 0 )
        {
            rbProductType->setChecked( true );
        }
        else if( m_poDiscount->productId() > 0 )
        {
            rbProduct->setChecked( true );
        }
        slotFillHCDComboList();

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
            ledName->setEnabled( false );
            rbRegularCustomer->setEnabled( false );
            rbEmployee->setEnabled( false );
            rbService->setEnabled( false );
            rbGuest->setEnabled( false );
            rbCompany->setEnabled( false );
            rbProductType->setEnabled( false );
            rbProduct->setEnabled( false );
            cmbHCDList->setEnabled( false );
            rbDiscountValue->setEnabled( false );
            rbDiscountPercent->setEnabled( false );
            ledDiscount->setEnabled( false );
            pbSave->setEnabled( false );
        }
    }
    if( m_poDiscount->id() > 0 )
        checkIndependent->setEnabled( false );

    connect( rbRegularCustomer, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbEmployee, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbService, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbGuest, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbCompany, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbProductType, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbProduct, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditDiscount", QPoint(517,269) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgDiscountEdit::~cDlgDiscountEdit()
{
    g_poPrefs->setDialogSize( "EditDiscount", QPoint( width(), height() ) );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgDiscountEdit::slotRefreshWarningColors()
{
    lblName->setStyleSheet( "QLabel {font: normal;}" );

    if( ledName->text().length() == 0 )
    {
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgDiscountEdit::accept ()
{
    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    if( ledName->text().length() == 0 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Name of the discount can not be empty!\n"
                                   "Please add a name to the discount." ) );
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( rbRegularCustomer->isChecked() )
    {
        if( m_poDiscount->isRegularCustomerExists() )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for regular customer already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbEmployee->isChecked() )
    {
        if( m_poDiscount->isEmployeeExists() )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for employee already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbService->isChecked() )
    {
        if( m_poDiscount->isServiceExists() )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for service person already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbGuest->isChecked() )
    {
        if( m_poDiscount->isGuestExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for selected guest already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbCompany->isChecked() )
    {
        if( m_poDiscount->isCompanyExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for selected company already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbProductType->isChecked() )
    {
        if( m_poDiscount->isProductTypeExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for selected product type already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }
    else if( rbProduct->isChecked() )
    {
        if( m_poDiscount->isProductExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for selected product already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }

    if( (rbGuest->isChecked() ||
         rbCompany->isChecked() ||
         rbProductType->isChecked() ||
         rbProduct->isChecked()) &&
        cmbHCDList->currentIndex() < 1 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "One of the item from the list must be selected!" ) );
        cmbHCDList->setFocus();
    }

    bool    boConversion = true;
    ledDiscount->text().toInt( &boConversion );
    if( !boConversion )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Value of discount is invalid.\nPlease use only numbers." ) );
        ledDiscount->setFocus();
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poDiscount->setLicenceId( g_poPrefs->getLicenceId() );
            if( rbRegularCustomer->isChecked() )
            {
                m_poDiscount->setRegularCustomer( true );
            }
            else if( rbEmployee->isChecked() )
            {
                m_poDiscount->setEmployee( true );
            }
            else if( rbService->isChecked() )
            {
                m_poDiscount->setService( true );
            }
            else if( rbGuest->isChecked() ||
                     rbCompany->isChecked() ||
                     rbProductType->isChecked() ||
                     rbProduct->isChecked() )
            {
                if( rbGuest->isChecked() )
                {
                    ledName->setText( tr("Guest discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setGuestId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbCompany->isChecked() )
                {
                    ledName->setText( tr("Company discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setCompanyId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbProductType->isChecked() )
                {
                    ledName->setText( tr("Product type discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setProductTypeId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbProduct->isChecked() )
                {
                    ledName->setText( tr("Product discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setProductId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
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
            m_poDiscount->setName( ledName->text() );
            m_poDiscount->setActive( true );
            m_poDiscount->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgDiscountEdit::slotFillHCDComboList()
{
    QSqlQuery *poQuery = NULL;

    cmbHCDList->setEnabled( true );
    cmbHCDList->clear();

    if( rbRegularCustomer->isChecked() ||
        rbEmployee->isChecked() ||
        rbService->isChecked() )
    {
        if( rbRegularCustomer->isChecked() )
            ledName->setText( tr("Regular customer discount") );
        else if( rbEmployee->isChecked() )
            ledName->setText( tr("Employee discount") );
        else if( rbService->isChecked() )
            ledName->setText( tr("Service discount") );

        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        cmbHCDList->setEnabled( false );
    }
    else if( rbGuest->isChecked() )
    {
        ledName->setText( tr("Guest discount") );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name FROM patients WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->guestId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    else if( rbCompany->isChecked() )
    {
        ledName->setText( tr("Company discount") );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT companyId, name FROM companies WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->companyId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    else if( rbProductType->isChecked() )
    {
        ledName->setText( tr("Product type discount") );
        poQuery = g_poDB->executeQTQuery( QString("SELECT productTypeId, name FROM productTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name") );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->productTypeId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    else if( rbProduct->isChecked() )
    {
        ledName->setText( tr("Product discount") );
        poQuery = g_poDB->executeQTQuery( QString("SELECT productId, name FROM products WHERE active=1 AND archive<>\"DEL\" ORDER BY name") );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->productTypeId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    if( poQuery ) delete poQuery;
}

