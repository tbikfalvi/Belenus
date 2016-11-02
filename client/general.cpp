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

//====================================================================================
cGeneral::cGeneral()
//====================================================================================
{
    m_poBlTr            = new QTranslator();
    m_poQtTr            = new QTranslator();
    m_bIsLanguageLoaded = false;
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
void cGeneral::showPatientCardInformation(QString p_qsBarcode)
//------------------------------------------------------------------------------------
{
    try
    {
        cDlgInformation obDlgInformation;
        cDBPatientCard  obDBPatientCard;
        cDBGuest        obDBGuest;
        QString         qsText = "";

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

        QString qsQuery = QString( "SELECT patientCardUnitId, patientCardTypeId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                   "FROM patientcardunits "
                                   "WHERE patientCardId=%1 "
                                   "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                   "AND prepared=0 "
                                   "AND active=1 "
                                   "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" ).arg( obDBPatientCard.id() );
        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

        qsText.append( QObject::tr("<p><b>Valid units:</b><br>") );

        while( poQuery->next() )
        {
            QString qsValid;
            unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

            if( uiPCTId > 0 )
            {
                cDBPatientCardType obDBPatientCardType;

                obDBPatientCardType.load( uiPCTId );
                obDBPatientCard.isPatientCardCanBeUsed( uiPCTId, &qsValid );
                qsText.append( QObject::tr("<br><b>%1 units (%2 minutes) (%3) valid on</b>%4")
                               .arg( poQuery->value( 5 ).toString() )
                               .arg( poQuery->value( 2 ).toString() )
                               .arg( obDBPatientCardType.name() )
                               .arg( qsValid ) );
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
            qsText.append( QObject::tr("<p><b>Expired units:</b><br>") );

            while( poQuery->next() )
            {
                unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                if( uiPCTId > 0 )
                {
                    cDBPatientCardType obDBPatientCardType;

                    obDBPatientCardType.load( uiPCTId );
                    qsText.append( QObject::tr("<br><b>%1 units (%2 minutes) (%3) expired on</b>%4")
                                   .arg( poQuery->value( 5 ).toString() )
                                   .arg( poQuery->value( 2 ).toString() )
                                   .arg( obDBPatientCardType.name() )
                                   .arg( poQuery->value( 4 ).toString() ) );
                }
            }
        }

        QString qsTitle;

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
        obDlgInformation.exec();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
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
    }

    return bRet;
}
//====================================================================================
//QString cGeneral::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
//====================================================================================
/*{
    QString qsValue = QString::number( p_nCurrencyValue );
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
    qsRet += " " + p_qsCurrency;

    return qsRet;
}*/
//*********************************************************************************************************************
// Class cCurrency
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
