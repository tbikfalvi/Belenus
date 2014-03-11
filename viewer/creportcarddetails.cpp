
#include <QMessageBox>

#include "../framework/qtframework.h"
#include "creportcarddetails.h"

cReportCardDetails::cReportCardDetails(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Patientcards details ");
    m_qsReportDescription   = tr( "This report shows the details of the selected patient card. "
                                  "Please enter the barcode of the patient card you interested in." );

    _setDataNameEnabled( true );
    _setDataNameLabelText( tr("Barcode contains :") );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Patientcard types :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All type") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT patientCardTypeId, name FROM patientcardtypes WHERE patientCardTypeId>1 AND active=1" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( QString( "0|%1" ).arg( tr("All type") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}

void cReportCardDetails::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    QString     qsTitle = "";
    QString     qsCondition;
    QStringList qslFilterType = filterType().split("|");

    if( qslFilterType.at(0).toInt() > 0 )
    {
        qsTitle.append( tr( "Patientcard type: '%1'" ).arg( qslFilterType.at(1) ) );
        qsCondition.append( QString(" AND patientcards.patientCardTypeId=%1 ").arg( qslFilterType.at(0).toInt() ) );
    }
    if( filterName().length() > 0 )
    {
        qsTitle.append( tr( "%1Patientcard barcode contains: '%2'" ).arg( qsTitle.length()?"  -  ":"" ).arg( filterName() ) );
        qsCondition.append( QString(" AND patientcards.barcode LIKE \"\%%1\%\" ").arg(filterName()) );
    }
    m_dlgProgress.increaseProgressValue();

    QString      qsQueryCards = "SELECT * FROM patientcards, patientcardtypes, patients WHERE "
                                "patientCardId>0 AND "
                                "patientcards.active=1 AND "
                                "patientcards.patientCardTypeId=patientcardtypes.patientCardTypeId AND "
                                "patientcards.patientId=patients.patientId";

    if( qsCondition.length() > 0 )
    {
        qsQueryCards.append( qsCondition );
    }

    QSqlQuery   *poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
    QStringList  qslQueryResult;

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    if( qsTitle.length() )
        addSubTitle( qsTitle );
    addHorizontalLine();

    m_dlgProgress.setProgressValue( 100 );

    if( poQueryResultCards->size() < 1 )
    {
        startSection();
        addTable();
        addTableRow();
        addTableCell( tr("No valid patientcard found for the selected filters"), "center italic" );
        finishTable();
        finishSection();
    }
    else if( poQueryResultCards->size() == 1 )
    {
        m_dlgProgress.setProgressMax( 7 );
        m_dlgProgress.setProgressValue( 0 );

        poQueryResultCards->first();

        unsigned int    uiPatientCardId = poQueryResultCards->value(0).toUInt();

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "Barcode" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( poQueryResultCards->value(5).toString() );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Patientcard type" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( poQueryResultCards->value(18).toString() );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Owner" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( poQueryResultCards->value(33).toString() );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "No. units" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( poQueryResultCards->value(7).toString() );
        m_dlgProgress.increaseProgressValue();

        unsigned int    uiTimeLeft = poQueryResultCards->value(9).toInt();
        QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );

        addTableRow();
        addTableCell( tr( "Time left" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( qtTemp.toString( "hh:mm:ss" ) );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Valid" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( QString("%1 -> %2").arg( poQueryResultCards->value(10).toString() ).arg( poQueryResultCards->value(11).toString() ) );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Comment" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( poQueryResultCards->value(6).toString() );
        m_dlgProgress.increaseProgressValue();

        finishTable();
        finishSection();

        addSeparator();
        addSubTitle( tr( "Valid and active units" ) );

        m_dlgProgress.setProgressMax( 100 );
        m_dlgProgress.setProgressValue( 0 );

        qsQueryCards = QString( "SELECT patientCardUnitId, unitTime, validDateFrom, validDateTo, COUNT(unitTime) "
                                "FROM patientcardunits "
                                "WHERE patientCardId=%1 "
                                "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                "AND active=1 "
                                "GROUP BY unitTime, validDateTo ORDER BY validDateTo" ).arg( uiPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_dlgProgress.setProgressValue( 10 );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "Unit time" ), "center bold" );
        addTableCell( tr( "Valid" ), "center bold" );
        addTableCell( tr( "Valid till ..." ), "center bold" );
        addTableCell( tr( "No. units" ), "center bold" );

        m_dlgProgress.setProgressMax( poQueryResultCards->size()+10 );

        while( poQueryResultCards->next() )
        {
            addTableRow();
            addTableCell( poQueryResultCards->value(1).toString(), "center" );
            addTableCell( QString("%1 -> %2").arg( poQueryResultCards->value(2).toString() ).arg( poQueryResultCards->value(3).toString() ), "center" );
            addTableCell( tr("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(poQueryResultCards->value(3).toString(), "yyyy-MM-dd") ) ), "center" );
            addTableCell( poQueryResultCards->value(4).toString(), "center" );
            m_dlgProgress.increaseProgressValue();
        }
        finishTable();
        finishSection();

        addSeparator();
        addSubTitle( tr( "Patientcard unit usages" ) );

        m_dlgProgress.setProgressMax( 100 );
        m_dlgProgress.setProgressValue( 0 );

        qsQueryCards = QString( "SELECT dateTimeUsed, COUNT(dateTimeUsed) "
                                "FROM patientcardunits "
                                "WHERE patientCardId=%1 "
                                "AND active=0 "
                                "GROUP BY dateTimeUsed" ).arg( uiPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_dlgProgress.setProgressValue( 10 );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "Date of usage" ), "center bold" );
        addTableCell( tr( "Unit count" ), "center bold" );

        m_dlgProgress.setProgressMax( poQueryResultCards->size()+10 );

        while( poQueryResultCards->next() )
        {
            addTableRow();
            addTableCell( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm" ), "center" );
            addTableCell( poQueryResultCards->value(1).toString(), "center" );
            m_dlgProgress.increaseProgressValue();
        }
        finishTable();
        finishSection();
    }
    else
    {
        m_dlgProgress.setProgressMax( poQueryResultCards->size()*2+1 );
        m_dlgProgress.setProgressValue( 0 );

        while( poQueryResultCards->next() )
        {
            QStringList qslRecord;

            qslRecord << poQueryResultCards->value(5).toString();
            qslRecord << poQueryResultCards->value(7).toString();
            qslRecord << poQueryResultCards->value(9).toString();
            qslRecord << poQueryResultCards->value(10).toString();
            qslRecord << poQueryResultCards->value(11).toString();
            qslRecord << poQueryResultCards->value(18).toString();
            qslRecord << poQueryResultCards->value(6).toString();

            qslQueryResult << qslRecord.join("#");
            m_dlgProgress.increaseProgressValue();
        }

        startSection();
        addTable();

        // Add table header row
        addTableRow();
        addTableCell( tr( "Barcode" ), "center bold" );
        addTableCell( tr( "No. units"), "center bold"  );
        addTableCell( tr( "Time" ), "center bold" );
        addTableCell( tr( "Valid" ), "center bold" );
        addTableCell( tr( "Patientcard type" ), "bold" );
        addTableCell( tr( "Comment" ), "bold" );

        for( int i=0; i<qslQueryResult.size(); i++ )
        {
            QStringList     qslRecord = qslQueryResult.at(i).split('#');
            unsigned int    uiTimeLeft = qslRecord.at(2).toInt();
            QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );

            addTableRow();
            addTableCell( qslRecord.at(0), "center" );
            addTableCell( qslRecord.at(1), "center" );
            addTableCell( qtTemp.toString( "hh:mm:ss" ), "center" );
            addTableCell( QString("%1 -> %2").arg( qslRecord.at(3) ).arg( qslRecord.at(4) ), "center" );
            addTableCell( qslRecord.at(5) );
            addTableCell( qslRecord.at(6) );

            m_dlgProgress.increaseProgressValue();
        }
        finishTable();
        finishSection();
    }

    finishReport();

    m_dlgProgress.hide();
}
