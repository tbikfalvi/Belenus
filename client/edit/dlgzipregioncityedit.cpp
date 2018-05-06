#include <QPushButton>
#include <QMessageBox>
#include <iostream>

#include "dlgzipregioncityedit.h"

cDlgZipRegionCityEdit::cDlgZipRegionCityEdit( QWidget *p_poParent, cDBZipRegionCity *p_poZipRegionCity )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "ZipRegionCity" ) );
    setWindowIcon( QIcon("./resources/40x40_address.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    QSqlQuery *poQuery = NULL;
    poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
    while( poQuery->next() )
    {
        cmbRegion->addItem( poQuery->value( 0 ).toString() );
    }
    if( poQuery ) delete poQuery;

    m_poZipRegionCity = p_poZipRegionCity;
    if( m_poZipRegionCity )
    {
        cmbRegion->setEditText( m_poZipRegionCity->region() );
        ledZip->setText( m_poZipRegionCity->zip() );
        ledCity->setText( m_poZipRegionCity->city() );
    }
}

cDlgZipRegionCityEdit::~cDlgZipRegionCityEdit()
{
}

void cDlgZipRegionCityEdit::accept ()
{
    cDBZipRegionCity    obDBCheck;
    bool                boCanBeSaved = true;

    if( (cmbRegion->currentText() == "") && (ledZip->text() == "") && (ledCity->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Error" ), tr( "One of the fields must be filled in with valid data." ) );
    }

    if( ledZip->text().length() != g_poPrefs->getZipLength() )
    {
        boCanBeSaved = false;
        QMessageBox::warning( this, tr( "Error" ), tr( "Invalid zip code. Zip code must be %1 character length." ).arg(g_poPrefs->getZipLength()) );
        ledZip->setFocus();
    }

    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        try
        {
            obDBCheck.load( ledZip->text() );

            if( obDBCheck.id() != m_poZipRegionCity->id() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The defined zip code already saved into the database with\n\n"
                                              "Region : %1\n"
                                              "City : %2\n\n"
                                              "Do you want to save the defined region and city anyway?").arg(obDBCheck.region()).arg(obDBCheck.city()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
            }
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
        }
    }

    if( ledCity->text().length() > 0 )
    {
        try
        {
            obDBCheck.loadCity( ledCity->text() );

            if( cmbRegion->currentText().compare( obDBCheck.region() ) != 0 )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The city already assigned to a different region\n\n"
                                              "Region : %1\n\n"
                                              "Do you want to save the defined region and city anyway?").arg(obDBCheck.region()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::No )
                {
                    return;
                }
            }
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poZipRegionCity->setLicenceId( g_poPrefs->getLicenceId() );
            m_poZipRegionCity->setRegion( cmbRegion->currentText() );
            m_poZipRegionCity->setZip( ledZip->text() );
            m_poZipRegionCity->setCity( ledCity->text() );
            m_poZipRegionCity->setActive( true );
            m_poZipRegionCity->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }

        QDialog::accept();
    }
}

void cDlgZipRegionCityEdit::on_cmbRegion_editTextChanged(QString )
{

}
