
#include "../framework/qtframework.h"
#include "creportdevicepatientdispersion.h"


cReportDevicePatientDispersion::cReportDevicePatientDispersion(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Device patient dispersion ");
    m_qsReportDescription   = tr( "This report shows the dispersion of the patients per hour on the selected date intervall." );

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

void cReportDevicePatientDispersion::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QStringList qslFilterPanelType = filterType().split("|");

    QString qsQuery = QString( "SELECT title, SUM(timeReal), SUM(timeLeft), SUM(timeCard), SUM(timeCash), SUM(units) FROM panels, ledgerdevice WHERE "
                               "panels.panelId=ledgerdevice.panelId AND "
                               "panels.panelId>0 AND "
                               "DATE(ledgerTime)>=\"%1\" AND "
                               "DATE(ledgerTime)<=\"%2\" " ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));

    if( qslFilterPanelType.at(0).toInt() > 0 )
    {
        qsQuery.append( QString(" AND panels.panelTypeId=%1 ").arg( qslFilterPanelType.at(0).toInt() ) );
    }

    qsQuery.append( " GROUP BY ledgerdevice.panelId " );

    QSqlQuery *poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressValue( 90 );

    startReport();

    addTitle( m_qsReportName );
    addHorizontalLine();

    m_dlgProgress.setProgressMax( poQueryResult->size()+1 );
    m_dlgProgress.setProgressValue( 0 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr("Device"), "bold" );
    addTableCell( tr("Planned time"), "center bold" );
    addTableCell( tr("Real time"), "center bold" );
    addTableCell( tr("Aborted time"), "center bold" );
    addTableCell( tr("Payed by cash"), "center bold" );
    addTableCell( tr("Used by card"), "center bold" );
    addTableCell( tr("No. units used"), "center bold" );

    while( poQueryResult->next() )
    {
        unsigned int hour       = 0;
        unsigned int minute     = 0;
        unsigned int second     = 0;

        int timeReal    = poQueryResult->value(1).toInt();
        int timeLeft    = poQueryResult->value(2).toInt();
        int timeCard    = poQueryResult->value(3).toInt();
        int timeCash    = poQueryResult->value(4).toInt();;
        int timeSum     = timeCard + timeCash;
        int countUnits  = poQueryResult->value(5).toInt();

        if( timeLeft < 0 ) timeLeft = 0;

        addTableRow();
        addTableCell( poQueryResult->value(0).toString() );

        hour       = timeSum/3600;
        minute     = (timeSum-(hour*3600))/60;
        second     = (timeSum-(hour*3600))%60;

        addTableCell( QString( "%1%2" ).arg( hour ).arg( QTime(0,minute,second).toString(":mm:ss") ), "center" );

        hour       = timeReal/3600;
        minute     = (timeReal-(hour*3600))/60;
        second     = (timeReal-(hour*3600))%60;

        addTableCell( QString( "%1%2" ).arg( hour ).arg( QTime(0,minute,second).toString(":mm:ss") ), "center" );

        hour       = timeLeft/3600;
        minute     = (timeLeft-(hour*3600))/60;
        second     = (timeLeft-(hour*3600))%60;

        addTableCell( QString( "%1%2" ).arg( hour ).arg( QTime(0,minute,second).toString(":mm:ss") ), "center" );

        hour       = timeCash/3600;
        minute     = (timeCash-(hour*3600))/60;
        second     = (timeCash-(hour*3600))%60;

        addTableCell( QString( "%1%2" ).arg( hour ).arg( QTime(0,minute,second).toString(":mm:ss") ), "center" );

        hour       = timeCard/3600;
        minute     = (timeCard-(hour*3600))/60;
        second     = (timeCard-(hour*3600))%60;

        addTableCell( QString( "%1%2" ).arg( hour ).arg( QTime(0,minute,second).toString(":mm:ss") ), "center" );

        addTableCell( QString::number( countUnits ), "center" );

        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
