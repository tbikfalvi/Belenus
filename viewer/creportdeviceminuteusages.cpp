
#include "../framework/qtframework.h"
#include "creportdeviceminuteusages.h"

cReportDeviceMinuteUsages::cReportDeviceMinuteUsages(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Device minute usages ");
    m_qsReportDescription   = tr( "This report shows the minute usage of the devices on the selected date intervall." );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Panel type :") );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

    QSqlQuery   *poQueryResult;
    QStringList  qslDataTypes;

    poQueryResult = g_poDB->executeQTQuery( "SELECT panelTypeId, name FROM paneltypes WHERE panelTypeId>0" );

    qslDataTypes << QString( "0|%1" ).arg( tr("All paneltypes") );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( QString( "0|%1" ).arg( tr("All paneltypes") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}

void cReportDeviceMinuteUsages::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QStringList  qslFilterPanelType = filterType().split("|");
    QStringList  qslMinutes;
    QStringList  qslPanelIds;
    QString      qsDeviceTitle = "";

    QSqlQuery   *poQueryResult;
    QString      qsQuery;
    int          i;

    qsQuery = "SELECT panelId FROM panels WHERE panels.panelId > 0";

    if( qslFilterPanelType.at(0).toInt() > 0 )
    {
        qsQuery.append( QString(" AND panelTypeId=%1 ").arg( qslFilterPanelType.at(0).toInt() ) );
    }

    poQueryResult   = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        qslPanelIds << poQueryResult->value(0).toString();
    }

	// SELECT panelId, COUNT(timeReal) as uses, timeReal FROM `ledgerdevice` WHERE timeLeft = 0 AND ledgerDeviceId > 0 GROUP BY panelId, timeReal
	
    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

//    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
//    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Device"), "bold" );

    poQueryResult   = g_poDB->executeQTQuery( "SELECT timeReal FROM ledgerdevice WHERE timeLeft = 0 AND ledgerDeviceId > 0 GROUP BY timeReal" );
    int nColums     = poQueryResult->size();

    for( i=0; i<nColums; i++ )
    {
        poQueryResult->next();
        addTableCell( tr( "%1 minutes" ).arg( poQueryResult->value(0).toInt()/60 ), "center bold" );
        qslMinutes << QString::number( poQueryResult->value(0).toInt() );
    }

    for( i=0; i<qslPanelIds.size(); i++ )
    {
        qsQuery = QString( "SELECT title, COUNT(timeReal) as uses, timeReal "
                           "FROM panels, ledgerdevice WHERE "
                           "timeLeft = 0 AND "
                           "ledgerDeviceId > 0 AND "
                           "panels.panelId=ledgerdevice.panelId AND "
                           "ledgerdevice.panelId = %1 AND "
                           "DATE(ledgerTime)>=\"%2\" AND "
                           "DATE(ledgerTime)<=\"%3\" "
                           "GROUP BY timeReal" ).arg( qslPanelIds.at(i) ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));

        poQueryResult = g_poDB->executeQTQuery( qsQuery );
        poQueryResult->first();
        addTableRow();
        addTableCell( poQueryResult->value(0).toString() );

        for( int j=0; j<nColums; j++ )
        {
            if( poQueryResult->value(2).toString().compare( qslMinutes.at(j) ) == 0 )
            {
                addTableCell( poQueryResult->value(1).toString(), "center" );
                poQueryResult->next();
            }
            else
            {
                addTableCell( "0", "center" );
            }
        }
    }

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
