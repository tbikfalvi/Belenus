#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardtypeedit.h"

cDlgPatientCardTypeEdit::cDlgPatientCardTypeEdit( QWidget *p_poParent, cDBPatientCardType *p_poPatientCardType )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Patient card type" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcardtype.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbAdd->setIcon( QIcon("./resources/40x40_new.png") );
    pbEdit->setIcon( QIcon("./resources/40x40_edit.png") );
    pbDelete->setIcon( QIcon("./resources/40x40_delete.png") );

    deValidDateFrom->setDate( QDate(2000,1,1) );
    deValidDateTo->setDate( QDate(2000,1,1) );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPatientCardType", QPoint(420,245) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_poPatientCardType = p_poPatientCardType;

    if( m_poPatientCardType )
    {
        ledName->setText( m_poPatientCardType->name() );
        ledPrice->setText( QString::number(m_poPatientCardType->price()) );
        ledVatpercent->setText( QString::number(m_poPatientCardType->vatpercent()) );
        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        ledUnitTime->setText( QString::number(m_poPatientCardType->unitTime()) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCardType->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        ledValidDays->setText( QString::number(m_poPatientCardType->validDays()) );
        chkMonday->setChecked( m_poPatientCardType->validWeekDays() & 1 );
        chkTuesday->setChecked( m_poPatientCardType->validWeekDays() & 2 );
        chkWednesday->setChecked( m_poPatientCardType->validWeekDays() & 4 );
        chkThursday->setChecked( m_poPatientCardType->validWeekDays() & 8 );
        chkFriday->setChecked( m_poPatientCardType->validWeekDays() & 16 );
        chkSaturday->setChecked( m_poPatientCardType->validWeekDays() & 32 );
        chkSunday->setChecked( m_poPatientCardType->validWeekDays() & 64 );
        if( m_poPatientCardType->id() == 0 )
        {
            listValidInterval->addItem( "00:00 => 24:00" );
        }

        if( m_poPatientCardType->validDays() < 1 )
        {
            rbInterval->setChecked( true );
            deValidDateFrom->setEnabled( true );
            deValidDateTo->setEnabled( true );
            ledValidDays->setEnabled( false );
        }
        else
        {
            rbDays->setChecked( true );
            deValidDateFrom->setEnabled( false );
            deValidDateTo->setEnabled( false );
            ledValidDays->setEnabled( true );
        }

        if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            checkIndependent->setChecked( true );

        if( !g_obUser.isInGroup( cAccessGroup::ROOT ) && !g_obUser.isInGroup( cAccessGroup::SYSTEM ) )
        {
            checkIndependent->setEnabled( false );
            if( m_poPatientCardType->licenceId() == 0 && m_poPatientCardType->id() > 0 )
            {
                ledName->setEnabled( false );
                ledPrice->setEnabled( false );
                ledVatpercent->setEnabled( false );
                ledUnits->setEnabled( false );
                ledUnitTime->setEnabled( false );
                rbInterval->setEnabled( false );
                deValidDateFrom->setEnabled( false );
                deValidDateTo->setEnabled( false );
                rbDays->setEnabled( false );
                ledValidDays->setEnabled( false );
                pbSave->setEnabled( false );
            }
        }
        if( m_poPatientCardType->id() > 0 )
            checkIndependent->setEnabled( false );
    }
}

cDlgPatientCardTypeEdit::~cDlgPatientCardTypeEdit()
{
    g_poPrefs->setDialogSize( "EditPatientCardType", QPoint( width(), height() ) );
}

void cDlgPatientCardTypeEdit::on_rbInterval_toggled(bool checked)
{
    if( checked )
    {
        deValidDateFrom->setEnabled( true );
        deValidDateTo->setEnabled( true );
        ledValidDays->setEnabled( false );
    }
    else
    {
        deValidDateFrom->setEnabled( false );
        deValidDateTo->setEnabled( false );
        ledValidDays->setEnabled( true );
    }
}

void cDlgPatientCardTypeEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    lblName->setStyleSheet( "QLabel {font: normal;}" );
    lblPrice->setStyleSheet( "QLabel {font: normal;}" );
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblUnitTime->setStyleSheet( "QLabel {font: normal;}" );
    rbInterval->setStyleSheet( "QRadioButton {font: normal;}" );
    rbDays->setStyleSheet( "QRadioButton {font: normal;}" );
    chkMonday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkTuesday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkWednesday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkThursday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkFriday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkSaturday->setStyleSheet( "QCheckBox {font: normal;}" );
    chkSunday->setStyleSheet( "QCheckBox {font: normal;}" );

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        qsErrorMessage.append( tr( "Name of patientcard type must be set." ) );
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( ledPrice->text() == "" )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Price of patientcard type must be set." ) );
        lblPrice->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( ledVatpercent->text() == "" )
        ledVatpercent->setText( "0" );
    if( ledUnits->text() == "" || ledUnits->text().toInt() < 1 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Number of units of patientcard type must be set." ) );
        lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( ledUnitTime->text() == "" || ledUnitTime->text().toInt() < 1 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Unittime of patientcard type must be set." ) );
        lblUnitTime->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( rbDays->isChecked() && (ledValidDays->text() == "" || ledValidDays->text().toInt() < 1) )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Number of validation days of patientcard type must be set." ) );
        rbDays->setStyleSheet( "QRadioButton {font: bold; color: red;}" );
    }
    else if( rbInterval->isChecked() && deValidDateTo->date() <= QDate::currentDate() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Invalid end date. End date must be in the future." ) );
        rbInterval->setStyleSheet( "QRadioButton {font: bold; color: red;}" );
    }
    else if( rbInterval->isChecked() && deValidDateFrom->date() > deValidDateTo->date() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "FROM date must be before TO date." ) );
        rbInterval->setStyleSheet( "QRadioButton {font: bold; color: red;}" );
    }

    if( !chkMonday->isChecked() &&
        !chkTuesday->isChecked() &&
        !chkWednesday->isChecked() &&
        !chkThursday->isChecked() &&
        !chkFriday->isChecked() &&
        !chkSaturday->isChecked() &&
        !chkSunday->isChecked() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "One of the weekdays must be selected." ) );
        chkMonday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkTuesday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkWednesday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkThursday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkFriday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkSaturday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
        chkSunday->setStyleSheet( "QCheckBox {font: bold; color: red;;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            int nWeekDays = 0;
            if( chkMonday->isChecked() )    nWeekDays |= 1;
            if( chkTuesday->isChecked() )   nWeekDays |= 2;
            if( chkWednesday->isChecked() ) nWeekDays |= 4;
            if( chkThursday->isChecked() )  nWeekDays |= 8;
            if( chkFriday->isChecked() )    nWeekDays |= 16;
            if( chkSaturday->isChecked() )  nWeekDays |= 32;
            if( chkSunday->isChecked() )    nWeekDays |= 64;

            m_poPatientCardType->setName( ledName->text() );
            m_poPatientCardType->setPrice( ledPrice->text().toUInt() );
            m_poPatientCardType->setVatpercent( ledVatpercent->text().toInt() );
            m_poPatientCardType->setUnits( ledUnits->text().toUInt() );
            m_poPatientCardType->setUnitTime( ledUnitTime->text().toUInt() );
            m_poPatientCardType->setValidDateFrom( (rbDays->isChecked()?"2000-01-01":deValidDateFrom->date().toString("yyyy-MM-dd")) );
            m_poPatientCardType->setValidDateTo( (rbDays->isChecked()?"2000-01-01":deValidDateTo->date().toString("yyyy-MM-dd")) );
            m_poPatientCardType->setValidDays( ledValidDays->text().toUInt() );
            m_poPatientCardType->setValidWeekDays( nWeekDays );
            m_poPatientCardType->setActive( true );

            if( checkIndependent->isChecked() )
            {
                m_poPatientCardType->setLicenceId( 0 );
            }
            else
            {
                m_poPatientCardType->setLicenceId( g_poPrefs->getLicenceId() );
            }

            m_poPatientCardType->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), qsErrorMessage );
    }
}

void cDlgPatientCardTypeEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgPatientCardTypeEdit::on_pbAdd_clicked()
{

}

void cDlgPatientCardTypeEdit::on_pbEdit_clicked()
{

}

void cDlgPatientCardTypeEdit::on_pbDelete_clicked()
{
    listValidInterval->takeItem( listValidInterval->currentRow() );
}

void cDlgPatientCardTypeEdit::on_listValidInterval_currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
    pbEdit->setEnabled( current != NULL ? true : false );
    pbDelete->setEnabled( current != NULL ? true : false );
}
