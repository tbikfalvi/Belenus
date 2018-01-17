
#include <QPushButton>
#include <QMessageBox>

#include "dlgsendmail.h"
#include "belenus.h"
#include "dlgselectmailrecipient.h"
#include "../db/dbdistlist.h"
#include "../db/dbguest.h"
#include "../db/dbsendmail.h"

dlgSendMail::dlgSendMail( QWidget *p_poParent, QString p_qsTitle ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( p_qsTitle );

    pbSend->setIcon( QIcon("./resources/40x40_send.png") );
    pbSave->setIcon( QIcon("./resources/40x40_save.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    connect( ledSubject, SIGNAL(textEdited(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( teMessageBody, SIGNAL(textChanged()), this, SLOT(slotRefreshWarningColors()) );

    slotRefreshWarningColors();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "SendMail", QPoint(270,70) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

dlgSendMail::~dlgSendMail()
{
    g_poPrefs->setDialogSize( "SendMail", QPoint( width(), height() ) );
}

void dlgSendMail::loadMail()
{
/*
    try
    {
        cDBSendMail obDBSendMail;

        obDBSendMail.load( p_uiMailId );

        teTo->setPlainText( obDBSendMail.recipients() );
        ledSubject->setText( obDBSendMail.subject() );
        teMessageBody->setPlainText( obDBSendMail.message() );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }
*/
}

//===========================================================================================================
//  slotRefreshWarningColors
//-----------------------------------------------------------------------------------------------------------
//  A dialog-ban talalhato label-ek adat-fuggo atszinezeseert felelos procedura
//-----------------------------------------------------------------------------------------------------------
void dlgSendMail::slotRefreshWarningColors()
{
    lblSubject->setStyleSheet( "QLabel {font: normal;}" );
    gbMessageBody->setStyleSheet( "QGroupBox {font: normal;}" );

    if( ledSubject->text().length() == 0 )
        lblSubject->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( teMessageBody->toPlainText().length() < 1 )
        gbMessageBody->setStyleSheet( "QGroupBox {font: bold; color: red;}" );
}

void dlgSendMail::on_pbSend_clicked()
{
    QStringList qslRecipients           = teTo->toPlainText().trimmed().split(";");
    QStringList qslMailRecipients       = QStringList();
    QStringList qslUnknownRecipients    = QStringList();

    if( teTo->toPlainText().trimmed().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no recipient for this message.") );
        return;
    }

    if( ledSubject->text().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no subject for this message.") );
        return;
    }

    if( teMessageBody->toPlainText().trimmed().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no text in the body of the message.") );
        return;
    }

    for( int i=0; i<qslRecipients.count(); i++ )
    {
        QString qsRecipient = qslRecipients.at( i ).trimmed();

        if( qsRecipient.left(1).compare("@") == 0 )
        {
            try
            {
                cDBDistList obDBDistList;

                obDBDistList.load( qsRecipient.right( qsRecipient.length()-1 ) );
                qslMailRecipients.append( obDBDistList.recipients() );
            }
            catch( cSevException &e )
            {
                if( QString( e.what() ).compare("Distribution list name not found") == 0 )
                {
                    qslUnknownRecipients.append( qsRecipient );
                }
                else
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
                    return;
                }
            }
        }
        else
        {
            QStringList qslPatient  = qsRecipient.trimmed().split(" (");
            QString     qsName      = qslPatient.at(0);
            QString     qsMail      = qslPatient.at(1).left( qslPatient.at(1).length()-1 );

            try
            {
                cDBGuest  poPatient;

                poPatient.load( qsName, qsMail );
                qslMailRecipients.append( poPatient.email() );
            }
            catch( cSevException &e )
            {
                if( QString( e.what() ).compare("More than one guest found") == 0 ||
                    QString( e.what() ).compare("Guest name/email not found") == 0 )
                {
                    qslUnknownRecipients.append( qsRecipient );
                }
                else
                {
                    g_obLogger(e.severity()) << e.what() << EOM;
                    return;
                }
            }
        }
    }

    if( qslUnknownRecipients.count() > 0 )
    {
        QString qsQuestion = tr( "The following addresses could not be found in database:\n"
                                 "\n%1\n\n"
                                 "Do you still want to send your mail?" ).arg( qslUnknownRecipients.join("\n") );

        if( QMessageBox::question( this, tr( "Question" ),
                                   qsQuestion,
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
        {
            return;
        }
    }

    if( qslMailRecipients.count() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("There is no valid recipient for this message.") );
        return;
    }

    try
    {
        cDBSendMail obDBSendMail;

        obDBSendMail.createNew();
        obDBSendMail.setLicenceId( g_poPrefs->getLicenceId() );
        obDBSendMail.setMailTypeId( 0 );
        obDBSendMail.setDateSend( QDate::currentDate().toString("yyyy-MM-dd") );
        obDBSendMail.setRecipients( qslMailRecipients.join(";") );
        obDBSendMail.setSubject( ledSubject->text() );
        obDBSendMail.setMessage( teMessageBody->toPlainText() );
        obDBSendMail.save();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    QDialog::accept();
}

void dlgSendMail::on_pbSave_clicked()
{
    try
    {
        cDBSendMail obDBSendMail;

        obDBSendMail.createNew();
        obDBSendMail.setLicenceId( g_poPrefs->getLicenceId() );
        obDBSendMail.setMailTypeId( 0 );
        obDBSendMail.setDateSend( "" );
        obDBSendMail.setRecipients( teTo->toPlainText().trimmed() );
        obDBSendMail.setSubject( ledSubject->text() );
        obDBSendMail.setMessage( teMessageBody->toPlainText() );
        obDBSendMail.save();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    QDialog::accept();
}

void dlgSendMail::on_pbCancel_clicked()
{
    QDialog::reject();
}

void dlgSendMail::on_pbTo_clicked()
{
    cDlgSelectMailRecipient obDlgSelectMailRecipient( this );

    obDlgSelectMailRecipient.exec();

    QString qsRecipients = teTo->toPlainText().trimmed();

    if( obDlgSelectMailRecipient.qslRecipients.count() > 0 )
    {
        if( qsRecipients.length() > 0 )
        {
            qsRecipients.append( "; " );
        }
        qsRecipients.append( obDlgSelectMailRecipient.qslRecipients.join("; ") );
    }
    teTo->setPlainText( qsRecipients );
}
