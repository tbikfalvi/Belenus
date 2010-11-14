#include <QPushButton>
#include <QMessageBox>

#include "dlgdoctoredit.h"

cDlgDoctorEdit::cDlgDoctorEdit( QWidget *p_poParent, cDBDoctor *p_poDoctor )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Advisor" ) );
    setWindowIcon( QIcon("./resources/40x40_doctor.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poDoctor = p_poDoctor;

    QSqlQuery *poQuery;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT doctorTypeId, name FROM doctorTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    cmbDoctorType->addItem( tr("<Not selected>"), 0 );
    while( poQuery->next() )
    {
        cmbDoctorType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        if( m_poDoctor->doctorTypeId() == poQuery->value( 0 ) )
            cmbDoctorType->setCurrentIndex( cmbDoctorType->count()-1 );
    }

    if( m_poDoctor )
    {
        ledName->setText( m_poDoctor->name() );
        ledLicence->setText( m_poDoctor->licence() );
        ptData->setPlainText( m_poDoctor->data() );
        ( m_poDoctor->discountType()==1?rbDiscountValue->setChecked(true):rbDiscountPercent->setChecked(true) );
        ledDiscount->setText( QString::number(m_poDoctor->discount()) );

        if( m_poDoctor->licenceId() == 0 && m_poDoctor->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poDoctor->licenceId() == 0 && m_poDoctor->id() > 0 )
            {
                ledName->setEnabled( false );
                ledLicence->setEnabled( false );
                cmbDoctorType->setEnabled( false );
                ptData->setEnabled( false );
                rbDiscountValue->setEnabled( false );
                rbDiscountPercent->setEnabled( false );
                ledDiscount->setEnabled( false );
                poBtnSave->setEnabled( false );
            }
        }
        if( m_poDoctor->id() > 0 )
            checkIndependent->setEnabled( false );
    }
}

cDlgDoctorEdit::~cDlgDoctorEdit()
{
}

void cDlgDoctorEdit::accept ()
{
    bool  boCanBeSaved = true;

    /*if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Warning" ), tr( "Advisor name cannot be empty." ) );
    }*/
    if( (ledLicence->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Warning" ), tr( "Advisor's licence cannot be empty." ) );
    }
    if( cmbDoctorType->currentIndex() < 1 )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Warning" ), tr( "The advisor must be member one of the groups." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( checkIndependent->isChecked() )
            {
                m_poDoctor->setLicenceId( 0 );
            }
            else
            {
                m_poDoctor->setLicenceId( g_poPrefs->getLicenceId() );
            }
            m_poDoctor->setName( ledName->text() );
            m_poDoctor->setLicence( ledLicence->text() );
            m_poDoctor->setDoctorTypeId( cmbDoctorType->itemData( cmbDoctorType->currentIndex() ).toUInt() );
            m_poDoctor->setData( ptData->toPlainText() );
            if( rbDiscountValue->isChecked() ) m_poDoctor->setDiscountType( 1 );
            else if ( rbDiscountPercent->isChecked() ) m_poDoctor->setDiscountType( 2 );
            m_poDoctor->setDiscount( ledDiscount->text().toInt() );

            m_poDoctor->setActive( true );
            m_poDoctor->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}
