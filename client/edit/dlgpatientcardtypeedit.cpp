#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgpatientcardtypeedit.h"
#include "dlgvalidtimeperiodedit.h"
#include "../db/dbvalidtimeperiods.h"

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

    deValidDateFrom->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );
    deValidDateTo->setDisplayFormat( g_poPrefs->getDateFormat().replace("-",".") );

    checkIndependent->setVisible( false );
    checkIndependent->setEnabled( false );

    connect( ledVatpercent, SIGNAL(textChanged(QString)), this, SLOT(on_ledPrice_textChanged(QString)) );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditPatientCardType", QPoint(420,245) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_poPatientCardType = p_poPatientCardType;

    if( m_poPatientCardType )
    {
        ledName->setText( m_poPatientCardType->name() );
        ledPrice->setText( QString::number(m_poPatientCardType->price()/100) );
        ledVatpercent->setText( QString::number(m_poPatientCardType->vatpercent()) );
        ledUnits->setText( QString::number(m_poPatientCardType->units()) );
        ledUnitTime->setText( QString::number(m_poPatientCardType->unitTime()) );
        deValidDateFrom->setDate( QDate::fromString(m_poPatientCardType->validDateFrom(),"yyyy-MM-dd") );
        deValidDateTo->setDate( QDate::fromString(m_poPatientCardType->validDateTo(),"yyyy-MM-dd") );
        ledValidDays->setText( QString::number(m_poPatientCardType->validDays()) );
        if( m_poPatientCardType->id() == 0 )
        {
            listValidInterval->addItem( tr("00:00 => 23:59 Mon Tue Wed Thu Fri Sat Sun") );
        }
        else
        {
            cDBValidTimePeriod  obDBValidTimePeriod;

            QStringList qslPeriods = obDBValidTimePeriod.loadPeriods( m_poPatientCardType->id() );

            for( int i=0; i<qslPeriods.count(); i++ )
            {
                listValidInterval->addItem( qslPeriods.at(i) );
            }
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
    m_dlgProgress = new cDlgProgress( this );
}

cDlgPatientCardTypeEdit::~cDlgPatientCardTypeEdit()
{
    g_poPrefs->setDialogSize( "EditPatientCardType", QPoint( width(), height() ) );

    delete m_dlgProgress;
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
    lblTimeInterval->setStyleSheet( "QLabel {font: normal;}" );

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
    if( ledUnits->text() == "" )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "Number of units of patientcard type must be set." ) );
        lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else if( ledUnits->text().toInt() == 0 )
    {
        lblUnits->setStyleSheet( "QLabel {font: bold; color: blue;}" );
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

    if( listValidInterval->count() == 0 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n" );
        qsErrorMessage.append( tr( "One time period must be set." ) );
        lblTimeInterval->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            if( m_poPatientCardType->unitTime() > 0 &&
                m_poPatientCardType->unitTime() != ledUnitTime->text().toInt() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The unit time of this patientcard was different.\n"
                                              "Old unit time: %1\n"
                                              "New unit time: %2\n\n"
                                              "Do you want to update all of the units of all patientcard\n"
                                              "assigned to this patientcard type?").arg( m_poPatientCardType->unitTime() )
                                                                                   .arg( ledUnitTime->text().toUInt() ),
                                           QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
                {
                    m_dlgProgress->showProgress();
                    m_poPatientCardType->updatePatientCardUnits( ledUnitTime->text().toInt() );
                    m_dlgProgress->hideProgress();
                }
            }

            cCurrency currPrice( ledPrice->text(), cCurrency::CURR_GROSS, ledVatpercent->text().toInt() );

            m_poPatientCardType->setName( ledName->text() );
            m_poPatientCardType->setPrice( currPrice.currencyValue().toInt() );
            m_poPatientCardType->setVatpercent( ledVatpercent->text().toInt() );
            m_poPatientCardType->setUnits( ledUnits->text().toUInt() );
            m_poPatientCardType->setUnitTime( ledUnitTime->text().toInt() );
            m_poPatientCardType->setValidDateFrom( (rbDays->isChecked()?"2000-01-01":deValidDateFrom->date().toString("yyyy-MM-dd")) );
            m_poPatientCardType->setValidDateTo( (rbDays->isChecked()?"2000-01-01":deValidDateTo->date().toString("yyyy-MM-dd")) );
            m_poPatientCardType->setValidDays( ledValidDays->text().toUInt() );
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

            QStringList qslPeriods;

            for( int i=0; i<listValidInterval->count(); i++ )
            {
                qslPeriods << listValidInterval->item(i)->text();
            }

            cDBValidTimePeriod  obDBValidTimePeriod;

            obDBValidTimePeriod.setLicenceId( m_poPatientCardType->licenceId() );
            obDBValidTimePeriod.setPatientCardTypeId( m_poPatientCardType->id() );
            obDBValidTimePeriod.saveList( qslPeriods );
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
    cDlgValidTimePeriodEdit     obDlgValidTimePeriodEdit;

    if( obDlgValidTimePeriodEdit.exec() == QDialog::Accepted )
    {
        if( listValidInterval->findItems( obDlgValidTimePeriodEdit.validPeriod(), Qt::MatchFixedString ).count() > 0 )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The selected time period already present in the list.") );
        }
        else
        {
            listValidInterval->addItem( obDlgValidTimePeriodEdit.validPeriod() );
        }
    }
}

void cDlgPatientCardTypeEdit::on_pbEdit_clicked()
{
    cDlgValidTimePeriodEdit     obDlgValidTimePeriodEdit( this, listValidInterval->currentItem()->text() );

    if( obDlgValidTimePeriodEdit.exec() == QDialog::Accepted )
    {
        int nIndex = listValidInterval->currentRow();
        QListWidgetItem *lwItem = listValidInterval->takeItem( nIndex );

        if( listValidInterval->findItems( obDlgValidTimePeriodEdit.validPeriod(), Qt::MatchFixedString ).count() > 0 )
        {
            QMessageBox::warning( this, tr("Warning"),
                                  tr("The selected time period already present in the list.") );
            listValidInterval->insertItem( nIndex, lwItem );
        }
        else
        {
            listValidInterval->insertItem( nIndex, obDlgValidTimePeriodEdit.validPeriod() );
        }
        listValidInterval->setCurrentRow( nIndex );
    }
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

void cDlgPatientCardTypeEdit::on_ledPrice_textChanged(const QString &/*arg1*/)
{
    cCurrency currPrice( ledPrice->text(), cCurrency::CURR_GROSS, ledVatpercent->text().toInt() );

    lblPriceFull->setText( tr("(%1 + %2 \% VAT)").arg(currPrice.currencyStringSeparator( cCurrency::CURR_NET)).arg(ledVatpercent->text()) );
}

