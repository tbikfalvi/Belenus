
#include <QMessageBox>

#include "../framework/qtframework.h"
#include "creportcarddetails.h"
#include "currency.h"

cReportCardDetails::cReportCardDetails(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
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

    QString      qsQueryCards = "SELECT "
                                    "patientCardId, "
                                    "patientcards.validDateFrom, "
                                    "barcode, "
                                    "patients.name, "
                                    "patientcards.validDateTo, "
                                    "patientcards.comment, "
                                    "patientcards.units, "
                                    "patientcards.timeLeft "
                                "FROM patientcards, patientcardtypes, patients WHERE "
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

        unsigned int    queryPatientCardId      = poQueryResultCards->value(0).toUInt();
        QString         queryCardValidDateFrom  = poQueryResultCards->value(1).toString();
        QString         queryBarcode            = poQueryResultCards->value(2).toString();
        QString         queryPatientName        = poQueryResultCards->value(3).toString();
        QString         queryCardValidDateTo    = poQueryResultCards->value(4).toString();
        QString         queryCardComment        = poQueryResultCards->value(5).toString();

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "Barcode" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( queryBarcode );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Owner" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( queryPatientName );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Valid" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( QString("%1 -> %2").arg( queryCardValidDateFrom ).arg( queryCardValidDateTo ) );
        m_dlgProgress.increaseProgressValue();

        addTableRow();
        addTableCell( tr( "Comment" ), "bold" );
        addTableCell( " : ", "bold" );
        addTableCell( queryCardComment );
        m_dlgProgress.increaseProgressValue();

        finishTable();
        finishSection();

        addSeparator();
        addSubTitle( tr( "Valid and active units" ) );

        m_dlgProgress.setProgressMax( 100 );
        m_dlgProgress.setProgressValue( 0 );

        qsQueryCards = QString( "SELECT "
                                "patientCardUnitId, "
                                "patientcardunits.unitTime, "
                                "patientcardunits.validDateFrom, "
                                "patientcardunits.validDateTo, "
                                "COUNT(patientcardunits.unitTime), "
                                "name "
                                "FROM patientcardunits, patientcardtypes "
                                "WHERE "
                                "patientcardunits.patientCardTypeId=patientcardtypes.patientCardTypeId AND "
                                "patientCardId=%1 AND "
                                "patientcardunits.validDateFrom<=CURDATE() AND "
                                "patientcardunits.validDateTo>=CURDATE() AND "
                                "patientcardunits.active=1 "
                                "GROUP BY "
                                "patientcardunits.unitTime, "
                                "patientcardunits.validDateTo, "
                                "patientcardunits.patientCardTypeId "
                                "ORDER BY validDateTo" ).arg( queryPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_dlgProgress.setProgressValue( 10 );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "No. units" ), "center bold" );
        addTableCell( tr( "Unit time" ), "center bold" );
        addTableCell( tr( "Unit type" ), "center bold" );
        addTableCell( tr( "Valid" ), "center bold" );
        addTableCell( tr( "Valid till ..." ), "center bold" );

        m_dlgProgress.setProgressMax( poQueryResultCards->size()+10 );

        int nSumUnits = 0;

        while( poQueryResultCards->next() )
        {
            addTableRow();
            addTableCell( poQueryResultCards->value(4).toString(), "center" );
            addTableCell( poQueryResultCards->value(1).toString(), "center" );
            addTableCell( poQueryResultCards->value(5).toString(), "center" );
            addTableCell( QString("%1 -> %2").arg( poQueryResultCards->value(2).toString() ).arg( poQueryResultCards->value(3).toString() ), "center" );
            addTableCell( tr("%1 day(s)").arg( QDate::currentDate().daysTo( QDate::fromString(poQueryResultCards->value(3).toString(), "yyyy-MM-dd") ) ), "center" );

            nSumUnits += poQueryResultCards->value(4).toInt();

            m_dlgProgress.increaseProgressValue();
        }

        addTableRow();
        addTableCell( QString::number( nSumUnits ), "center bold" );
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( "", "" );

        finishTable();
        finishSection();

        addSeparator();
        addSubTitle( tr( "Patientcard life history" ) );

        g_poDB->executeQTQuery( "DELETE FROM `report_cardhistory` " );

        startSection();
        addTable();

        addTableRow();
        addTableCell( tr( "Date of action" ), "center bold" );
        addTableCell( tr( "Action name" ), "center bold" );
        addTableCell( tr( "Number of units" ), "center bold" );
        addTableCell( tr( "Unit time" ), "center bold" );
        addTableCell( tr( "Unit type" ), "center bold" );
        addTableCell( tr( "Price" ), "center bold" );
        addTableCell( tr( "Action performed by" ), "center bold" );

        m_dlgProgress.setProgressMax( 50 );
        m_dlgProgress.setProgressValue( 0 );

        //----------------------------------------------------------------------------------------------
        // First activated
        g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName` ) "
                                         " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7' ) " )
                                        .arg( queryCardValidDateFrom )
                                        .arg( tr( "Patientcard activated" ) )
                                        .arg( "" )
                                        .arg( "" )
                                        .arg( "" )
                                        .arg( "" )
                                        .arg( "" ) );

        //----------------------------------------------------------------------------------------------
        // Usages
        qsQueryCards = QString( "SELECT "
                                "patientcardunits.dateTimeUsed, "
                                "COUNT(patientcardunits.dateTimeUsed), "
                                "patientcardunits.unitTime, "
                                "name "
                                "FROM patientcardunits, patientcardtypes WHERE "
                                "patientcardunits.patientCardTypeId=patientcardtypes.patientCardTypeId AND "
                                "patientCardId=%1 AND "
                                "panelId>0 AND "
                                "patientcardunits.active=0 "
                                "GROUP BY "
                                "patientcardunits.dateTimeUsed, "
                                "patientcardunits.patientCardTypeId " ).arg( queryPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );

        m_dlgProgress.setProgressValue( 10 );
        m_dlgProgress.increaseProgressMax( poQueryResultCards->size() );

        while( poQueryResultCards->next() )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName` ) "
                                             " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7' ) " )
                                            .arg( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) )
                                            .arg( tr( "Patientcard usage" ) )
                                            .arg( QString( "-%1" ).arg( poQueryResultCards->value(1).toString() ) )
                                            .arg( poQueryResultCards->value(2).toString() )
                                            .arg( poQueryResultCards->value(3).toString().replace( QString("\'"), QString("\\\'") ) )
                                            .arg( "" )
                                            .arg( "" ) );

            m_dlgProgress.increaseProgressValue();
        }

        //----------------------------------------------------------------------------------------------
        // Unit sell, refill, increase
        qsQueryCards = QString( "SELECT ledger.ledgertime, "
                                "ledgertypes.name, "
                                "patientcardtypes.name, "
                                "COUNT( patientcardunits.unittime ), "
                                "patientcardunits.unittime, "
                                "ledger.totalprice, "
                                "users.realname "
                                "FROM `ledger` , `ledgertypes` , `patientcardtypes` , `users` , `patientcardunits` "
                                "WHERE ledger.ledgerid = patientcardunits.ledgerid "
                                "AND ledger.userid = users.userid "
                                "AND ledger.patientcardtypeid = patientcardtypes.patientcardtypeid "
                                "AND ledger.ledgertypeid = ledgertypes.ledgertypeid "
                                "AND (ledger.patientcardid = %1 OR patientcardunits.patientcardid = %1) "
                                "GROUP BY ledger.ledgerid" ).arg( queryPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
        m_dlgProgress.increaseProgressMax( poQueryResultCards->size() );

        while( poQueryResultCards->next() )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName` ) "
                                             " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7' ) " )
                                            .arg( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) )
                                            .arg( poQueryResultCards->value(1).toString() )
                                            .arg( poQueryResultCards->value(3).toString() )
                                            .arg( poQueryResultCards->value(4).toString() )
                                            .arg( poQueryResultCards->value(2).toString().replace( QString("\'"), QString("\\\'") ) )
                                            .arg( poQueryResultCards->value(5).toString() )
                                            .arg( poQueryResultCards->value(6).toString() ) );

            m_dlgProgress.increaseProgressValue();
        }

        //----------------------------------------------------------------------------------------------
        // Unit decrease
        qsQueryCards = QString( "SELECT ledger.ledgertime, "
                                "ledgertypes.name, "
                                "patientcardtypes.name, "
                                "COUNT( patientcardunits.unittime ), "
                                "patientcardunits.unittime, "
                                "ledger.totalprice, "
                                "users.realname "
                                "FROM `ledger` , `ledgertypes` , `patientcardtypes` , `users` , `patientcardunits` "
                                "WHERE ledger.ledgerid = patientcardunits.ledgerid "
                                "AND ledger.userid = users.userid "
                                "AND ledger.patientcardtypeid = patientcardtypes.patientcardtypeid "
                                "AND ledger.ledgertypeid = ledgertypes.ledgertypeid "
                                "AND (ledger.patientcardid = %1 OR patientcardunits.patientcardid = %1) "
                                "GROUP BY ledger.ledgerid" ).arg( queryPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
        m_dlgProgress.increaseProgressMax( poQueryResultCards->size() );

        while( poQueryResultCards->next() )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName` ) "
                                             " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7' ) " )
                                            .arg( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) )
                                            .arg( poQueryResultCards->value(1).toString() )
                                            .arg( poQueryResultCards->value(3).toString() )
                                            .arg( poQueryResultCards->value(4).toString() )
                                            .arg( poQueryResultCards->value(2).toString().replace( QString("\'"), QString("\\\'") ) )
                                            .arg( poQueryResultCards->value(5).toString() )
                                            .arg( poQueryResultCards->value(6).toString() ) );

            m_dlgProgress.increaseProgressValue();
        }

        //----------------------------------------------------------------------------------------------
        // Replace card
        qsQueryCards = QString( "SELECT ledger.ledgertime, "
                                "ledger.comment, "
                                "patientcardtypes.name, "
                                "ledger.totalprice, "
                                "users.realname "
                                "FROM `ledger`, `patientcardtypes`, `users` WHERE "
                                "ledger.userid = users.userid AND "
                                "ledger.patientcardtypeid = patientcardtypes.patientcardtypeid AND "
                                "(ledger.ledgertypeid=5 OR ledger.ledgertypeid=6) AND "
                                "ledger.patientcardid = %1" ).arg( queryPatientCardId );
        poQueryResultCards = g_poDB->executeQTQuery( qsQueryCards );
        m_dlgProgress.increaseProgressMax( poQueryResultCards->size() );

        while( poQueryResultCards->next() )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO `report_cardhistory` ( `dateCardAction`, `cardAction`, `countUnits`, `unitTime`, `unitType`, `priceAction`, `userName` ) "
                                             " VALUES ( '%1', '%2', '%3', '%4', '%5', '%6', '%7' ) " )
                                            .arg( poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) )
                                            .arg( poQueryResultCards->value(1).toString() )
                                            .arg( "" )
                                            .arg( "" )
                                            .arg( poQueryResultCards->value(2).toString().replace( QString("\'"), QString("\\\'") ) )
                                            .arg( poQueryResultCards->value(3).toString() )
                                            .arg( poQueryResultCards->value(4).toString() ) );

            m_dlgProgress.increaseProgressValue();
        }

        //----------------------------------------------------------------------------------------------
        // Show history
        poQueryResultCards = g_poDB->executeQTQuery( "SELECT dateCardAction, cardAction, countUnits, unitTime, unitType, priceAction, userName FROM `report_cardhistory` ORDER BY dateCardAction " );

        int nSumCount = 0;
        int nSumPrice = 0;

        while( poQueryResultCards->next() )
        {
            QString     queryDateCardAction = poQueryResultCards->value(0).toDateTime().toString( "yyyy-MM-dd hh:mm" );
            QString     queryCardAction     = poQueryResultCards->value(1).toString();
            int         queryUnitCount      = poQueryResultCards->value(2).toInt();
            int         queryUnitTime       = poQueryResultCards->value(3).toInt();
            QString     queryUnitType       = poQueryResultCards->value(4).toString();
            int         queryPriceAction    = poQueryResultCards->value(5).toInt();
            QString     queryUserName       = poQueryResultCards->value(6).toString();

            cCurrency   obPrice( queryPriceAction );

            addTableRow();
            addTableCell( queryDateCardAction, "center" );
            addTableCell( queryCardAction, "" );
            if( queryUnitCount == 0 )
                addTableCell( "", "" );
            else
                addTableCell( QString::number( queryUnitCount ), "center" );
            if( queryUnitTime == 0 )
                addTableCell( "", "" );
            else
                addTableCell( QString::number( queryUnitTime ), "center" );
            addTableCell( queryUnitType, "" );
            if( queryPriceAction == 0 )
                addTableCell( "", "" );
            else
                addTableCell( obPrice.currencyFullStringShort(), "right" );
            addTableCell( queryUserName, "" );

            nSumCount += queryUnitCount;
            nSumPrice += queryPriceAction;
        }

        cCurrency   obSumPrice( nSumPrice );

        addTableRow();
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( QString::number( nSumCount ), "center bold" );
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( obSumPrice.currencyFullStringShort(), "right bold" );
        addTableCell( "", "" );

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

            QString         queryCardValidDateFrom  = poQueryResultCards->value(1).toString();
            QString         queryBarcode            = poQueryResultCards->value(2).toString();
            QString         queryPatientName        = poQueryResultCards->value(3).toString();
            QString         queryCardValidDateTo    = poQueryResultCards->value(4).toString();
            QString         queryCardComment        = poQueryResultCards->value(5).toString();
            QString         queryCardUnits          = poQueryResultCards->value(6).toString();
            QString         queryCardTimeLeft       = poQueryResultCards->value(7).toString();

            qslRecord << queryBarcode;
            qslRecord << queryCardUnits;
            qslRecord << queryCardTimeLeft;
            qslRecord << queryCardValidDateFrom;
            qslRecord << queryCardValidDateTo;
            qslRecord << queryPatientName;
            qslRecord << queryCardComment;

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
        addTableCell( tr( "Owner" ), "bold" );
        addTableCell( tr( "Comment" ), "bold" );

        int nSumUnits = 0;

        for( int i=0; i<qslQueryResult.size(); i++ )
        {
            QStringList     qslRecord = qslQueryResult.at(i).split('#');
            unsigned int    uiTimeLeft = qslRecord.at(2).toInt();
            QTime           qtTemp( uiTimeLeft/3600, (uiTimeLeft%3600)/60, (uiTimeLeft%3600)%60, 0 );
            int             nUnits = qslRecord.at(1).toInt();

            if( nUnits < 0 )
                nUnits = 0;

            addTableRow();
            addTableCell( qslRecord.at(0), "center" );
            addTableCell( QString::number(nUnits), "center" );
            addTableCell( qtTemp.toString( "hh:mm:ss" ), "center" );
            addTableCell( QString("%1 -> %2").arg( qslRecord.at(3) ).arg( qslRecord.at(4) ), "center" );
            addTableCell( qslRecord.at(5) );
            addTableCell( qslRecord.at(6) );

            nSumUnits += nUnits;

            m_dlgProgress.increaseProgressValue();
        }

        addTableRow();
        addTableCell( tr( "Sum" ), "center bold" );
        addTableCell( QString::number(nSumUnits), "center bold"  );
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( "", "" );
        addTableCell( "", "" );

        finishTable();
        finishSection();
    }

    finishReport();

    m_dlgProgress.hide();
}
