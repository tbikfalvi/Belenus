//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : general.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Altalanos, tobb resz altal hasznalt fuggvenyek, process-ek
//====================================================================================

#include <QMessageBox>
#include <QList>
#include <QProcess>

#include "general.h"
#include "belenus.h"
#include "dlg/dlglogin.h"
#include "dlg/dlginformation.h"
#include "db/dbguest.h"
#include "db/dbpatientcardtype.h"
#include "db/dbskintypes.h"
#include "db/dbpanelgroups.h"

//====================================================================================
cGeneral::cGeneral()
//====================================================================================
{
    m_poBlTr            = new QTranslator();
    m_poQtTr            = new QTranslator();
    m_bIsLanguageLoaded = false;
    m_poWindowMain      = 0;
    m_poWindowSecondary = 0;
}
//====================================================================================
cGeneral::~cGeneral()
//====================================================================================
{
    delete m_poBlTr;
    delete m_poQtTr;
}
//====================================================================================
void cGeneral::setApplication( QApplication *p_poApplication )
//====================================================================================
{
    m_poMainApplication = p_poApplication;
}
//====================================================================================
void cGeneral::setApplicationLanguage( const QString &p_qsLang )
//====================================================================================
{
    if( m_bIsLanguageLoaded )
    {
        m_poMainApplication->removeTranslator( m_poBlTr );
        m_poMainApplication->removeTranslator( m_poQtTr );
    }

    QString qsLangBl = QString("lang/%1%2.qm").arg( g_poPrefs->getLangFilePrefix() ).arg( p_qsLang );
    QString qsLangQT = QString("lang/qt_%1.qm").arg( p_qsLang );

    m_poBlTr->load( qsLangBl );
    m_poQtTr->load( qsLangQT );

    m_poMainApplication->installTranslator( m_poBlTr );
    m_poMainApplication->installTranslator( m_poQtTr );
}
//====================================================================================
int cGeneral::customMsgBox(QWidget */*parent*/, msgBoxType msgtype, QString buttonstext, QString msg, QString details)
//====================================================================================
{
    QMessageBox msgBox;

    //msgBox.setParent( parent );

    switch(msgtype)
    {
        case MSG_INFORMATION:
            msgBox.setWindowTitle( QObject::tr("Information") );
            msgBox.setIcon( QMessageBox::Information );
            break;
        case MSG_WARNING:
            msgBox.setWindowTitle( QObject::tr("Warning") );
            msgBox.setIcon( QMessageBox::Warning );
            break;
        case MSG_ATTENTION:
            msgBox.setWindowTitle( QObject::tr("Attention") );
            msgBox.setIcon( QMessageBox::Warning );
            break;
        case MSG_ERROR:
            msgBox.setWindowTitle( QObject::tr("Error") );
            msgBox.setIcon( QMessageBox::Critical );
            break;
        case MSG_QUESTION:
            msgBox.setWindowTitle( QObject::tr("Question") );
            msgBox.setIcon( QMessageBox::Question );
            break;
    }

    msgBox.setText( msg );
    if( details.length() > 0 ) msgBox.setDetailedText( details );

    QList<QPushButton*> qlButtons;
    QStringList         qslButtons = buttonstext.split('|');

    for( int i=0; i<qslButtons.size(); i++ )
    {
        QPushButton *pButton = msgBox.addButton( qslButtons.at(i), QMessageBox::ActionRole );
        qlButtons.append( pButton );
    }

    msgBox.exec();

    int nRet = 0;

    for( int i=0; i<qslButtons.size(); i++ )
    {
        if( msgBox.clickedButton() == (QAbstractButton*)qlButtons.at(i) )
        {
            nRet = i+1;
            break;
        }
    }

    return nRet;
}
//====================================================================================
void cGeneral::backupDatabase(QWidget *parent)
{
    bool    bBackupDatabase = false;

    if( g_poPrefs->isBackupDatabase() )
    {
        g_obLogger(cSeverity::INFO) << "Backup database process enabled";

        QStringList     qslDays;
        QString         qsBackupDays = g_poPrefs->getBackupDatabaseDays();
        int             nDaysInMonth = QDate::currentDate().daysInMonth();

        qslDays << QObject::tr("Mon")
                << QObject::tr("Tue")
                << QObject::tr("Wed")
                << QObject::tr("Thu")
                << QObject::tr("Fri")
                << QObject::tr("Sat")
                << QObject::tr("Sun");

        if( g_poPrefs->getBackupDatabaseType() == 1 )
        {
            g_obLogger(cSeverity::INFO) << " on every exit" << EOM;
            bBackupDatabase = true;
        }
        else if( g_poPrefs->getBackupDatabaseType() == 2 && g_obCassa.isCassaClosed() )
        {
            g_obLogger(cSeverity::INFO) << " on exit if cassa is closed" << EOM;
            bBackupDatabase = true;
        }
        else if( g_poPrefs->getBackupDatabaseType() == 3 && QDate::currentDate().dayOfWeek() == 7 )
        {
            g_obLogger(cSeverity::INFO) << " on exit on the last day of week" << EOM;
            bBackupDatabase = true;
        }
        else if( g_poPrefs->getBackupDatabaseType() == 4 && QDate::currentDate().day() == nDaysInMonth )
        {
            g_obLogger(cSeverity::INFO) << " on exit on the last day of month" << EOM;
            bBackupDatabase = true;
        }
        else if( g_poPrefs->getBackupDatabaseType() == 5 && qsBackupDays.contains(qslDays.at( QDate::currentDate().dayOfWeek()-1 ), Qt::CaseInsensitive) )
        {
            g_obLogger(cSeverity::INFO) << " on exit on the selected day: " << qslDays.at( QDate::currentDate().dayOfWeek()-1 ) << EOM;
            bBackupDatabase = true;
        }
    }

    if( g_poPrefs->isForceBackupDatabase() )
    {
        g_obLogger(cSeverity::INFO) << "User selected to force database backup process" << EOM;
        bBackupDatabase = true;
    }

    if( bBackupDatabase )
    {
        if( QMessageBox::question( parent, QObject::tr("Question"),
                                   QObject::tr("Do you want to backup database now?"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
        {
            bBackupDatabase = false;
        }
    }

    if( bBackupDatabase )
    {
        g_obLogger(cSeverity::INFO) << "Database backup process initiated" << EOM;
        QProcess::startDetached( "dbbackup.exe" );
    }
}
//====================================================================================
bool cGeneral::isSystemAdmin()
//------------------------------------------------------------------------------------
{
    bool    bRet = false;

    cDlgLogIn   *poDlgLogIn = new cDlgLogIn();

    if( poDlgLogIn->exec() == QDialog::Accepted )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT password FROM users WHERE userId=1" );
        poQuery->first();
        QString qsPsw = poQuery->value(0).toString();

        if( poDlgLogIn->m_qsPassword.compare( qsPsw ) == 0 )
        {
            bRet = true;
        }
    }

    return bRet;
}
//====================================================================================
bool cGeneral::isExtendedAdmin()
//------------------------------------------------------------------------------------
{
    bool    bRet = false;

    cDlgLogIn   *poDlgLogIn = new cDlgLogIn();

    if( poDlgLogIn->exec() == QDialog::Accepted )
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT value FROM settings WHERE identifier='ADMIN_EXT_PASSWORD' " );
        poQuery->first();
        QString qsPsw = poQuery->value(0).toString();

        if( poDlgLogIn->m_qsPassword.compare( qsPsw ) == 0 )
        {
            bRet = true;
        }
    }

    return bRet;
}
//====================================================================================
bool cGeneral::isExtendedOrSystemAdmin()
//------------------------------------------------------------------------------------
{
    bool    bRet = false;

    cDlgLogIn   *poDlgLogIn = new cDlgLogIn();

    if( poDlgLogIn->exec() == QDialog::Accepted )
    {
        QSqlQuery *poQuery;

        poQuery = g_poDB->executeQTQuery( "SELECT password FROM users WHERE userId=1" );
        poQuery->first();
        QString qsPsw1 = poQuery->value(0).toString();

        poQuery = g_poDB->executeQTQuery( "SELECT value FROM settings WHERE identifier='ADMIN_EXT_PASSWORD' " );
        poQuery->first();
        QString qsPsw2 = poQuery->value(0).toString();

        if( poDlgLogIn->m_qsPassword.compare( qsPsw1 ) == 0 ||
            poDlgLogIn->m_qsPassword.compare( qsPsw2 ) == 0 )
        {
            bRet = true;
        }
    }

    return bRet;
}
//====================================================================================
QString cGeneral::getPatientCardInformationString(QString p_qsBarcode)
//------------------------------------------------------------------------------------
{
    QString qsText = "";
    try
    {
        cDBPatientCard  obDBPatientCard;
        obDBPatientCard.load( p_qsBarcode );

        QString qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime), panelGroupId "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                   "AND prepared=0 "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo, patientCardTypeId, panelGroupId ORDER BY validDateTo, patientCardUnitId" ).arg( obDBPatientCard.id() );
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        qsText.append( QObject::tr("<b>Valid units:</b>") );

        if( poQuery->size() > 0 )
        {
            qsText.append( "<br><table>" );
            while( poQuery->next() )
            {
                QString qsValid;
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();
                unsigned int uiPanelGroupId = poQuery->value( 6 ).toUInt();

                if( uiPCTId > 0 )
                {
                    cDBPatientCardType obDBPatientCardType;
                    cDBPanelGroups     obDBPanelGroup;

                    obDBPatientCardType.load( uiPCTId );
                    obDBPatientCard.isPatientCardCanBeUsed( uiPCTId, &qsValid );

                    obDBPanelGroup.load( uiPanelGroupId );

                    QString qsPanelGroupName = obDBPanelGroup.name();
                    g_obLogger(cSeverity::INFO) << "Panel group [" << qsPanelGroupName << "] " << EOM;

                    qsText.append( "<tr>" );
                    qsText.append( QObject::tr("<td><b>%1 units</b></td>").arg( poQuery->value( 5 ).toString() ) );
                    qsText.append( QObject::tr("<td>(<i>%1 minutes</i>)</td>").arg( poQuery->value( 2 ).toString() ) );
                    qsText.append( QString("<td>(<i>%1 - %2</i>)</td>").arg( obDBPatientCardType.name() ).arg( qsPanelGroupName ) );
                    qsText.append( QObject::tr("<td>valid on %1</td>").arg( qsValid ) );
                    qsText.append( "<td> | </td>" );
                    qsText.append( QString("<td>%1</td>").arg( poQuery->value( 3 ).toString() ) );
                    qsText.append( "<td> -> </td>" );
                    qsText.append( QString("<td>%1</td>").arg( poQuery->value( 4 ).toString() ) );
                    qsText.append( "</tr>" );
/*
                    qsText.append( QObject::tr("<br><b>%1 units (%2 minutes) (%3) valid on</b>%4 | %5 - %6")
                                   .arg( poQuery->value( 5 ).toString() )
                                   .arg( poQuery->value( 2 ).toString() )
                                   .arg( obDBPatientCardType.name() )
                                   .arg( qsValid )
                                   .arg( poQuery->value( 3 ).toString() )
                                   .arg( poQuery->value( 4 ).toString() ) );
*/
                }
            }
            qsText.append( "</table>" );
        }
        else
        {
            qsText.append( QObject::tr("<br><i>There is no valid, useable unit on this card.</i>" ) );
        }

        qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                           "FROM patientcardunits "
                           "WHERE patientCardId=%1 "
                           "AND validDateTo<CURDATE() "
                           "AND prepared=0 "
                           "AND active=1 "
                           "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" ).arg( obDBPatientCard.id() );
        poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            qsText.append( QObject::tr("<br><br><b>Expired units:</b><br>") );

            while( poQuery->next() )
            {
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId > 0 )
                {
                    cDBPatientCardType obDBPatientCardType;

                    obDBPatientCardType.load( uiPCTId );
                    qsText.append( QObject::tr("<br>%1 units (%2 minutes) (%3) expired on %4")
                                   .arg( poQuery->value( 5 ).toString() )
                                   .arg( poQuery->value( 2 ).toString() )
                                   .arg( obDBPatientCardType.name() )
                                   .arg( poQuery->value( 4 ).toString() ) );
                }
            }
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
    return qsText;
}
//====================================================================================
QStringList cGeneral::getPatientCardUnusedUnits(unsigned int p_uiCardId)
//------------------------------------------------------------------------------------
{
    QStringList qslValidUnits = QStringList();
    QString     qsText = "";
    try
    {
        cDBPatientCard  obDBPatientCard;
        obDBPatientCard.load( p_uiCardId );

        QString qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                   "AND prepared=0 "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" ).arg( obDBPatientCard.id() );
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            while( poQuery->next() )
            {
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId > 0 )
                {
                    qsText = "";
                    cDBPatientCardType obDBPatientCardType;

                    obDBPatientCardType.load( uiPCTId );

                    qsText.append( QObject::tr("%1 units ").arg( poQuery->value( 5 ).toString() ) );
                    qsText.append( QObject::tr("(%1 minutes) ").arg( poQuery->value( 2 ).toString() ) );
                    qsText.append( QString("(%1) ").arg( obDBPatientCardType.name() ) );
                    qsText.append( QString("%1 -> ").arg( poQuery->value( 3 ).toString() ) );
                    qsText.append( QString("%1").arg( poQuery->value( 4 ).toString() ) );
                    qsText.append( "#" );
                    qsText.append( QString("%1|%2|%3").arg( poQuery->value(2).toInt() )
                                                      .arg( poQuery->value(4).toString() )
                                                      .arg( poQuery->value(1).toUInt() ) );

                    qslValidUnits << qsText;
                }
            }
        }

        qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                           "FROM patientcardunits "
                           "WHERE patientCardId=%1 "
                           "AND validDateTo<CURDATE() "
                           "AND prepared=0 "
                           "AND active=1 "
                           "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" ).arg( obDBPatientCard.id() );
        poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            while( poQuery->next() )
            {
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId > 0 )
                {
                    qsText = "";
                    cDBPatientCardType obDBPatientCardType;

                    obDBPatientCardType.load( uiPCTId );

                    qsText.append( QObject::tr("%1 units ").arg( poQuery->value( 5 ).toString() ) );
                    qsText.append( QObject::tr("(%1 minutes) ").arg( poQuery->value( 2 ).toString() ) );
                    qsText.append( QString("(%1) ").arg( obDBPatientCardType.name() ) );
                    qsText.append( QString("%1 -> ").arg( poQuery->value( 3 ).toString() ) );
                    qsText.append( QString("%1").arg( poQuery->value( 4 ).toString() ) );
                    qsText.append( "#" );
                    qsText.append( QString("%1|%2|%3").arg( poQuery->value(2).toInt() )
                                                      .arg( poQuery->value(4).toString() )
                                                      .arg( poQuery->value(1).toUInt() ) );

                    qslValidUnits << qsText;
                }
            }
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return qslValidUnits;
}
//====================================================================================
void cGeneral::showPatientCardInformation(QString p_qsBarcode, int p_nCloseSeconds)
//------------------------------------------------------------------------------------
{
    try
    {
        cDlgInformation obDlgInformation( m_poWindowMain );
        QString         qsTitle;
        QString         qsText;
        cDBPatientCard  obDBPatientCard;
        cDBGuest        obDBGuest;

        obDBPatientCard.load( p_qsBarcode );
        obDBGuest.load( obDBPatientCard.patientId() );

        QString qsOwner     = obDBGuest.name();
        QString qsValidFrom = obDBPatientCard.validDateFrom();
        QString qsValidTo   = obDBPatientCard.validDateTo();
        int     nUnitCount  = obDBPatientCard.units();
        QString qsComment   = obDBPatientCard.comment();

        qsText.append( QString("<table>") );
        qsText.append( QObject::tr("<tr><td width=\"100\"><b>Owner:</b></td><td>%1</td></tr>").arg( qsOwner ) );
        if( qsValidFrom.length() > 0 && qsValidTo.length() > 0 )
        {
            qsText.append( QObject::tr("<tr><td><b>Valid:</b></td><td>%1 -> %2</td></tr>").arg( qsValidFrom ).arg( qsValidTo ) );
        }
        else
        {
            qsText.append( QObject::tr("<tr><td><b>Valid:</b></td><td></td></tr>") );
        }
        qsText.append( QObject::tr("<tr><td><b>Units:</b></td><td>%1</td></tr>").arg( nUnitCount ) );
        qsText.append( QObject::tr("<tr><td><b>Comment:</b></td></tr>") );
        qsText.append( QString("<tr><td>%1</td></tr>").arg( qsComment ) );
        qsText.append( QString("</table>") );
        qsText.append( "<p>" );
        if( obDBPatientCard.parentId() > 0 )
        {
            cDBPatientCard  obDBParentCard;

            obDBParentCard.load( obDBPatientCard.parentId() );
            qsText.append( getPatientCardInformationString( obDBParentCard.barcode() ) );
        }
        else
        {
            qsText.append( getPatientCardInformationString( p_qsBarcode ) );
        }

        if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
        {
            QString qsBarcode = obDBPatientCard.barcode();
            qsTitle = qsBarcode.fill('*');
        }
        else
        {
            qsTitle = obDBPatientCard.barcode();
        }
        if( obDBPatientCard.active() == 0 )
        {
            qsTitle.append( QObject::tr(" - INACTIVE Patientcard") );
        }
        obDlgInformation.setInformationTitle( qsTitle );
        obDlgInformation.setInformationText( qsText );
        obDlgInformation.setTimer( p_nCloseSeconds );
        obDlgInformation.exec();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}
//====================================================================================
void cGeneral::showPatientLastVisitInformation(QString p_qsBarcode, int p_nCloseSeconds)
//------------------------------------------------------------------------------------
{
    try
    {
        QWidget        *poParent = m_poWindowMain;

        if( g_poPrefs->getShowInfoOnWindow() == 2 )
            poParent = m_poWindowSecondary;

        cDlgInformation obDlgInformation( poParent );
        QString         qsTitle;
        QString         qsText;
        cDBPatientCard  obDBPatientCard;
        cDBGuest        obDBGuest;
        cDBSkinTypes    obDBSkinType;

        obDBPatientCard.load( p_qsBarcode );
        obDBGuest.load( obDBPatientCard.patientId() );
        obDBSkinType.load( obDBGuest.skinTypeId() );

        QString qsOwner     = obDBGuest.name();
        QString qsSkinType  = obDBSkinType.name();
        QString qsBarcode   = "";

        if( g_poPrefs->isBarcodeHidden() && !g_obUser.isInGroup( cAccessGroup::ADMIN ) )
        {
            QString qsTemp = obDBPatientCard.barcode();
            qsBarcode = qsTemp.fill('*');
        }
        else
        {
            qsBarcode = obDBPatientCard.barcode();
        }

        qsTitle = QObject::tr("%1 last visit information").arg( qsOwner );

        qsText.append( QString("<table>") );
        qsText.append( QObject::tr("<tr><td width=\"150\"><b>Skin type:</b></td><td>%1</td></tr>").arg( qsSkinType ) );
        qsText.append( QObject::tr("<tr><td width=\"150\"><b>Patientcard barcode:</b></td><td>%1</td></tr>").arg( qsBarcode ) );
        qsText.append( QString("</table>") );
        qsText.append( "<p>" );

        QString qsQuery = QString( "SELECT dateTimeUsed, COUNT(dateTimeUsed), patientcardunits.unitTime, title "
                                   "FROM patientcardunits, panels WHERE "
                                   "patientcardunits.panelid = panels.panelid AND "
                                   "patientcardunits.active=0 AND "
                                   "patientcardid=%1 GROUP BY "
                                   "patientcardid, dateTimeUsed ORDER BY dateTimeUsed" ).arg( obDBPatientCard.id() );
        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        qsText.append( QObject::tr("<table><tr><td><b>Last visit</b></td></tr></table>") );
        qsText.append( QString("<table>") );
        if( poQuery->size() > 0 )
        {
            poQuery->last();

            QString qsDate      = poQuery->value(0).toDateTime().toString( QString("%1  hh:mm").arg( g_poPrefs->getDateFormat() ) );
            QString qsLength    = QString::number( poQuery->value(1).toInt()*poQuery->value(2).toInt() );
            QString qsTitle     = poQuery->value( 3 ).toString();

            qsText.append( QObject::tr("<tr><td width=\"150\"><b>Date:</b></td><td>%1</td></tr>").arg( qsDate ) );
            qsText.append( QObject::tr("<tr><td width=\"150\"><b>Length:</b></td><td>%1 minutes</td></tr>").arg( qsLength ) );
            qsText.append( QObject::tr("<tr><td width=\"150\"><b>Device:</b></td><td>%1</td></tr>").arg( qsTitle ) );
        }
        else
        {
            qsText.append( QObject::tr("<tr><td><i>There is no recorded usage of this card yet</i></td></tr>") );
        }



        qsText.append( QString("</table>") );

        obDlgInformation.setInformationTitle( qsTitle );
        obDlgInformation.setInformationText( qsText );
        obDlgInformation.setTimer( p_nCloseSeconds );
        obDlgInformation.exec();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}
//====================================================================================
bool cGeneral::isShoppingCartHasItems()
//------------------------------------------------------------------------------------
{
    bool bRet = false;

    try
    {
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( "SELECT * FROM `shoppingcartitems` WHERE shoppingCartItemId>0" );

        if( poQuery->size() > 0 )
        {
            bRet = true;
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return bRet;
}

//====================================================================================
bool cGeneral::isAppicationRunning(QString p_qsAppName)
//------------------------------------------------------------------------------------
{
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");

    process.waitForStarted(1000);
    process.waitForFinished(1000);

    QString listOfRunningApps = QString( process.readAll() );

    if( listOfRunningApps.contains( p_qsAppName) )
        return true;

    return false;
}

//====================================================================================
void cGeneral::initSysTrayIcon()
//------------------------------------------------------------------------------------
{
    m_stIcon = new QSystemTrayIcon();

    m_stIcon->setIcon( QIcon("./resources/belenus.ico") );
    m_stIcon->show();
}

//====================================================================================
void cGeneral::setSysTrayIconParent( QObject *parent )
//------------------------------------------------------------------------------------
{
    m_stIcon->setParent( parent );
}

//====================================================================================
void cGeneral::showTrayInfo( QString p_qsMessage )
//------------------------------------------------------------------------------------
{
    m_stIcon->showMessage( QObject::tr("Information"), p_qsMessage, QSystemTrayIcon::Information, 5000 );
}

//====================================================================================
void cGeneral::showTrayWarning( QString p_qsMessage )
//------------------------------------------------------------------------------------
{
    m_stIcon->showMessage( QObject::tr("Information"), p_qsMessage, QSystemTrayIcon::Warning, 5000 );
}

//====================================================================================
void cGeneral::showTrayError( QString p_qsMessage )
//------------------------------------------------------------------------------------
{
    m_stIcon->showMessage( QObject::tr("Information"), p_qsMessage, QSystemTrayIcon::Critical, 5000 );
}
//====================================================================================
bool cGeneral::isArchiveOnDifferentPath()
//------------------------------------------------------------------------------------
{
    QString qsPathApp = g_poPrefs->getApplicationPath().right( g_poPrefs->getApplicationPath().length() - 3 ).replace( "\\", "/" );
    QString qsPathArc = g_poPrefs->getDirDbBackup().right( g_poPrefs->getDirDbBackup().length() - 3 ).replace( "\\", "/" );

    QString qsDirApp    = qsPathApp.left( qsPathApp.indexOf( '/' ) );
    QString qsDirArc    = qsPathArc.left( qsPathArc.indexOf( '/' ) );

    if( qsDirApp.compare( qsDirArc ) == 0 )
        return false;

    return true;
}
//====================================================================================
void cGeneral::setWindowMainWidget( QWidget *poWindow )
//------------------------------------------------------------------------------------
{
    m_poWindowMain = poWindow;
}
//====================================================================================
void cGeneral::setWindowSecondaryWidget( QWidget *poWindow )
//------------------------------------------------------------------------------------
{
    m_poWindowSecondary = poWindow;
}
//====================================================================================
int cGeneral::deleteOldLogFiles(int p_nDeleteLogFileMonths)
//------------------------------------------------------------------------------------
{
    int nFilesDeleted = 0;

    QDir logDir = QDir( QCoreApplication::applicationDirPath() ); // Log fájlok mappája
    QStringList logFiles = logDir.entryList( QStringList() << "belenus_*.log", QDir::Files );

    // Küszöb dátum: ennyi hónapnál régebbi fájlokat törlünk
    QDate thresholdDate = QDate::currentDate().addMonths( -p_nDeleteLogFileMonths );

    QRegExp rx("^belenus_(\\d{8})\\.log$"); // Regexp a dátum kiszedésére

    foreach( QString fileName, logFiles )
    {
        if( rx.indexIn(fileName) != -1 )
        {
            QString dateString = rx.cap(1); // Pl. "20250316"
            QDate fileDate = QDate::fromString(dateString, "yyyyMMdd");

            if( fileDate.isValid() && fileDate < thresholdDate )
            {
                QString filePath = logDir.filePath( fileName );
                if( QFile::remove(filePath) )
                {
                    nFilesDeleted++;
                    g_obLogger(cSeverity::DEBUG) << "Log file deleted: [" << filePath << "]" << EOM;
                }
                else
                {
                    g_obLogger(cSeverity::WARNING) << "Log file delete failed: [" << filePath << "]" << EOM;
                }
            }
        }
    }
    return nFilesDeleted;
}
//====================================================================================
void cGeneral::saveGuestActivity(unsigned int p_uiPatientId,
                                 int p_nPatientHistoryType,
                                 unsigned int p_uiPanelId,
                                 int p_nDeviceTime,
                                 unsigned int p_uiPatientCardId,
                                 unsigned int p_uiProductId)
//------------------------------------------------------------------------------------
{
    try
    {
        QString qsQuery = "INSERT INTO patienthistory SET ";

        qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
        qsQuery += QString( "patientId = \"%1\", " ).arg( p_uiPatientId );
        qsQuery += QString( "patientHistoryTypeId = \"%1\", " ).arg( p_nPatientHistoryType );
        qsQuery += QString( "panelId = \"%1\", " ).arg( p_uiPanelId );
        qsQuery += QString( "panelTime = \"%1\", " ).arg( p_nDeviceTime );
        qsQuery += QString( "patientCardId = \"%1\", " ).arg( p_uiPatientCardId );
        qsQuery += QString( "productId = \"%1\" " ).arg( p_uiProductId );

        QSqlQuery *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery ) delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

}


//*********************************************************************************************************************
//
// Class cCurrency
//
//*********************************************************************************************************************
cCurrency::cCurrency(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int p_nVat)
{
//    cTracer obTrace( "cCurrency::cCurrency(QString,currType,int)" );

    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;
    m_bIsNegative   = false;

    _init( p_qsCurrencyString, p_ctCurrencyType, p_nVat );
}
//*********************************************************************************************************************
// Class cCurrency
//*********************************************************************************************************************
cCurrency::cCurrency(int p_nCurrencyValue, currType p_ctCurrencyType, int p_nVat)
{
//    cTracer obTrace( "cCurrency::cCurrency(int,currType,int)" );

    m_nValue        = 0;
    m_nValueLeft    = 0;
    m_nValueRight   = 0;
    m_ctCurrType    = p_ctCurrencyType;
    m_nVatValue     = p_nVat;

    QString qsCurrency = QString::number(p_nCurrencyValue);

    qsCurrency.insert( qsCurrency.length()-2, g_poPrefs->getCurrencyDecimalSeparator() );

    _init( qsCurrency, p_ctCurrencyType, p_nVat );

//    g_obLogger(cSeverity::INFO) << "m_nValue " << m_nValue << EOM;
//    g_obLogger(cSeverity::INFO) << "m_nValueNet " << m_nValueNet << EOM;
//    g_obLogger(cSeverity::INFO) << "m_nValueGross " << m_nValueGross << EOM;
}
//====================================================================================
void cCurrency::_init(const QString &p_qsCurrencyString, currType p_ctCurrencyType, int /*p_nVat*/)
{
//    g_obLogger(cSeverity::DEBUG) << "fullstr: [" << p_qsCurrencyString << "]" << EOM;

    QString qsPureCurrency = p_qsCurrencyString;

    // Remove currency string
    if( qsPureCurrency.contains( g_poPrefs->getCurrencyLong() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencyLong() );
    }
    else if( qsPureCurrency.contains( g_poPrefs->getCurrencyShort() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencyShort() );
    }

    // Remove currency tousand separator
    if( p_qsCurrencyString.contains( g_poPrefs->getCurrencySeparator() ) )
    {
        qsPureCurrency = qsPureCurrency.remove( g_poPrefs->getCurrencySeparator() );
    }

    // Remove spaces
    qsPureCurrency = qsPureCurrency.remove( " " );

//    g_obLogger(cSeverity::DEBUG) << "purestr: [" << qsPureCurrency << "]" << EOM;

    m_bIsNegative = false;
    // Remove the minus sign
    if( qsPureCurrency.left(1).compare("-") == 0 )
    {
        m_bIsNegative = true;
        qsPureCurrency = qsPureCurrency.remove( "-" );
    }

    // Get value before and after decimal separator
    if( qsPureCurrency.contains( g_poPrefs->getCurrencyDecimalSeparator() ) )
    {
        QStringList qslCurr = qsPureCurrency.split( g_poPrefs->getCurrencyDecimalSeparator() );
        QString     qsRight = qslCurr.at(1);

        m_nValueLeft    = qslCurr.at(0).toInt();
        m_nValueRight   = qsRight.append("00").left(2).toInt();
    }
    else
    {
        m_nValueLeft    = qsPureCurrency.toInt();
        m_nValueRight   = 0;
    }
//    g_obLogger(cSeverity::DEBUG) << "nValueLeft: ["
//                                 << m_nValueLeft
//                                 << "] nValueRight: ["
//                                 << m_nValueRight
//                                 << "]"
//                                 << EOM;

    // Calculate full currency value (original *100)
    m_nValue = m_nValueLeft * 100 + m_nValueRight;

//    g_obLogger(cSeverity::DEBUG) << "m_nValue: [" << m_nValue << "]" << EOM;

    if( m_nVatValue > 0 )
    {
        if( p_ctCurrencyType == CURR_NET )
        {
            m_nValueNet   = m_nValue;
            m_nValueGross = m_nValue + (m_nValue * m_nVatValue) / 100;
        }
        else
        {
            m_nValueNet   = m_nValue  * 100 / (100 + m_nVatValue);
            m_nValueGross = m_nValue;
        }
    }
    else
    {
        m_nValueNet   = m_nValue;
        m_nValueGross = m_nValue;
    }

//    g_obLogger(cSeverity::DEBUG) << "net/gross " << QString("%1/%2").arg(m_nValueNet).arg(m_nValueGross) << EOM;
}
//====================================================================================
cCurrency::~cCurrency()
{

}
//====================================================================================
QString cCurrency::currencyValue(currType p_ctCurrencyType)
{
    if( p_ctCurrencyType == CURR_GROSS )
    {
        m_nValue = m_nValueGross;
    }
    else
    {
        m_nValue = m_nValueNet;
    }

    m_nValueLeft  = m_nValue / 100;
    m_nValueRight = m_nValue % 100;

//    g_obLogger(cSeverity::DEBUG) << "L/R " << QString("%1/%2").arg(m_nValueLeft).arg(m_nValueRight) << EOM;
//    g_obLogger(cSeverity::DEBUG) << "RET currencyValue ["
//                                 << QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg( m_nValue )
//                                 << "]"
//                                 << EOM;

    return QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg( m_nValue );
}
//====================================================================================
QString cCurrency::currencyString(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
    {
        QString qsRight = "";

        if( m_nValueRight < 10 )
            qsRight.append( "0" );

        qsRight.append( QString::number(m_nValueRight) );

        qsRet = QString( "%1%2%3%4" ).arg(m_bIsNegative?"-":"").arg(m_nValueLeft).arg(g_poPrefs->getCurrencyDecimalSeparator()).arg(qsRight);
    }
    else
    {
        qsRet = QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg(m_nValueLeft);
    }

//    g_obLogger(cSeverity::DEBUG) << "RET currencyString ["
//                                 << qsRet
//                                 << "]"
//                                 << EOM;
    return qsRet;
}
//====================================================================================
QString cCurrency::currencyStringSeparator(currType p_ctCurrencyType)
{
    currencyValue( p_ctCurrencyType );

    QString qsRet = "";

    if( m_nValueRight > 0 )
    {
        QString qsRight = "";

        if( m_nValueRight < 10 )
            qsRight.append( "0" );

        qsRight.append( QString::number(m_nValueRight) );

        qsRet = QString( "%1%2%3%4" ).arg(m_bIsNegative?"-":"")
                                     .arg(_separatedValue(m_nValueLeft))
                                     .arg(g_poPrefs->getCurrencyDecimalSeparator())
                                     .arg(qsRight);
    }
    else
    {
        qsRet = QString( "%1%2" ).arg(m_bIsNegative?"-":"").arg(_separatedValue(m_nValueLeft));
    }

//    g_obLogger(cSeverity::DEBUG) << "RET currencyStringSeparator ["
//                                 << qsRet
//                                 << "]"
//                                 << EOM;
    return qsRet;
}
//====================================================================================
QString cCurrency::currencyFullStringShort( currType p_ctCurrencyType )
{
    QString qsRet = QString( "%1 %2" ).arg(currencyStringSeparator(p_ctCurrencyType)).arg(g_poPrefs->getCurrencyShort());

//    g_obLogger(cSeverity::DEBUG) << "RET currencyFullStringShort ["
//                                 << qsRet
//                                 << "]"
//                                 << EOM;
    return qsRet;
}
//====================================================================================
QString cCurrency::currencyFullStringLong( currType p_ctCurrencyType )
{
    QString qsRet = QString( "%1 %2" ).arg(currencyStringSeparator(p_ctCurrencyType)).arg(g_poPrefs->getCurrencyLong());
//    g_obLogger(cSeverity::DEBUG) << "RET currencyFullStringLong ["
//                                 << qsRet
//                                 << "]"
//                                 << EOM;
    return qsRet;
}
//====================================================================================
QString cCurrency::_separatedValue(int p_nValue)
{
    QString qsValue = QString::number( p_nValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );

    return qsRet;
}
//====================================================================================
