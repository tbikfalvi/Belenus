
#include "../framework/qtframework.h"
#include "creportdevicepatientdispersion.h"


cReportDevicePatientDispersion::cReportDevicePatientDispersion(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Device patient dispersion ");
    m_qsReportDescription   = tr( "This report shows the dispersion of the patients per hour on the selected date intervall." );

//    _setDataTypeEnabled( true );
//    _setDataTypeLabelText( tr("Panel type :") );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate();

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();
/*
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
*/
}

void cReportDevicePatientDispersion::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

//    QStringList qslFilterPanelType = filterType().split("|");

    QString qsQuery = QString( "SELECT ledgerTime, DATE(ledgerTime), HOUR(ledgerTime), COUNT(ledgerTime) "
                               "FROM `ledgerdevice` WHERE "
                               "DATE(ledgerTime)>=\"%1\" AND "
                               "DATE(ledgerTime)<=\"%2\" " ).arg(filterDateStart().toString( "yyyy-MM-dd" )).arg(filterDateStop().toString( "yyyy-MM-dd" ));
/*
    if( qslFilterPanelType.at(0).toInt() > 0 )
    {
        qsQuery.append( QString(" AND panels.panelTypeId=%1 ").arg( qslFilterPanelType.at(0).toInt() ) );
    }
*/
    qsQuery.append( "GROUP BY DATE(ledgerTime), HOUR(ledgerTime) " );

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
    addTableCell( tr("Date"), "bold" );
    addTableCell( tr("0"), "center bold" );
    addTableCell( tr("1"), "center bold" );
    addTableCell( tr("2"), "center bold" );
    addTableCell( tr("3"), "center bold" );
    addTableCell( tr("4"), "center bold" );
    addTableCell( tr("5"), "center bold" );
    addTableCell( tr("6"), "center bold" );
    addTableCell( tr("7"), "center bold" );
    addTableCell( tr("8"), "center bold" );
    addTableCell( tr("9"), "center bold" );
    addTableCell( tr("10"), "center bold" );
    addTableCell( tr("11"), "center bold" );
    addTableCell( tr("12"), "center bold" );
    addTableCell( tr("13"), "center bold" );
    addTableCell( tr("14"), "center bold" );
    addTableCell( tr("15"), "center bold" );
    addTableCell( tr("16"), "center bold" );
    addTableCell( tr("17"), "center bold" );
    addTableCell( tr("18"), "center bold" );
    addTableCell( tr("19"), "center bold" );
    addTableCell( tr("20"), "center bold" );
    addTableCell( tr("21"), "center bold" );
    addTableCell( tr("22"), "center bold" );
    addTableCell( tr("23"), "center bold" );

    QString actualDate = "";
    int     countUsage[ 24 ];
    int     sumUsage[ 24 ];
    int     i;
    int     rowCount = 0;

    for( i=0; i<24; i++ )
    {
        countUsage[i]   = 0;
        sumUsage[i]     = 0;
    }

    while( poQueryResult->next() )
    {
        if( actualDate.compare( poQueryResult->value(1).toString() ) != 0 )
        {
            if( actualDate.length() == 0 )
            {
                actualDate = poQueryResult->value(1).toString();
            }
            else
            {
                rowCount++;
                addTableRow();
                addTableCell( actualDate );
                addTableCell( QString::number( countUsage[0] ), "center" );
                addTableCell( QString::number( countUsage[1] ), "center" );
                addTableCell( QString::number( countUsage[2] ), "center" );
                addTableCell( QString::number( countUsage[3] ), "center" );
                addTableCell( QString::number( countUsage[4] ), "center" );
                addTableCell( QString::number( countUsage[5] ), "center" );
                addTableCell( QString::number( countUsage[6] ), "center" );
                addTableCell( QString::number( countUsage[7] ), "center" );
                addTableCell( QString::number( countUsage[8] ), "center" );
                addTableCell( QString::number( countUsage[9] ), "center" );
                addTableCell( QString::number( countUsage[10] ), "center" );
                addTableCell( QString::number( countUsage[11] ), "center" );
                addTableCell( QString::number( countUsage[12] ), "center" );
                addTableCell( QString::number( countUsage[13] ), "center" );
                addTableCell( QString::number( countUsage[14] ), "center" );
                addTableCell( QString::number( countUsage[15] ), "center" );
                addTableCell( QString::number( countUsage[16] ), "center" );
                addTableCell( QString::number( countUsage[17] ), "center" );
                addTableCell( QString::number( countUsage[18] ), "center" );
                addTableCell( QString::number( countUsage[19] ), "center" );
                addTableCell( QString::number( countUsage[20] ), "center" );
                addTableCell( QString::number( countUsage[21] ), "center" );
                addTableCell( QString::number( countUsage[22] ), "center" );
                addTableCell( QString::number( countUsage[23] ), "center" );

                actualDate = poQueryResult->value(1).toString();
                for( i=0; i<24; i++ )
                {
                    sumUsage[i] += countUsage[i];
                    countUsage[i] = 0;
                }
            }
        }

        i = poQueryResult->value(2).toInt();

        if( i > -1 && i < 24 ) countUsage[i] = poQueryResult->value(3).toInt();

        m_dlgProgress.increaseProgressValue();
    }

    rowCount++;
    addTableRow();
    addTableCell( actualDate );
    addTableCell( QString::number( countUsage[0] ), "center" );
    addTableCell( QString::number( countUsage[1] ), "center" );
    addTableCell( QString::number( countUsage[2] ), "center" );
    addTableCell( QString::number( countUsage[3] ), "center" );
    addTableCell( QString::number( countUsage[4] ), "center" );
    addTableCell( QString::number( countUsage[5] ), "center" );
    addTableCell( QString::number( countUsage[6] ), "center" );
    addTableCell( QString::number( countUsage[7] ), "center" );
    addTableCell( QString::number( countUsage[8] ), "center" );
    addTableCell( QString::number( countUsage[9] ), "center" );
    addTableCell( QString::number( countUsage[10] ), "center" );
    addTableCell( QString::number( countUsage[11] ), "center" );
    addTableCell( QString::number( countUsage[12] ), "center" );
    addTableCell( QString::number( countUsage[13] ), "center" );
    addTableCell( QString::number( countUsage[14] ), "center" );
    addTableCell( QString::number( countUsage[15] ), "center" );
    addTableCell( QString::number( countUsage[16] ), "center" );
    addTableCell( QString::number( countUsage[17] ), "center" );
    addTableCell( QString::number( countUsage[18] ), "center" );
    addTableCell( QString::number( countUsage[19] ), "center" );
    addTableCell( QString::number( countUsage[20] ), "center" );
    addTableCell( QString::number( countUsage[21] ), "center" );
    addTableCell( QString::number( countUsage[22] ), "center" );
    addTableCell( QString::number( countUsage[23] ), "center" );

    for( i=0; i<24; i++ )
    {
        sumUsage[i] += countUsage[i];
    }

    addTableRow();
    addTableCell( tr("Sum"), "bold" );
    addTableCell( QString::number( sumUsage[0] ), "center bold" );
    addTableCell( QString::number( sumUsage[1] ), "center bold" );
    addTableCell( QString::number( sumUsage[2] ), "center bold" );
    addTableCell( QString::number( sumUsage[3] ), "center bold" );
    addTableCell( QString::number( sumUsage[4] ), "center bold" );
    addTableCell( QString::number( sumUsage[5] ), "center bold" );
    addTableCell( QString::number( sumUsage[6] ), "center bold" );
    addTableCell( QString::number( sumUsage[7] ), "center bold" );
    addTableCell( QString::number( sumUsage[8] ), "center bold" );
    addTableCell( QString::number( sumUsage[9] ), "center bold" );
    addTableCell( QString::number( sumUsage[10] ), "center bold" );
    addTableCell( QString::number( sumUsage[11] ), "center bold" );
    addTableCell( QString::number( sumUsage[12] ), "center bold" );
    addTableCell( QString::number( sumUsage[13] ), "center bold" );
    addTableCell( QString::number( sumUsage[14] ), "center bold" );
    addTableCell( QString::number( sumUsage[15] ), "center bold" );
    addTableCell( QString::number( sumUsage[16] ), "center bold" );
    addTableCell( QString::number( sumUsage[17] ), "center bold" );
    addTableCell( QString::number( sumUsage[18] ), "center bold" );
    addTableCell( QString::number( sumUsage[19] ), "center bold" );
    addTableCell( QString::number( sumUsage[20] ), "center bold" );
    addTableCell( QString::number( sumUsage[21] ), "center bold" );
    addTableCell( QString::number( sumUsage[22] ), "center bold" );
    addTableCell( QString::number( sumUsage[23] ), "center bold" );

    addTableRow();
    addTableCell( tr("Average"), "bold italic" );
    addTableCell( QString::number( sumUsage[0]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[1]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[2]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[3]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[4]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[5]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[6]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[7]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[8]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[9]  / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[10] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[11] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[12] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[13] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[14] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[15] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[16] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[17] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[18] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[19] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[20] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[21] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[22] / rowCount ), "center bold italic" );
    addTableCell( QString::number( sumUsage[23] / rowCount ), "center bold italic" );

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
