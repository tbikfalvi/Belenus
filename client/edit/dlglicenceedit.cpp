#include <QPushButton>
#include <QMessageBox>

#include "dlglicenceedit.h"
#include "../db/dbzipregioncity.h"

dlgLicenceEdit::dlgLicenceEdit( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Address" ) );
    setWindowIcon( QIcon("./resources/40x40_address.png") );
    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    QSqlQuery *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT region FROM zipRegionCity WHERE active=1 GROUP BY region ORDER BY region" ) );
    while( poQuery->next() )
    {
        cmbRegion->addItem( poQuery->value( 0 ).toString() );
    }
    cmbRegion->setEditText( "" );

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM licences ORDER BY licenceId DESC" ) );
    poQuery->first();

    m_uiId = poQuery->value( 0 ).toInt();

    ledLicenceKey->setText( poQuery->value( 1 ).toString() );
    ledName->setText( poQuery->value( 7 ).toString() );
    if( g_poPrefs->getDefaultCountry().length() > 0 && poQuery->value( 2 ).toString().length() < 1 )
    {
        ledCountry->setText( g_poPrefs->getDefaultCountry() );
    }
    else
    {
        ledCountry->setText( poQuery->value( 2 ).toString() );
    }
    cmbRegion->setEditText( poQuery->value( 3 ).toString() );
    ledZip->setText( poQuery->value( 5 ).toString() );
    ledCity->setText( poQuery->value( 4 ).toString() );
    ledAddress->setText( poQuery->value( 6 ).toString() );
    ledContactName->setText( poQuery->value( 8 ).toString() );

    if( m_uiId < 2 )
    {
        ledName->setEnabled( false );
        ledCountry->setEnabled( false );
        cmbRegion->setEnabled( false );
        ledZip->setEnabled( false );
        ledCity->setEnabled( false );
        ledAddress->setEnabled( false );
        ledContactName->setEnabled( false );
        pbSave->setEnabled( false );
    }
}

dlgLicenceEdit::~dlgLicenceEdit()
{
}

void dlgLicenceEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledCountry->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Address cannot be empty." ) );
    }

    checkRegionZipCity();

    if( boCanBeSaved )
    {
        try
        {
            QString qsQuery = "UPDATE licences SET ";

            qsQuery += QString( "country = \"%1\", " ).arg( ledCountry->text() );
            qsQuery += QString( "region = \"%1\", " ).arg( cmbRegion->currentText() );
            qsQuery += QString( "city = \"%1\", " ).arg( ledCity->text() );
            qsQuery += QString( "zip = \"%1\", " ).arg( ledZip->text() );
            qsQuery += QString( "address = \"%1\", " ).arg( ledAddress->text() );
            qsQuery += QString( "studio = \"%1\", " ).arg( ledName->text() );
            qsQuery += QString( "contact = \"%1\" " ).arg( ledContactName->text() );

            qsQuery += QString( " WHERE licenceId = %1" ).arg( m_uiId );

            QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
            if( poQuery ) delete poQuery;

//            g_obDBMirror.updateLicenceData();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }

        QDialog::accept();
    }
}

void dlgLicenceEdit::on_pbCitySearch_clicked()
{
    cDBZipRegionCity    obDBZipRegionCity;

    try
    {
        obDBZipRegionCity.loadCity( ledCity->text() );
        ledZip->setText( obDBZipRegionCity.zip() );
        ledCity->setText( obDBZipRegionCity.city() );
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("ZipRegionCity city not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
        else
        {
            QMessageBox::information( this, tr("Information"),
                                      tr("There is no city in database like\n\n\"%1\"").arg(ledCity->text()) );
            ledZip->setText( "" );
        }
    }
}

void dlgLicenceEdit::on_ledZip_textEdited(QString )
{
    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        cDBZipRegionCity    obDBZipRegionCity;

        try
        {
            obDBZipRegionCity.load( ledZip->text() );
            ledCity->setText( obDBZipRegionCity.city() );
            cmbRegion->setEditText( obDBZipRegionCity.region() );
        }
        catch( cSevException &e )
        {
            if( QString(e.what()).compare("ZipRegionCity zip not found") != 0 )
            {
                g_obLogger(e.severity()) << e.what() << EOM;
                g_obGen.showTrayError( e.what() );
            }
            else
            {
                ledCity->setText( "" );
            }
        }
    }
}

void dlgLicenceEdit::checkRegionZipCity()
{
    cDBZipRegionCity    obDBCheck;

    if( ledZip->text().length() == g_poPrefs->getZipLength() )
    {
        try
        {
            obDBCheck.load( ledZip->text() );

            if( cmbRegion->currentText() != obDBCheck.region() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The defined zip code already saved into the database with\n\n"
                                              "Region : %1\n\n"
                                              "Do you want to overwrite the region entered with this one?").arg(obDBCheck.region()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    cmbRegion->setEditText( obDBCheck.region() );
                }
            }
            if( ledCity->text() != obDBCheck.city() )
            {
                if( QMessageBox::question( this, tr("Question"),
                                           tr("The defined zip code already saved into the database with\n\n"
                                              "City : %1\n\n"
                                              "Do you want to overwrite the city entered with this one?").arg(obDBCheck.city()),
                                           QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                {
                    ledCity->setText( obDBCheck.city() );
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
            else
            {
                try
                {
                    obDBCheck.loadCity( ledCity->text() );

                    if( cmbRegion->currentText() != obDBCheck.region() )
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined city already assigned to a different region\n\n"
                                                      "Region : %1\n\n"
                                                      "Do you want to correct the defined region?").arg(obDBCheck.region()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            cmbRegion->setEditText( obDBCheck.region() );
                        }
                    }

                    if( ledZip->text() != obDBCheck.zip() )
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined city already assigned to a different zip code\n\n"
                                                      "Zip : %1\n\n"
                                                      "Do you save the following data\n\n"
                                                      "Region : %2\n"
                                                      "Zip : %3\n"
                                                      "City : %4\n\n"
                                                      "as new into the database?").arg(obDBCheck.zip()).arg(cmbRegion->currentText()).arg(ledZip->text()).arg(ledCity->text()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            obDBCheck.createNew();
                            obDBCheck.setLicenceId( g_poPrefs->getLicenceId() );
                            obDBCheck.setRegion( cmbRegion->currentText() );
                            obDBCheck.setZip( ledZip->text() );
                            obDBCheck.setCity( ledCity->text() );
                            obDBCheck.setActive( true );
                            obDBCheck.save();
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
                    else
                    {
                        if( QMessageBox::question( this, tr("Question"),
                                                   tr("The defined data did not found in the database\n\n"
                                                      "Region : %1\n"
                                                      "Zip : %2\n"
                                                      "City : %3\n\n"
                                                      "Do you want to save them as new into the database?").arg(cmbRegion->currentText()).arg(ledZip->text()).arg(ledCity->text()),
                                                   QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes )
                        {
                            obDBCheck.createNew();
                            obDBCheck.setLicenceId( g_poPrefs->getLicenceId() );
                            obDBCheck.setRegion( cmbRegion->currentText() );
                            obDBCheck.setZip( ledZip->text() );
                            obDBCheck.setCity( ledCity->text() );
                            obDBCheck.setActive( true );
                            obDBCheck.save();
                        }
                    }
                }
            }
        }
    }
}
