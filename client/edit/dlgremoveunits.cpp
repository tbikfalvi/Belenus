//===========================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//===========================================================================================================
//
// Filename    : dlgremoveunits.cpp
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

#include "dlgremoveunits.h"
#include "../db/dbpatientcardtype.h"
#include "../db/dbpatientcardunits.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgRemoveUnits::cDlgRemoveUnits( QWidget *p_poParent, cDBPatientCard *p_poPatientCard ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatientCard     = p_poPatientCard;
    m_poPatientCardType = new cDBPatientCardType;
    m_bDlgLoaded        = false;

    m_nUnitMax          = 1;

    setWindowTitle( tr( "Remove units from patientcard" ) );
    setWindowIcon( QIcon("./resources/40x40_patientcard.png") );
    pbRemove->setIcon( QIcon("./resources/40x40_patientcard.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    sbUnits->setMinimum( 1 );
    sbUnits->setMaximum( m_nUnitMax );
    sbUnits->setValue( 1 );

    int nCount = 0;

    if( m_poPatientCard )
    {
        QSqlQuery *poQuery;

        ledBarcode->setText( m_poPatientCard->barcode() );

        poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardTypes.patientCardTypeId, COUNT(name), name, patientcardunits.validDateTo "
                                                   "FROM patientcardunits, patientCardTypes "
                                                   "WHERE "
                                                   "patientcardunits.patientCardId=%1 AND "
                                                   "patientCardTypes.patientCardTypeId = patientcardunits.patientCardTypeId AND "
                                                   "patientcardunits.active=1 "
                                                   "GROUP BY name, patientcardunits.validDateTo ORDER BY name " ).arg( m_poPatientCard->id() ) );
        nCount = poQuery->size();

        if( nCount > 0 )
        {
            while( poQuery->next() )
            {
                cmbCardType->addItem( tr( "%1 (Valid unitl: %2)" ).arg( poQuery->value( 2 ).toString() ).arg( poQuery->value( 3 ).toString() ), poQuery->value( 0 ) );
                m_qslList.append( QString( "%1" ).arg( poQuery->value( 1 ).toInt() ) );
            }
            cmbCardType->setCurrentIndex( 0 );
        }
    }

    if( nCount > 0 )
    {
        connect( cmbCardType, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateUnitCount()) );

        slotEnableButtons();
        slotUpdateUnitCount();
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "RemoveUnits", QPoint(550,135) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    if( nCount < 1 )
    {
        sbUnits->setEnabled( false );
        cmbCardType->setEnabled( false );
        pbRemove->setEnabled( false );
    }

    m_bDlgLoaded = true;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgRemoveUnits::~cDlgRemoveUnits()
{
    g_poPrefs->setDialogSize( "RemoveUnits", QPoint( width(), height() ) );

    if( m_poPatientCardType ) delete m_poPatientCardType;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgRemoveUnits::slotUpdateUnitCount()
{
    m_nUnitMax = m_qslList.at( cmbCardType->currentIndex() ).toInt();

    sbUnits->setMaximum( m_nUnitMax );
    sbUnits->setValue( 1 );
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgRemoveUnits::slotEnableButtons()
{
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgRemoveUnits::on_pbRemove_clicked()
{
    int             nCountUnits         = sbUnits->value();
    unsigned int    uiPatientCardTypeId = cmbCardType->itemData( cmbCardType->currentIndex() ).toInt();
    unsigned int    uiPatientCardUnitId;

    QSqlQuery *poQuery;

    try
    {
        for( int i=0; i<nCountUnits; i++ )
        {
            poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardUnits "
                                                       "WHERE "
                                                       "patientCardId = %1 AND "
                                                       "patientCardTypeId = %2 AND "
                                                       "active=1" ).arg( m_poPatientCard->id() ).arg( uiPatientCardTypeId ) );
            poQuery->first();

            uiPatientCardUnitId = poQuery->value( 0 ).toUInt();

            QString  qsQuery;

            qsQuery = "UPDATE patientCardUnits SET ";
            qsQuery += QString( "dateTimeUsed = NOW(), " );
            qsQuery += QString( "active = 0, " );
            qsQuery += QString( "archive = \"MOD\" " );
            qsQuery += QString( "WHERE patientCardUnitId = %1" ).arg( uiPatientCardUnitId );

            g_poDB->executeQTQuery( qsQuery );
        }

        m_poPatientCard->sendDataToWeb();

        if( g_poPrefs->isCardyGoSync() )
        {
            g_obLogger(cSeverity::INFO) << "PatientCard units removed" << EOM;
            m_poPatientCard->sendAutoMail( AUTO_MAIL_ON_UNITCHANGE, AUTO_MAIL_DESTINATION_CARDY, QDate::currentDate().toString("yyyy-MM-dd"), 0, "" );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    QDialog::accept();
/*
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
*/
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgRemoveUnits::on_pbCancel_clicked()
{
    QDialog::reject();
}
