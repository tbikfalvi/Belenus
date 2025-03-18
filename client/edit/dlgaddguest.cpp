//===========================================================================================================

#include <QPushButton>
#include <QMessageBox>
#include <ctime>

//===========================================================================================================

#include "dlgaddguest.h"
#include "../db/dbguest.h"

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgAddGuest::cDlgAddGuest( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    m_poPatient     = new cDBGuest;
    m_bDlgLoaded    = false;

    setWindowTitle( tr( "Add patient to database" ) );
    setWindowIcon( QIcon("./resources/40x40_patient.png") );
    pbAdd->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    m_poPatient->createNew();

    connect( ledName, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledEmail, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbGenderMale, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbGenderFemale, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );
    connect( rbGenderNone, SIGNAL(toggled(bool)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledEmail, SIGNAL(editingFinished()), this, SLOT(slotEmailEdited()) );

    slotEnableButtons();
    slotRefreshWarningColors();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "AddGuest", QPoint(550,135) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bDlgLoaded = true;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
cDlgAddGuest::~cDlgAddGuest()
{
    g_poPrefs->setDialogSize( "AddUnits", QPoint( width(), height() ) );

    if( m_poPatient ) delete m_poPatient;
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddGuest::slotRefreshWarningColors()
{
    lblName->setStyleSheet( "QLabel {font: normal;}" );
    lblGender->setStyleSheet( "QLabel {font: normal;}" );
    lblEmail->setStyleSheet( "QLabel {font: normal;}" );

    if( ledName->text().length() == 0 )
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() && !rbGenderNone->isChecked() )
        lblGender->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( !_isEmailFormatOk() && !ledEmail->text().isEmpty() )
    {
        lblEmail->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
    else if( ledEmail->text().length() == 0 )
    {
        lblEmail->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddGuest::slotEnableButtons()
{
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddGuest::on_pbAdd_clicked()
{
    bool    boCanBeSaved = true;
    QString qsErrorMessage = "";

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Guest name cannot be empty." ) );
    }
    if( !rbGenderMale->isChecked() && !rbGenderFemale->isChecked() && !rbGenderNone->isChecked() )
    {
        boCanBeSaved = false;
        if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
        qsErrorMessage.append( tr( "Gender of the guest must be set." ) );
    }
    if( ledEmail->text().length() > 0 )
    {
        if( !_isEmailFormatOk() )
        {
            boCanBeSaved = false;
            if( qsErrorMessage.length() ) qsErrorMessage.append( "\n\n" );
            qsErrorMessage.append( tr( "Email format is incorrect." ) );
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poPatient->load( ledName->text(), ledEmail->text() );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }

        if( m_poPatient->id() == 0 )
        {
            if( _saveGuestData() )
            {
                g_obGen.showTrayInfo( tr( "Patient '%1' saved in the database" ).arg( ledName->text() ) );
                QDialog::accept();
            }
        }
        else
        {
            g_obGen.showTrayInfo( tr( "Patient '%1' with %2 email address found in the database" ).arg( ledName->text() ).arg( ledEmail->text() ) );
            QDialog::accept();
        }
    }
    else
    {
        QMessageBox::warning( this, tr( "Warning" ), qsErrorMessage );
    }
}
//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
void cDlgAddGuest::on_pbCancel_clicked()
{
    QDialog::reject();
}

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
unsigned int cDlgAddGuest::patientId()
{
    return m_poPatient->id();
}

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
bool cDlgAddGuest::_isEmailFormatOk()
{
    bool    bRet = true;

    // ^[a-z0-9!#$%&\'*+\=?^_`{|}~-]+(?:\.[a-z0-9!#$%&\'*+\=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?
    QRegExp qreEmail( "^[a-z0-9!#$%&\\'*+\\=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&\\'*+\\=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?" );

    if( !qreEmail.exactMatch( ledEmail->text() ) )
    {
        bRet = false;
    }

    return bRet;
}

//===========================================================================================================
//
//-----------------------------------------------------------------------------------------------------------
bool cDlgAddGuest::_saveGuestData()
{
    bool bRet = false;

    try
    {
        m_poPatient->setLicenceId( g_poPrefs->getLicenceId() );
        m_poPatient->setName( ledName->text() );
        if( rbGenderMale->isChecked() )
            m_poPatient->setGender( 1 );
        else if( rbGenderFemale->isChecked() )
            m_poPatient->setGender( 2 );
        else if( rbGenderNone->isChecked() )
            m_poPatient->setGender( 3 );
        m_poPatient->setEmail( ledEmail->text() );
        if( !ledEmail->text().isEmpty() )
        {
            m_poPatient->setIsNewsletter( 1 );
            m_poPatient->setIsCardMail( 1 );
        }

        m_poPatient->save();

        bRet = true;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return bRet;
}

