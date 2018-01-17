#include <QPushButton>
#include <QMessageBox>
#include <ctime>

#include "dlgdistlistedit.h"
#include "../db/dbguest.h"


cDlgDistListEdit::cDlgDistListEdit( QWidget *p_poParent, cDBDistList *p_poDistList )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Distribution list" ) );
    setWindowIcon( QIcon("./resources/40x40_distlist.png") );

    pbSave->setIcon(        QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon(      QIcon("./resources/40x40_cancel.png") );

    m_poDistList = p_poDistList;

    QStringList qslPatients;

    if( m_poDistList->id() > 0 )
    {
        qslPatients = m_poDistList->patients();
    }

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientId, name, email FROM patients WHERE active=1 AND archive<>\"DEL\" AND email!=\"\" " ) );
    while( poQuery->next() )
    {
        if( qslPatients.contains( poQuery->value( 0 ).toString() ) )
        {
            listPatientsAssigned->addItem( QString("%1 (%2)").arg( poQuery->value( 1 ).toString() )
                                                             .arg( poQuery->value( 2 ).toString() ));
        }
        else
        {
            listPatientsIndependent->addItem( QString("%1 (%2)").arg( poQuery->value( 1 ).toString() )
                                                                .arg( poQuery->value( 2 ).toString() ));
        }
    }
    if( poQuery ) delete poQuery;

    if( m_poDistList )
    {
        ledName->setText( m_poDistList->name() );
        teDescription->setText( m_poDistList->description() );
    }

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditDistList", QPoint(740,480) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgDistListEdit::~cDlgDistListEdit()
{
    g_poPrefs->setDialogSize( "EditDistList", QPoint( width(), height() ) );
}

void cDlgDistListEdit::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( ledName->text() == "" )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Name of distribution list must be set." ) );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM distlist WHERE distlistId<>%1 AND name=\"%2\" AND active=1 AND archive<>\"DEL\"" ).arg(m_poDistList->id()).arg(ledName->text()) );
        if( poQuery->first() )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Distribution list with this name already exists.\nPlease set another one." ) );
        }
        if( poQuery ) delete poQuery;
    }

    if( boCanBeSaved )
    {
        QStringList     qslPatients;
        for( int i=0; i<listPatientsAssigned->count(); i++ )
        {
            QStringList qslPatient = listPatientsAssigned->item(i)->text().split(" (");
            QString     qsName = qslPatient.at(0);
            QString     qsMail = qslPatient.at(1).left( qslPatient.at(1).length()-1 );

            try
            {
                cDBGuest  poPatient;

                poPatient.load( qsName, qsMail );
                qslPatients.append( QString("%1").arg(poPatient.id()) );
            }
            catch( cSevException &e )
            {
                if( QString( e.what() ).compare("More than one guest found") == 0 )
                {
                    QString qsMessage = QString( "More than one record found in guest database with\n"
                                                 "Name: '%1'\n"
                                                 "E-mail address: '%2'\n\n"
                                                 "The application is not able ot identify which guest should be added to the distribution list.\n"
                                                 "Please first modify name or e-mail address to remove duplications.")
                                                .arg( qsName ).arg( qsMail );
                    QMessageBox::warning( this, tr( "Warning" ), qsMessage );
                    return;
                }
                else
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
                    return;
                }
            }
        }

        m_poDistList->setName( ledName->text() );
        m_poDistList->setDescription( teDescription->toPlainText() );
        m_poDistList->setPatients( qslPatients );
        m_poDistList->setActive( true );

        try
        {
            m_poDistList->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgDistListEdit::on_pbCancel_clicked()
{
    QDialog::reject();
}

void cDlgDistListEdit::on_pbPatientAdd_clicked()
{
    for( int i=listPatientsIndependent->count()-1; i>-1; i-- )
    {
        if( listPatientsIndependent->item(i)->isSelected() )
        {
            listPatientsAssigned->addItem( listPatientsIndependent->takeItem(i) );
        }
    }
}

void cDlgDistListEdit::on_pbPatientAddAll_clicked()
{
    for( int i=listPatientsIndependent->count()-1; i>-1; i-- )
    {
        listPatientsAssigned->addItem( listPatientsIndependent->takeItem(i) );
    }
}

void cDlgDistListEdit::on_pbPatientRemove_clicked()
{
    for( int i=listPatientsAssigned->count()-1; i>-1; i-- )
    {
        if( listPatientsAssigned->item(i)->isSelected() )
        {
            listPatientsIndependent->addItem( listPatientsAssigned->takeItem(i) );
        }
    }
}

void cDlgDistListEdit::on_pbPatientRemoveAll_clicked()
{
    for( int i=listPatientsAssigned->count()-1; i>-1; i-- )
    {
        listPatientsIndependent->addItem( listPatientsAssigned->takeItem(i) );
    }
}

