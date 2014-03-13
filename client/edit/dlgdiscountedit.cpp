#include <QPushButton>
#include <QMessageBox>

#include "dlgdiscountedit.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgDiscountEdit::cDlgDiscountEdit( QWidget *p_poParent, cDBDiscount *p_poDiscount ) : QDialog( p_poParent )
{
    m_bLoading  = true;

    setupUi( this );

    setWindowTitle( tr( "Discount" ) );
    setWindowIcon( QIcon("./resources/40x40_discount.png") );

    cmbHCDList->setEnabled( false );
    gbDiscountCategory->setStyleSheet( "QGroupBox {font: bold; color: red;}" );
    gbValue->setStyleSheet( "QGroupBox {font: bold; color: red;}" );
    lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );

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
        rbPaymentMethod->setEnabled( false );
        rbProduct->setEnabled( false );
        rbCoupon->setEnabled( false );
        cmbHCDList->setEnabled( false );

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
        else if( m_poDiscount->paymentMethodId() > 0 )
        {
            rbPaymentMethod->setChecked( true );
        }
        else if( m_poDiscount->productId() > 0 )
        {
            rbProduct->setChecked( true );
        }
        else
        {
            rbCoupon->setChecked( true );
        }
        slotFillHCDComboList();

        if( m_poDiscount->discountValue() > 0 )
        {
            cCurrency   cValue( m_poDiscount->discountValue() );
            rbDiscountValue->setChecked(true);
            ledDiscount->setText( cValue.currencyString() );
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
            rbPaymentMethod->setEnabled( false );
            rbProduct->setEnabled( false );
            rbCoupon->setEnabled( false );
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
    connect( rbPaymentMethod, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbProduct, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( rbCoupon, SIGNAL(clicked()), this, SLOT(slotFillHCDComboList()) );
    connect( ledDiscount, SIGNAL(textEdited(QString)), this, SLOT(slotCheckValue()) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditDiscount", QPoint(517,269) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bLoading = false;

    slotCheckValue();
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
    else if( rbPaymentMethod->isChecked() )
    {
        if( m_poDiscount->isPaymentMethodExists(cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt()) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount for selected payment method already exists!\n"
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
    else if( rbCoupon->isChecked() )
    {
        if( m_poDiscount->isCouponExists( ledName->text() ) )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Discount with the name defined already exists!\n"
                                       "Please modify the existing discount." ) );
        }
    }

    if( (rbGuest->isChecked() ||
         rbCompany->isChecked() ||
         rbPaymentMethod->isChecked() ||
         rbProduct->isChecked()) &&
        cmbHCDList->currentIndex() < 1 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "One of the item from the list must be selected!" ) );
        cmbHCDList->setFocus();
    }

    bool    boConversion = true;

    if( rbDiscountValue->isChecked() )
    {
        cCurrency   cDiscount( ledDiscount->text() );
        cDiscount.currencyValue().toInt( &boConversion );
    }
    else
    {
        ledDiscount->text().toInt( &boConversion );
    }
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
                     rbPaymentMethod->isChecked() ||
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
                else if( rbPaymentMethod->isChecked() )
                {
                    ledName->setText( tr("Payment method discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setPaymentMethodId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
                else if( rbProduct->isChecked() )
                {
                    ledName->setText( tr("Product discount - %1").arg(cmbHCDList->itemText(cmbHCDList->currentIndex())) );
                    m_poDiscount->setProductId( cmbHCDList->itemData(cmbHCDList->currentIndex()).toUInt() );
                }
            }
            if( rbDiscountValue->isChecked() )
            {
                cCurrency   cValue( ledDiscount->text() );
                m_poDiscount->setDiscountValue( cValue.currencyValue().toInt() );
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
    lblName->setStyleSheet( "QLabel {font: normal;}" );
    gbDiscountCategory->setStyleSheet( "QGroupBox {font: normal;}" );

    QSqlQuery *poQuery = NULL;

    ledName->setEnabled( false );
    cmbHCDList->setEnabled( true );
    cmbHCDList->clear();

    if( rbCoupon->isChecked() )
    {
        ledName->setEnabled( true );
        if( !m_bLoading )
        {
            ledName->setText( "" );
            lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );
        }
    }

    if( rbRegularCustomer->isChecked() ||
        rbEmployee->isChecked() ||
        rbService->isChecked() ||
        rbCoupon->isChecked() )
    {
        if( rbRegularCustomer->isChecked() )
            ledName->setText( tr("Regular customer discount") );
        else if( rbEmployee->isChecked() )
            ledName->setText( tr("Employee discount") );
        else if( rbService->isChecked() )
            ledName->setText( tr("Service discount") );

        cmbHCDList->addItem( tr("<Not selected>"), 0 );
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
    else if( rbPaymentMethod->isChecked() )
    {
        ledName->setText( tr("Payment method discount") );
        poQuery = g_poDB->executeQTQuery( QString("SELECT paymentMethodId, name FROM paymentmethods WHERE paymentMethodId>1 AND active=1 AND archive<>\"DEL\" ORDER BY name") );
        cmbHCDList->addItem( tr("<Not selected>"), 0 );
        while( poQuery->next() )
        {
            cmbHCDList->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
            if( m_poDiscount->paymentMethodId() == poQuery->value( 0 ) )
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
            if( m_poDiscount->productId() == poQuery->value( 0 ) )
                cmbHCDList->setCurrentIndex( cmbHCDList->count()-1 );
        }
    }
    if( poQuery ) delete poQuery;

    if( rbRegularCustomer->isChecked() ||
        rbEmployee->isChecked() ||
        rbService->isChecked() ||
        rbCoupon->isChecked() ||
        m_poDiscount->id() > 0 )
    {
        cmbHCDList->setEnabled( false );
    }
}


void cDlgDiscountEdit::slotCheckValue()
{
    gbValue->setStyleSheet( "QGroupBox {font: bold; color: red;}" );

    bool    boConversion = false;

    cCurrency   cDiscount( ledDiscount->text() );
    cDiscount.currencyValue().toInt( &boConversion );

    if( ledDiscount->text().length() > 0 && boConversion )
        gbValue->setStyleSheet( "QGroupBox {font: normal;}" );
}

void cDlgDiscountEdit::on_ledName_textChanged(const QString &arg1)
{
    if( ledName->text().length() > 0 )
        lblName->setStyleSheet( "QLabel {font: normal;}" );
    else
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );
}
