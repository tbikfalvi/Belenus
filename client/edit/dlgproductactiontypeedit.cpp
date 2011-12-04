#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgproductactiontypeedit.h"


cDlgProductActionTypeEdit::cDlgProductActionTypeEdit( QWidget *p_poParent, cDBProductActionType *p_poProductActionType )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Product type" ) );
    setWindowIcon( QIcon("./resources/40x40_productactiontype.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    m_poProductActionType = p_poProductActionType;

    if( m_poProductActionType )
    {
        ledName->setText( m_poProductActionType->name() );
        chkIncrease->setChecked( m_poProductActionType->increaseProductCount() );
        chkDecrease->setChecked( m_poProductActionType->decreaseProductCount() );

        if( m_poProductActionType->licenceId() == 0 && m_poProductActionType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poProductActionType->licenceId() == 0 && m_poProductActionType->id() > 0 )
            {
                ledName->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poProductActionType->id() > 0 )
            checkIndependent->setEnabled( false );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditProductActionType", QPoint(390,180) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgProductActionTypeEdit::~cDlgProductActionTypeEdit()
{
    g_poPrefs->setDialogSize( "EditProductActionType", QPoint( width(), height() ) );
}

void cDlgProductActionTypeEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of productaction type must be set." ) );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productActionType WHERE productActionTypeId<>%1 AND name=\"%2\" AND active=1 AND archive<>\"DEL\"" ).arg(m_poProductActionType->id()).arg(ledName->text()) );
        if( poQuery->first() )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Productaction type with this name already exists.\nPlease set another one." ) );
        }
        if( poQuery ) delete poQuery;
    }
    if( chkIncrease->isChecked() && chkDecrease->isChecked() )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "A product action can not increase and decrease\n"
                                                        "the product count at the same time." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poProductActionType->setName( ledName->text() );
            m_poProductActionType->setIncreaseProductCount( chkIncrease->isChecked() );
            m_poProductActionType->setDecreaseProductCount( chkDecrease->isChecked() );
            m_poProductActionType->setActive( true );

            if( checkIndependent->isChecked() )
            {
                m_poProductActionType->setLicenceId( 0 );
            }
            else
            {
                m_poProductActionType->setLicenceId( g_poPrefs->getLicenceId() );
            }

            m_poProductActionType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgProductActionTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}


void cDlgProductActionTypeEdit::on_chkIncrease_toggled(bool checked)
{
    if( checked && chkDecrease->isChecked() )
        chkDecrease->setChecked( !checked );
}

void cDlgProductActionTypeEdit::on_chkDecrease_toggled(bool checked)
{
    if( checked && chkIncrease->isChecked() )
        chkIncrease->setChecked( !checked );
}
