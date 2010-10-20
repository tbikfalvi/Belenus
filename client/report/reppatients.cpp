#include <QSqlQuery>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

#include "../framework/qtframework.h"
#include "reppatients.h"


cDlgReportPatients::cDlgReportPatients( QWidget *parent )
    : cDlgPreview( parent )
{
    cTracer obTrace( "cDlgReportPatients::cDlgReportPatients" );

    QSqlQuery *poQuery;

    setReportTitle( tr( "Patients" ) );

    //======================================================================================================

    verticalLayout1 = new QVBoxLayout( grpFilters );
    verticalLayout1->setObjectName( QString::fromUtf8( "verticalLayout1" ) );

    //======================================================================================================

    horizontalLayout1 = new QHBoxLayout( grpFilters );
    horizontalLayout1->setObjectName( QString::fromUtf8( "horizontalLayout1" ) );

    lblDate = new QLabel( tr("Entered into database :"), grpFilters );
    lblDate->setObjectName( QString::fromUtf8( "lblDate" ) );
    horizontalLayout1->addWidget( lblDate );

    dteStartDate = new QDateTimeEdit( grpFilters );
    dteStartDate->setObjectName( QString::fromUtf8( "dteStartDate" ) );
    dteStartDate->setCalendarPopup( true );
    dteStartDate->setDate( QDate::currentDate().addMonths(-1) );
    dteStartDate->setDisplayFormat( "yyyy-MM-dd" );
    horizontalLayout1->addWidget( dteStartDate );

    lblTo = new QLabel( "->", grpFilters );
    lblTo->setObjectName( QString::fromUtf8( "lblTo" ) );
    horizontalLayout1->addWidget( lblTo );

    dteEndDate = new QDateTimeEdit( grpFilters );
    dteEndDate->setObjectName( QString::fromUtf8( "dteEndDate" ) );
    dteEndDate->setCalendarPopup( true );
    dteEndDate->setDate( QDate::currentDate() );
    dteEndDate->setDisplayFormat( "yyyy-MM-dd" );
    horizontalLayout1->addWidget( dteEndDate );

    horizontalSpacer1 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout1->addItem( horizontalSpacer1 );

    //======================================================================================================

    horizontalLayout2 = new QHBoxLayout( grpFilters );
    horizontalLayout2->setObjectName( QString::fromUtf8( "horizontalLayout2" ) );

    lblAge = new QLabel( tr("Age :"), grpFilters );
    lblAge->setObjectName( QString::fromUtf8( "lblAge" ) );
    horizontalLayout2->addWidget( lblAge );

    cmbAge = new QComboBox( grpFilters );
    cmbAge->setObjectName( QString::fromUtf8( "cmbAge" ) );
    cmbAge->addItem( tr("<All patients>") );
    cmbAge->addItem( tr("0 year to 10 years") );
    cmbAge->addItem( tr("10 year to 20 years") );
    cmbAge->addItem( tr("20 year to 30 years") );
    cmbAge->addItem( tr("30 year to 40 years") );
    cmbAge->addItem( tr("40 year to 50 years") );
    cmbAge->addItem( tr("50 year to 60 years") );
    cmbAge->addItem( tr("60 year to 70 years") );
    cmbAge->addItem( tr("70 year to 80 years") );
    cmbAge->addItem( tr("80 year to 90 years") );
    cmbAge->addItem( tr("90 year to 100 years") );
    horizontalLayout2->addWidget( cmbAge );

    lblGender = new QLabel( tr("Gender :"), grpFilters );
    lblGender->setObjectName( QString::fromUtf8( "lblGender" ) );
    horizontalLayout2->addWidget( lblGender );

    rbBoth = new QRadioButton( tr("All"), grpFilters );
    rbBoth->setObjectName( QString::fromUtf8( "rbBoth" ) );
    rbBoth->setChecked( true );
    horizontalLayout2->addWidget( rbBoth );

    rbMale = new QRadioButton( tr("Male"), grpFilters );
    rbMale->setObjectName( QString::fromUtf8( "rbMale" ) );
    horizontalLayout2->addWidget( rbMale );

    rbFemale = new QRadioButton( tr("Female"), grpFilters );
    rbFemale->setObjectName( QString::fromUtf8( "rbFemale" ) );
    horizontalLayout2->addWidget( rbFemale );

    horizontalSpacer2 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout2->addItem( horizontalSpacer2 );

    //======================================================================================================

    horizontalLayout3 = new QHBoxLayout( grpFilters );
    horizontalLayout3->setObjectName( QString::fromUtf8( "horizontalLayout3" ) );

    chkRegularCustomer = new QCheckBox( tr("Regular customer"), grpFilters );
    chkRegularCustomer->setObjectName( QString::fromUtf8( "chkRegularCustomer" ) );
    horizontalLayout3->addWidget( chkRegularCustomer );

    chkEmployee = new QCheckBox( tr("Employee"), grpFilters );
    chkEmployee->setObjectName( QString::fromUtf8( "chkEmployee" ) );
    horizontalLayout3->addWidget( chkEmployee );

    chkService = new QCheckBox( tr("Service person"), grpFilters );
    chkService->setObjectName( QString::fromUtf8( "chkService" ) );
    horizontalLayout3->addWidget( chkService );

    horizontalSpacer3 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout3->addItem( horizontalSpacer3 );

    //======================================================================================================

    horizontalLayout4 = new QHBoxLayout( grpFilters );
    horizontalLayout4->setObjectName( QString::fromUtf8( "horizontalLayout4" ) );

    lblHealthInsurance = new QLabel( tr("Health Insurance Fund :"), grpFilters );
    lblHealthInsurance->setObjectName( QString::fromUtf8( "lblHealthInsurance" ) );
    horizontalLayout4->addWidget( lblHealthInsurance );

    cmbHealthInsurance = new QComboBox( grpFilters );
    cmbHealthInsurance->setObjectName( QString::fromUtf8( "cmbHealthInsurance" ) );
    cmbHealthInsurance->addItem( tr("<All Health Insurance Fund>"), 0 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT healthInsuranceId, name FROM healthInsurances WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    while( poQuery->next() )
    {
        cmbHealthInsurance->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }
    horizontalLayout4->addWidget( cmbHealthInsurance );

    horizontalSpacer4 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout4->addItem( horizontalSpacer4 );

    //======================================================================================================

    horizontalLayout5 = new QHBoxLayout( grpFilters );
    horizontalLayout5->setObjectName( QString::fromUtf8( "horizontalLayout5" ) );

    lblCompany = new QLabel( tr("Company :"), grpFilters );
    lblCompany->setObjectName( QString::fromUtf8( "lblCompany" ) );
    horizontalLayout5->addWidget( lblCompany );

    cmbCompany = new QComboBox( grpFilters );
    cmbCompany->setObjectName( QString::fromUtf8( "cmbCompany" ) );
    cmbCompany->addItem( tr("<All Company>"), 0 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT companyId, name FROM companies WHERE active=1 AND archive<>\"DEL\" ORDER BY name" ) );
    while( poQuery->next() )
    {
        cmbCompany->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }
    horizontalLayout5->addWidget( cmbCompany );

    horizontalSpacer5 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout5->addItem( horizontalSpacer5 );

    //======================================================================================================

    horizontalLayout6 = new QHBoxLayout( grpFilters );
    horizontalLayout6->setObjectName( QString::fromUtf8( "horizontalLayout6" ) );

    lblDoctor = new QLabel( tr("Proposal person :"), grpFilters );
    lblDoctor->setObjectName( QString::fromUtf8( "lblDoctor" ) );
    horizontalLayout6->addWidget( lblDoctor );

    cmbDoctor = new QComboBox( grpFilters );
    cmbDoctor->setObjectName( QString::fromUtf8( "cmbDoctor" ) );
    cmbDoctor->addItem( tr("<All proposal person>"), 0 );
    poQuery = g_poDB->executeQTQuery( QString( "SELECT doctorId, doctorLicence, name FROM doctors WHERE active=1 AND archive<>\"DEL\" ORDER BY doctorLicence" ) );
    while( poQuery->next() )
    {
        cmbDoctor->addItem( poQuery->value( 1 ).toString(), poQuery->value( 0 ) );
    }
    horizontalLayout6->addWidget( cmbDoctor );

    horizontalSpacer6 = new QSpacerItem( 400, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    horizontalLayout6->addItem( horizontalSpacer6 );

    //======================================================================================================

    verticalLayout1->insertLayout( 0, horizontalLayout1 );
    verticalLayout1->insertLayout( 1, horizontalLayout2 );
    verticalLayout1->insertLayout( 2, horizontalLayout3 );
    verticalLayout1->insertLayout( 3, horizontalLayout4 );
    verticalLayout1->insertLayout( 4, horizontalLayout5 );
    verticalLayout1->insertLayout( 5, horizontalLayout6 );

    horizontalLayout->insertLayout( 0, verticalLayout1 );

    if( poQuery ) delete poQuery;
}

cDlgReportPatients::~cDlgReportPatients()
{
    cTracer obTrace( "cDlgReportPatients::~cDlgReportPatients" );
}

void cDlgReportPatients::refreshReport()
{
    cTracer obTrace( "cDlgReportPatients::refreshReport()" );

    setCursor( Qt::WaitCursor);

    m_tdReport.clear();
    m_tdReport.setMetaInformation( QTextDocument::DocumentTitle, m_qsReportName );

    QTextCharFormat obTitleFormat;
    obTitleFormat.setFontPointSize( 14.0 );
    obTitleFormat.setFontWeight( QFont::Bold );

    QTextCharFormat obNormalFormat;
    obNormalFormat.setFontPointSize( 10.0 );
    obNormalFormat.setFontWeight( QFont::Normal );

    QTextCharFormat obBoldFormat;
    obBoldFormat.setFontPointSize( 10.0 );
    obBoldFormat.setFontWeight( QFont::Bold );

    QTextTableFormat obTableFormatCenter;
    obTableFormatCenter.setHeaderRowCount( 1 );
    obTableFormatCenter.setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormatCenter.setAlignment( Qt::AlignHCenter );

    QTextTableFormat obTableFormatLeft;
    obTableFormatLeft.setHeaderRowCount( 1 );
    obTableFormatLeft.setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormatLeft.setAlignment( Qt::AlignLeft );

    QTextBlockFormat obLeftCellFormat;
    obLeftCellFormat.setLeftMargin( 10 );
    obLeftCellFormat.setRightMargin( 10 );
    obLeftCellFormat.setAlignment( Qt::AlignLeft );

    QTextBlockFormat obRightCellFormat;
    obRightCellFormat.setLeftMargin( 10 );
    obRightCellFormat.setRightMargin( 10 );
    obRightCellFormat.setAlignment( Qt::AlignRight );

    QTextBlockFormat obCenterCellFormat;
    obCenterCellFormat.setLeftMargin( 10 );
    obCenterCellFormat.setRightMargin( 10 );
    obCenterCellFormat.setAlignment( Qt::AlignCenter );

    QTextCursor tcReport( &m_tdReport );

    tcReport.insertText( m_qsReportName + "   ", obTitleFormat );
    tcReport.setCharFormat( obNormalFormat );
    tcReport.insertText( QString( "%1 %2 -> " ).arg( tr( "Date:" ) ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ) );
    tcReport.insertText( dteEndDate->date().toString( "yyyy-MM-dd" ) );

    tcReport.insertHtml( "<hr>" );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    QString qsCondition = "";

    qsCondition += QString( "AND created>=\"%1\" AND created<=\"%2\" " ).arg( dteStartDate->date().toString( "yyyy-MM-dd" ) ).arg( dteEndDate->date().toString( "yyyy-MM-dd" ) );
    if( cmbAge->currentIndex() > 0 )
    {
        qsCondition += QString( "AND dateBirth>\"%1\" AND dateBirth<\"%2\" " ).arg( QDate::currentDate().addYears(-(cmbAge->currentIndex()*10)).toString("yyyy-MM-dd") ).arg( QDate::currentDate().addYears(-((cmbAge->currentIndex()-1)*10)).toString("yyyy-MM-dd") );
    }
    if( rbMale->isChecked() )
    {
        qsCondition += QString( "AND gender=1 " );
    }
    else if( rbFemale->isChecked() )
    {
        qsCondition += QString( "AND gender=2 " );
    }
    if( chkRegularCustomer->isChecked() )
    {
        qsCondition += QString( "AND regularCustomer>0 " );
    }
    if( chkEmployee->isChecked() )
    {
        qsCondition += QString( "AND employee>0 " );
    }
    if( chkService->isChecked() )
    {
        qsCondition += QString( "AND service>0 " );
    }
    if( cmbHealthInsurance->currentIndex() > 0 )
    {
        qsCondition += QString( "AND healthInsuranceId>0 " );
    }
    if( cmbCompany->currentIndex() > 0 )
    {
        qsCondition += QString( "AND companyId>0 " );
    }
    if( cmbDoctor->currentIndex() > 0 )
    {
        qsCondition += QString( "AND doctorId>0 " );
    }

    QString qsCellTitle = "";
    int     inRowCount = 1;

    if( rbBoth->isChecked() )
        inRowCount += 2;
    if( !chkRegularCustomer->isChecked() )
        inRowCount++;
    if( !chkEmployee->isChecked() )
        inRowCount++;
    if( !chkService->isChecked() )
        inRowCount++;
    if( cmbHealthInsurance->currentIndex() == 0 )
        inRowCount++;
    if( cmbCompany->currentIndex() == 0 )
        inRowCount++;
    if( cmbDoctor->currentIndex() == 0 )
        inRowCount++;

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    tcReport.insertTable( inRowCount, 3, obTableFormatLeft );

    qsQuery = "";
    qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
    qsQuery += qsCondition;

    qsCellTitle = tr( "Total number of patients" );

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );
    poReportResult->first();
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( qsCellTitle, obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obCenterCellFormat );
    tcReport.insertText( ":", obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.setBlockFormat( obRightCellFormat );
    tcReport.insertText( poReportResult->value(0).toString(), obBoldFormat );
    tcReport.movePosition( QTextCursor::NextCell );

    //------------------------------------------------------------------------------------------------------
    if( !qsCondition.contains("gender=", Qt::CaseInsensitive) )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND gender=1 " );

        qsCellTitle = tr( "Number of male patients" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );

        //------------------------------------------------------------------------------------------------------
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND gender=2 " );

        qsCellTitle = tr( "Number of female patients" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( !qsCondition.contains("regularCustomer>0", Qt::CaseInsensitive) )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND regularCustomer>0 " );

        qsCellTitle = tr( "Number of regular customers" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( !qsCondition.contains("employee>0", Qt::CaseInsensitive) )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND employee>0 " );

        qsCellTitle = tr( "Number of employees" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( !qsCondition.contains("service>0", Qt::CaseInsensitive) )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND service>0 " );

        qsCellTitle = tr( "Number of service persons" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( cmbHealthInsurance->currentIndex() == 0 )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND healthInsuranceId>0 " );

        qsCellTitle = tr( "Number of health insurance fund's members" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( cmbCompany->currentIndex() == 0 )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND companyId>0 " );

        qsCellTitle = tr( "Number of company employees" );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------
    if( cmbDoctor->currentIndex() == 0 )
    {
        qsQuery = "";
        qsQuery += QString( "SELECT COUNT(name) FROM patients WHERE active=1 " );
        qsQuery += qsCondition;
        qsQuery += QString( "AND doctorId>0 " );

        qsCellTitle = tr( "Number of patients proposed by ..." );

        poReportResult = NULL;
        poReportResult = g_poDB->executeQTQuery( qsQuery );
        poReportResult->first();
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( qsCellTitle, obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obCenterCellFormat );
        tcReport.insertText( ":", obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obRightCellFormat );
        tcReport.insertText( poReportResult->value(0).toString(), obNormalFormat );
        tcReport.movePosition( QTextCursor::NextCell );
    }

    //------------------------------------------------------------------------------------------------------

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================
    //
    //
    //
    //======================================================================================================

    qsQuery = "";
    qsQuery += QString( "SELECT name, gender, dateBirth FROM patients WHERE active=1 " );
    qsQuery += qsCondition;

    //------------------------------------------------------------------------------------------------------

    poReportResult = NULL;
    poReportResult = g_poDB->executeQTQuery( qsQuery );

    if( rbBoth->isChecked() )
        uiColumnCount = 4;
    else
        uiColumnCount = 3;

    tcReport.insertTable( poReportResult->size() + 2, uiColumnCount, obTableFormatLeft );

    //------------------------------------------------------------------------------------------------------
    // Headers
    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Name" ), obBoldFormat );

    if( rbBoth->isChecked() )
    {
        tcReport.movePosition( QTextCursor::NextCell );

        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( tr( "Gender" ), obBoldFormat );
    }

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Date of birth" ), obBoldFormat );

    tcReport.movePosition( QTextCursor::NextCell );

    tcReport.setBlockFormat( obLeftCellFormat );
    tcReport.insertText( tr( "Age" ), obBoldFormat );

    //------------------------------------------------------------------------------------------------------
    // Summary variables

    //------------------------------------------------------------------------------------------------------

    while( poReportResult->next() )
    {
        int         inColumn = 0;

        // Name
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );
        inColumn++;

        if( rbBoth->isChecked() )
        {
            // Gender
            tcReport.movePosition( QTextCursor::NextCell );
            tcReport.setBlockFormat( obLeftCellFormat );
            if( poReportResult->value(inColumn).toInt() == 1 )
                tcReport.insertText( tr("Male"), obNormalFormat );
            else if( poReportResult->value(inColumn).toInt() == 2 )
                tcReport.insertText( tr("Female"), obNormalFormat );
        }
        inColumn++;

        // Date of birth
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( poReportResult->value(inColumn).toString(), obNormalFormat );

        // Age
        QDate   qdDays = QDate::fromString( poReportResult->value(inColumn).toString(), "yyyy-MM-dd" );
        QDate   qdAge  = QDate( 1900, 1, 1 );
        qdAge = qdAge.addDays( qdDays.daysTo(QDate::currentDate()) );
        tcReport.movePosition( QTextCursor::NextCell );
        tcReport.setBlockFormat( obLeftCellFormat );
        tcReport.insertText( QString::number(qdAge.year()-1900), obNormalFormat );
        inColumn++;
    }
    delete poReportResult;

    //------------------------------------------------------------------------------------------------------
    // Summary fields
    tcReport.movePosition( QTextCursor::NextCell );
    if( rbBoth->isChecked() )
        tcReport.movePosition( QTextCursor::NextCell );
    tcReport.movePosition( QTextCursor::NextCell );
    tcReport.movePosition( QTextCursor::NextCell );

    //======================================================================================================
    //
    //======================================================================================================

    tcReport.movePosition( QTextCursor::NextBlock );
    tcReport.insertHtml( "<hr>" );
    tcReport.movePosition( QTextCursor::NextBlock );

    //======================================================================================================

    setCursor( Qt::ArrowCursor);
}
