//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgaddunits.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//===========================================================================================================
// Berletek adatait kezelo ablak.
//===========================================================================================================

#include <QPushButton>
#include <QMessageBox>
#include <ctime>

//===========================================================================================================

#include "dlgaddunits.h"
#include "../db/dbpatientcardtype.h"
#include "../db/dbpatientcardunits.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgAddUnits::cDlgAddUnits( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;
    m_bDlgLoaded        = false;

    setWindowTitle( tr( "Add units to patientcard" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );
    pbAdd->setIcon( QIcon("./resources/40x40_patientcard.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );

        cmbCardType->addItem( tr("<Not selected>"), 0 );
        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypeId, name FROM patientCardTypes WHERE active=1 AND archive<>\"DEL\" ORDER BY name " ) );
        while( poQuery->next() )
        {
            if( poQuery->value(0) == 1 )
                continue;

            cmbCardType->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
        }
        cmbCardType->setCurrentIndex( 0 );
        deValidDateTo->setDate( QDate::currentDate() );
    }

    connect( ledUnits, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( cmbCardType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotRefreshWarningColors()) );
    connect( deValidDateTo, SIGNAL(dateChanged(QDate)), this, SLOT(slotRefreshWarningColors()) );

    slotEnableButtons();
    slotRefreshWarningColors();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "AddUnits", QPoint(550,135) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bDlgLoaded = true;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgAddUnits::~cDlgAddUnits()
{
    g_poPrefs->setDialogSize( "AddUnits", QPoint( width(), height() ) );

    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddUnits::slotRefreshWarningColors()
{
    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( ledUnits->text().toInt() < 1 )
    {
        lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( cmbCardType->currentIndex() < 1 )
    {
        lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    if( deValidDateTo->date() <= QDate::currentDate() )
    {
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddUnits::slotEnableButtons()
{
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddUnits::on_pbAdd_clicked()
{
    bool  boCanBeSaved = true;

    QString qsErrorMessage = "";

    lblUnits->setStyleSheet( "QLabel {font: normal;}" );
    lblCardType->setStyleSheet( "QLabel {font: normal;}" );
    lblValidDate->setStyleSheet( "QLabel {font: normal;}" );

    if( ledUnits->text().toInt() < 1 )
    {
        boCanBeSaved = false;
        qsErrorMessage.append( tr( "Number of units must be greater than zero." ) );
        lblUnits->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( cmbCardType->currentIndex() == 0 )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Invalid Patientcard type.\nYou must select a valid patientcard type." ) );
        lblCardType->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( deValidDateTo->date() <= QDate::currentDate() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Incorrect expiration date." ) );
        lblValidDate->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatientCardType->createNew();

            if( cmbCardType->currentIndex() > -1 )
            {
                m_poPatientCardType->load( cmbCardType->itemData( cmbCardType->currentIndex() ).toInt() );
            }

            cDBPatientcardUnit  obDBPatientcardUnit;
            QStringList         qslUnitIds;

            for( int i=0; i<ledUnits->text().toInt(); i++ )
            {
                obDBPatientcardUnit.createNew();
                obDBPatientcardUnit.setLicenceId( m_poPatientCard->licenceId() );
                obDBPatientcardUnit.setPatientCardId( m_poPatientCard->id() );
                obDBPatientcardUnit.setPatientCardTypeId( m_poPatientCardType->id() );
                obDBPatientcardUnit.setLedgerId( 0 );
                obDBPatientcardUnit.setUnitTime( m_poPatientCardType->unitTime() );
                obDBPatientcardUnit.setUnitPrice( m_poPatientCardType->price()/ledUnits->text().toInt() );
                obDBPatientcardUnit.setValidDateFrom( m_poPatientCard->validDateFrom() );
                obDBPatientcardUnit.setValidDateTo( m_poPatientCard->validDateTo() );
                obDBPatientcardUnit.setDateTime( "" );
                obDBPatientcardUnit.setActive( true );
                obDBPatientcardUnit.save();
                qslUnitIds << QString::number( obDBPatientcardUnit.id() );
            }

            g_obLogger(cSeverity::INFO) << "Patientcard ["
                                        << m_poPatientCard->barcode()
                                        << "] filled by sysadmin with type ["
                                        << m_poPatientCardType->name()
                                        << "] units ["
                                        << ledUnits->text().toInt()
                                        << "]"
                                        << EOM;

            m_poPatientCard->synchronizeUnits();
            m_poPatientCard->synchronizeTime();
            m_poPatientCard->save();

            m_poPatientCard->sendDataToWeb();

            QDialog::accept();

        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
    else
    {
        QMessageBox::critical( this, tr( "Error" ), qsErrorMessage );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddUnits::on_pbCancel_clicked()
{
    QDialog::reject();
}
