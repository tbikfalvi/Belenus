
#include <QMessageBox>

#include "../framework/qtframework.h"
#include "creportdaily.h"
#include "currency.h"

//------------------------------------------------------------------------------------
cReportDaily::cReportDaily(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
//------------------------------------------------------------------------------------
{
    m_qsReportName          = tr( " Daily ledger " );
    m_qsReportDescription   = tr( "This report shows the daily bookkeepings the cash book and every accounting data for the selected day. "
                                  "Please select the day you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("Date :") );
    m_qdStartDate = QDate::currentDate();

    setFilterIsVisible( false );
    _setDataIsVisibleEnabled( true );
    _setDataIsVisibleText( tr("Calculate only with the selected day's entries") );
}
//------------------------------------------------------------------------------------
void cReportDaily::refreshReport()
//------------------------------------------------------------------------------------
{
    m_qslCassaIds.clear();
    m_qslCassaOwners.clear();

    // General report init
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    startReport();

    addTitle( m_qsReportName );
    addSubTitle( QString( "%1 %2" ).arg( tr( "Date :" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    // Main data about cassa entries of the selected date
    addSeparator();
    addSubTitle( tr( "Shift list" ) );
    _reportPartGeneral();

    // Product sold
    addSeparator();
    addSubTitle( tr( "Products sold" ) );
    unsigned int uiProductSoldTotal = _reportPartProductSell();

    // Patientcards sold
    addSeparator();
    addSubTitle( tr( "Patientcards sold" ) );
    unsigned int uiPatientCardTotal = _reportPartPatientCardSell();

    // Panel related data of the selected date
    addSeparator();
    addSubTitle( tr( "Occasionally usage of devices" ) );
    unsigned int uiDeviceUsagesTotal = _reportPartPanelUse();

    // Income based on payment methods
    addSeparator();
    addSubTitle( tr( "Income by payment methods" ) );
    int nIncomeCard = _reportPartPaymentMethods();

    // Cassa expenses
    addSeparator();
    addSubTitle( tr( "Expenses" ) );
    int nExpenses = _reportPartExpenses();

    // Income summary
    addSeparator();
    addSubTitle( tr( "Income summary" ) );
    _reportPartIncomeSummary( uiProductSoldTotal, uiPatientCardTotal, uiDeviceUsagesTotal, nExpenses, nIncomeCard );

    addHorizontalLine();

    // Withdrawed items (storno)
    addSeparator();
    addSubTitle( tr( "Storno items" ) );
    addDescription( tr( "The items in the storno list are automatically handled, the "
                        "related values automatically processed by the application. "
                        "No further calculation or process is needed related to these items.") );
    _reportPartStorno();

    // Panel usage by patientcard units
    addSeparator();
    addSubTitle( tr( "Device usages by patientcard units" ) );
    _reportPartPanelUseType( PU_USE_WITH_CARD );

    // Panel usage by cash
    addSeparator();
    addSubTitle( tr( "Device usages by cash" ) );
    _reportPartPanelUseType( PU_USE_WITH_CASH );

    // Panel usage by cash and units
    addSeparator();
    addSubTitle( tr( "Device usages by patientcard units and cash" ) );
    _reportPartPanelUseType( PU_USE_COMBINED );

    // Used patientcard unit
    addSeparator();
    addSubTitle( tr( "Number of patientcard units used" ) );
    _reportPartUsedPatientcardunits();

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartGeneral()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;
    QString     qsDateCondition = "";

//    if( filterIsVisible() )
//    {
//        qsDateCondition = QString( "DATE(cassa.startDateTime)=\"%1\"  " ).arg( filterDateStart().toString( "yyyy-MM-dd" ) );
//    }
//    else
//    {
        qsDateCondition = QString( "( DATE(cassa.startDateTime)=\"%1\" OR "
                                   "DATE(cassa.stopDateTime)=\"%1\" OR "
                                   "DATE(cassa.modified)=\"%1\" ) " ).arg( filterDateStart().toString( "yyyy-MM-dd" ) );
//    }

    qsQuery = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName, cassa.userId "
                       "FROM cassa, users "
                       "WHERE cassa.userId=users.userId AND %1 "
                       "ORDER BY cassa.startDateTime " ).arg(qsDateCondition);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    m_dlgProgress.setProgressMax( poQueryResult->size()+11 );
    m_dlgProgress.setProgressValue( 10 );

    startSection();
    addTable();

    addTableRow();
    addTableCell( tr( "Owner" ), "bold" );
    addTableCell( tr( "Cassa started" ), "center bold" );
    addTableCell( tr( "Cassa closed" ), "center bold" );
    addTableCell( tr( "Last entry" ), "center bold" );
    addTableCell( tr( "Balance" ), "center bold" );
    addTableCell( tr( "Income" ), "center bold" );
    m_dlgProgress.increaseProgressValue();

    int nSumTotal = 0;

    while( poQueryResult->next() )
    {
        m_qslCassaIds << poQueryResult->value( 0 ).toString();
        m_qslCassaOwners << poQueryResult->value( 5 ).toString();

        cCurrency       cBalance( poQueryResult->value(1).toInt() );
        unsigned int    uiCassaId   = poQueryResult->value(0).toUInt();
        int             nSum = _sumCassaIncome(uiCassaId);
        cCurrency       obIncome( nSum );

        nSumTotal += nSum;

        addTableRow();
        addTableCell( poQueryResult->value(5).toString() );
        addTableCell( poQueryResult->value(2).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResult->value(3).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( poQueryResult->value(4).toDateTime().toString("yyyy-MM-dd hh:mm"), "center" );
        addTableCell( cBalance.currencyFullStringShort(), "right" );
        addTableCell( obIncome.currencyFullStringShort(), "right" );
        m_dlgProgress.increaseProgressValue();
    }

    cCurrency obSumTotal( nSumTotal );

    addTableRow();
    addTableCell( tr( "Sum total" ), "bold" );
    addTableCell();
    addTableCell();
    addTableCell();
    addTableCell();
    addTableCell( obSumTotal.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartProductSell()
//------------------------------------------------------------------------------------
{
    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell();
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Amount"), "right bold" );

    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        unsigned int    uiCassaId       = m_qslCassaIds.at(i).toUInt();
        QString         qsUserName      = m_qslCassaOwners.at(i);
        QString         qsDateCondition = "";

        addTableRow();
        addTableCell( qsUserName, "bold" );
        addTableCell();
        addTableCell();
        addTableCell();

        if( filterIsVisible() )
        {
            qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
        }

        QSqlQuery  *poQueryProducts = g_poDB->executeQTQuery( QString( "SELECT name, SUM(itemCount), SUM(totalPrice) "
                                                                       "FROM cassahistory, ledger WHERE "
                                                                       "cassahistory.ledgerId=ledger.ledgerId AND "
                                                                       "cassahistory.ledgerId>0 AND "
                                                                       "cassaId=%1 AND "
                                                                       "ledgerTypeId=%2 AND "
                                                                       "ledger.active=1 "
                                                                       " %3 "
                                                                       "GROUP BY productId" ).arg( uiCassaId ).arg( LT_PROD_SELL ).arg( qsDateCondition ) );
        unsigned int uiTotalCassa = 0;
        while( poQueryProducts->next() )
        {
            cCurrency   obPrice( poQueryProducts->value(2).toInt() );
            uiTotalCassa += poQueryProducts->value(2).toInt();

            addTableRow();
            addTableCell();
            addTableCell( poQueryProducts->value(0).toString() );
            addTableCell( poQueryProducts->value(1).toString(), "center" );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
        cCurrency   obTotalPriceAm( uiTotalCassa );

        addTableRow();
        addTableCell();
        addTableCell( tr("Sum"), "bold" );
        addTableCell();
        addTableCell( obTotalPriceAm.currencyFullStringShort(), "right bold" );

        uiTotal += uiTotalCassa;
    }

    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotal;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
    unsigned int    uiTotalCardSell = 0;
    QSqlQuery      *poQueryResult;
    unsigned int    uiCountPCSell[ m_qslCassaIds.count() ];
    unsigned int    uiSumPCSell[ m_qslCassaIds.count() ];

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    for( int i=0; i<m_qslCassaOwners.count(); i++ )
    {
        uiCountPCSell[ i ] = 0;
        uiSumPCSell[ i ] = 0;
        addTableCell( m_qslCassaOwners.at(i), "center bold" );
    }
    addTableCell( tr("Sum"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM patientcardtypes WHERE patientCardTypeId>1" );

    while( poQueryResult->next() )
    {
        unsigned int    uiCountPCTSum = 0;
        unsigned int    uiPricePCTSum = 0;
        QString         qsPricePCTSum = "";
        QStringList     qslCells = QStringList();

        for( int i=0; i<m_qslCassaIds.count(); i++ )
        {
            unsigned int uiPricePCSell = 0;
            QString      qsCountPCSell = _countsumPatientCardTypeSell( m_qslCassaIds.at(i), poQueryResult->value(0).toUInt(), &uiPricePCSell );

            if( uiPricePCSell > 0 )
            {
                cCurrency   obPricePCSell( uiPricePCSell );

                qslCells << QString( "%1 / %2" ).arg( qsCountPCSell ).arg( obPricePCSell.currencyFullStringShort() );
            }
            else
            {
                qslCells << "";
            }

            uiCountPCSell[ i ] += qsCountPCSell.toInt();
            uiSumPCSell[ i ]   += uiPricePCSell;

            uiCountPCTSum += qsCountPCSell.toInt();
            uiPricePCTSum += uiPricePCSell;
        }

        if( uiPricePCTSum > 0 )
        {
            addTableRow();
            addTableCell( poQueryResult->value(2).toString() );

            for( int i=0; i<qslCells.count(); i++ )
            {
                addTableCell( qslCells.at(i), "right" );
            }

            cCurrency   obPricePCTSum( uiPricePCTSum );

            qsPricePCTSum = obPricePCTSum.currencyFullStringShort();
            addTableCell( QString( "%1 / %2" ).arg( uiCountPCTSum ).arg( qsPricePCTSum ) );
        }

        uiTotalCardSell += uiPricePCTSum;
    }

    cCurrency obTotalCardSell( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        cCurrency   obPricePCTSum( uiSumPCSell[ i ] );

        addTableCell( QString( "%1 / %2" ).arg( uiCountPCSell[ i ] ).arg( obPricePCTSum.currencyFullStringShort() ), "right bold" );
    }
    addTableCell( obTotalCardSell.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotalCardSell;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartPanelUse()
//------------------------------------------------------------------------------------
{
    unsigned int    uiPriceSumTotal = 0;
    QSqlQuery      *poQueryResult;
    unsigned int    uiPricePanelUse[ m_qslCassaIds.count() ];

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    for( int i=0; i<m_qslCassaOwners.count(); i++ )
    {
        addTableCell( m_qslCassaOwners.at(i), "center bold" );
        uiPricePanelUse[ i ] = 0;
    }
    addTableCell( tr("Amount"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM panelgroups WHERE panelGroupId>0" );

    while( poQueryResult->next() )
    {
        unsigned int    uiPricePanelTotal = 0;
        QString         qsPricePanelTotal = "";
        QString         qsTitlePanel = poQueryResult->value(2).toString();

        addTableRow();
        addTableCell( qsTitlePanel );

        for( int i=0; i<m_qslCassaIds.count(); i++ )
        {
            int             nPricePanel  = _sumPanelUse( m_qslCassaIds.at(i), poQueryResult->value(0).toUInt() );
            QString         qsPricePanel = "";

            if( nPricePanel > 0 )
            {
                cCurrency obPrice( nPricePanel );

                qsPricePanel = obPrice.currencyFullStringShort();
            }
            addTableCell( qsPricePanel, "right" );

            uiPricePanelUse[ i ] += nPricePanel;
            uiPricePanelTotal += nPricePanel;
        }

        if( uiPricePanelTotal > 0 )
        {
            cCurrency   obPanelUseFullday( uiPricePanelTotal );

            qsPricePanelTotal = obPanelUseFullday.currencyFullStringShort();
        }

        addTableCell( qsPricePanelTotal, "right" );

        uiPriceSumTotal += uiPricePanelTotal;
    }

    cCurrency   obPriceSumTotal( uiPriceSumTotal );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        cCurrency   obPricePanel( uiPricePanelUse[ i ] );

        addTableCell( obPricePanel.currencyFullStringShort(), "right bold" );
    }
    addTableCell( obPriceSumTotal.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiPriceSumTotal;
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartPanelUseType(tePanelUse p_tePanelUse)
//------------------------------------------------------------------------------------
{
    unsigned int    uiCountSumTotal = 0;
    QSqlQuery      *poQueryResult;
    unsigned int    uiCountPanelUseUnits[ m_qslCassaIds.count() ];

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    for( int i=0; i<m_qslCassaOwners.count(); i++ )
    {
        addTableCell( m_qslCassaOwners.at(i), "center bold" );
        uiCountPanelUseUnits[ i ] = 0;
    }
    addTableCell( tr("Sum"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM panelgroups WHERE panelGroupId>0" );

    while( poQueryResult->next() )
    {
        unsigned int    uiCountPanelTotal = 0;
        QString         qsTitlePanel = poQueryResult->value(2).toString();

        addTableRow();
        addTableCell( qsTitlePanel );

        for( int i=0; i<m_qslCassaIds.count(); i++ )
        {
            int nCountPanel  = _countPanelUse( m_qslCassaIds.at(i), poQueryResult->value(0).toUInt(), p_tePanelUse, PU_COUNT_GUEST );

            addTableCell( QString::number(nCountPanel), "center" );

            uiCountPanelUseUnits[ i ] += nCountPanel;
            uiCountPanelTotal += nCountPanel;
        }

        addTableCell( QString::number(uiCountPanelTotal), "center" );

        uiCountSumTotal += uiCountPanelTotal;
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        addTableCell( QString::number( uiCountPanelUseUnits[ i ] ), "center" );
    }
    addTableCell( QString::number(uiCountSumTotal), "center bold" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartUsedPatientcardunits()
//------------------------------------------------------------------------------------
{
    unsigned int    uiCountSumTotal = 0;
    QSqlQuery      *poQueryResult;
    unsigned int    uiCountPanelUseUnits[ m_qslCassaIds.count() ];

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    for( int i=0; i<m_qslCassaOwners.count(); i++ )
    {
        addTableCell( m_qslCassaOwners.at(i), "center bold" );
        uiCountPanelUseUnits[ i ] = 0;
    }
    addTableCell( tr("Sum"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM panelgroups WHERE panelGroupId>0" );

    while( poQueryResult->next() )
    {
        unsigned int    uiCountPanelTotal = 0;
        QString         qsTitlePanel = poQueryResult->value(2).toString();

        addTableRow();
        addTableCell( qsTitlePanel );

        for( int i=0; i<m_qslCassaIds.count(); i++ )
        {
            int nCountPanel  = _countPanelUse( m_qslCassaIds.at(i), poQueryResult->value(0).toUInt(), PU_USE_WITH_CARD, PU_COUNT_PCUNITS );

            addTableCell( QString::number(nCountPanel), "center" );

            uiCountPanelUseUnits[ i ] += nCountPanel;
            uiCountPanelTotal += nCountPanel;
        }

        addTableCell( QString::number(uiCountPanelTotal), "center" );

        uiCountSumTotal += uiCountPanelTotal;
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        addTableCell( QString::number( uiCountPanelUseUnits[ i ] ), "center" );
    }
    addTableCell( QString::number(uiCountSumTotal), "center bold" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
int cReportDaily::_reportPartPaymentMethods()
//------------------------------------------------------------------------------------
{
    startSection();
    addTable();
    addTableRow();
    addTableCell();
    addTableCell( tr("Payed by cash"), "center bold" );
    addTableCell( tr("Payed by card"), "center bold" );
    addTableCell( tr("Payed by voucher"), "center bold" );
    addTableCell( tr("Amount"), "center bold" );

    int inTotalCard     = 0;
    int inTotalCash     = 0;
    int inTotalVoucher  = 0;
    int inTotal         = 0;

    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        unsigned int    uiCassaId       = m_qslCassaIds.at(i).toUInt();
        QString         qsUserName      = m_qslCassaOwners.at(i);
        QString         qsDateCondition = "";

        addTableRow();
        addTableCell( qsUserName, "bold" );
        addTableCell();
        addTableCell();
        addTableCell();
        addTableCell();

        if( filterIsVisible() )
        {
            qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
        }

        QSqlQuery  *poQueryResults = g_poDB->executeQTQuery( QString( "SELECT ledgertypes.name, "
                                                                      "SUM(card), "
                                                                      "SUM(cash), "
                                                                      "SUM(voucher), "
                                                                      "SUM(totalPrice) "
                                                                      "FROM ledger, cassahistory, ledgertypes WHERE "
                                                                      "ledger.ledgerId=cassahistory.ledgerId AND "
                                                                      "ledger.ledgerTypeId=ledgertypes.ledgerTypeId AND "
                                                                      "ledger.totalPrice<>0 AND "
                                                                      "ledger.ledgerTypeId<%2 AND "
                                                                      "cassaId=%1 "
                                                                      "%3 "
                                                                      "GROUP BY ledgertypes.ledgerTypeId" ).arg( uiCassaId ).arg( LT_PROD_STORAGE_CHANGE ).arg(qsDateCondition) );
        int inSumCard       = 0;
        int inSumCash       = 0;
        int inSumVoucher    = 0;
        int inSumTotal      = 0;

        while( poQueryResults->next() )
        {
            cCurrency   obCard( poQueryResults->value(1).toInt() );
            cCurrency   obCash( poQueryResults->value(2).toInt() );
            cCurrency   obVoucher( poQueryResults->value(3).toInt() );
            cCurrency   obTotal( poQueryResults->value(4).toInt() );

            inSumCard       += poQueryResults->value(1).toInt();
            inSumCash       += poQueryResults->value(2).toInt();
            inSumVoucher    += poQueryResults->value(3).toInt();
            inSumTotal      += poQueryResults->value(4).toInt();

            addTableRow();
            addTableCell( poQueryResults->value(0).toString() );
            addTableCell( obCash.currencyFullStringShort(), "right italic" );
            addTableCell( obCard.currencyFullStringShort(), "right italic" );
            addTableCell( obVoucher.currencyFullStringShort(), "right italic" );
            addTableCell( obTotal.currencyFullStringShort(), "right bold" );
        }
        addTableRow();
        addTableCell( tr("Sum"), "bold" );

        cCurrency   obCash( inSumCash );
        cCurrency   obCard( inSumCard );
        cCurrency   obVoucher( inSumVoucher );
        cCurrency   obTotal( inSumTotal );

        addTableCell( obCash.currencyFullStringShort(), "right bold italic" );
        addTableCell( obCard.currencyFullStringShort(), "right bold italic" );
        addTableCell( obVoucher.currencyFullStringShort(), "right bold italic" );
        addTableCell( obTotal.currencyFullStringShort(), "right bold" );

        inTotalCash     += inSumCash;
        inTotalCard     += inSumCard;
        inTotalVoucher  += inSumVoucher;
        inTotal         += inSumTotal;
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );

    cCurrency   obCash( inTotalCash );
    cCurrency   obCard( inTotalCard );
    cCurrency   obVoucher( inTotalVoucher );
    cCurrency   obTotal( inTotal );

    addTableCell( obCash.currencyFullStringShort(), "right bold italic" );
    addTableCell( obCard.currencyFullStringShort(), "right bold italic" );
    addTableCell( obVoucher.currencyFullStringShort(), "right bold italic" );
    addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return inTotalCard;
}
//------------------------------------------------------------------------------------
unsigned int cReportDaily::_reportPartExpenses()
//------------------------------------------------------------------------------------
{
    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell();
    addTableCell( tr("Description"), "bold" );
    addTableCell( tr("Amount"), "right bold" );

    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        unsigned int    uiCassaId       = m_qslCassaIds.at(i).toUInt();
        QString         qsUserName      = m_qslCassaOwners.at(i);
        QString         qsDateCondition = "";

        addTableRow();
        addTableCell( qsUserName, "bold" );
        addTableCell();
        addTableCell();

        if( filterIsVisible() )
        {
            qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
        }

        QSqlQuery  *poQueryResults = g_poDB->executeQTQuery( QString( "SELECT comment, actionValue "
                                                                      "FROM cassahistory WHERE "
                                                                      "parentId=0 AND "
                                                                      "cassaId=%1 AND "
                                                                      "actionValue<0 AND "
                                                                      "ledgerId>0 "
                                                                      "%2 ").arg( uiCassaId ).arg(qsDateCondition) );
        unsigned int uiTotalCassa = 0;
        while( poQueryResults->next() )
        {
            cCurrency   obPrice( poQueryResults->value(1).toInt() );
            uiTotalCassa += poQueryResults->value(1).toInt();

            addTableRow();
            addTableCell();
            addTableCell( poQueryResults->value(0).toString() );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
        cCurrency   obTotalPrice( uiTotalCassa );

        addTableRow();
        addTableCell();
        addTableCell( tr("Sum"), "bold" );
        addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

        uiTotal += uiTotalCassa;
    }

    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotal;
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartStorno()
//------------------------------------------------------------------------------------
{
    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell();
    addTableCell( tr("Description"), "bold" );
    addTableCell( tr("Amount"), "right bold" );

    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
        unsigned int    uiCassaId       = m_qslCassaIds.at(i).toUInt();
        QString         qsUserName      = m_qslCassaOwners.at(i);
        QString         qsDateCondition = "";

        addTableRow();
        addTableCell( qsUserName, "bold" );
        addTableCell();
        addTableCell();

        if( filterIsVisible() )
        {
            qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
        }

        QSqlQuery  *poQueryResults = g_poDB->executeQTQuery( QString( "SELECT comment, actionValue "
                                                                      "FROM cassahistory WHERE "
                                                                      "parentId<>0 AND "
                                                                      "cassaId=%1 "
                                                                      "%2 ").arg( uiCassaId ).arg(qsDateCondition) );
        unsigned int uiTotalCassa = 0;
        while( poQueryResults->next() )
        {
            cCurrency   obPrice( abs(poQueryResults->value(1).toInt()) );
            uiTotalCassa += abs(poQueryResults->value(1).toInt());

            addTableRow();
            addTableCell();
            addTableCell( poQueryResults->value(0).toString() );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
        cCurrency   obTotalPrice( uiTotalCassa );

        addTableRow();
        addTableCell();
        addTableCell( tr("Sum"), "bold" );
        addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

        uiTotal += uiTotalCassa;
    }

    cCurrency   obTotalPrice( uiTotal );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell();
    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
void cReportDaily::_reportPartIncomeSummary( unsigned int p_uiProductSoldTotal, unsigned int p_uiPatientCardTotal, unsigned int p_uiDeviceUsagesTotal, int p_nExpenses, int p_nIncomeCard )
//------------------------------------------------------------------------------------
{
    startSection();
    addTable();

    cCurrency obPriceProducts = cCurrency( p_uiProductSoldTotal );

    addTableRow();
    addTableCell( tr("Products sold") );
    addTableCell( ":", "bold");
    addTableCell( obPriceProducts.currencyFullStringShort(), "right" );

    cCurrency obPricePatientcards = cCurrency( p_uiPatientCardTotal );

    addTableRow();
    addTableCell( tr("Patientcards sold") );
    addTableCell( ":", "bold");
    addTableCell( obPricePatientcards.currencyFullStringShort(), "right" );

    cCurrency obPriceDevices = cCurrency( p_uiDeviceUsagesTotal );

    addTableRow();
    addTableCell( tr("Device usages") );
    addTableCell( ":", "bold");
    addTableCell( obPriceDevices.currencyFullStringShort(), "right" );

    cCurrency obPriceExpenses = cCurrency( p_nExpenses );

    addTableRow();
    addTableCell( tr("Expenses") );
    addTableCell( ":", "bold");
    addTableCell( obPriceExpenses.currencyFullStringShort(), "right" );

    cCurrency   obTotal( p_uiProductSoldTotal + p_uiPatientCardTotal + p_uiDeviceUsagesTotal + p_nExpenses );
    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( ":", "bold");
    addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    cCurrency obPriceCashProfit = cCurrency( p_uiProductSoldTotal + p_uiPatientCardTotal + p_uiDeviceUsagesTotal + p_nExpenses - p_nIncomeCard );

    addTableRow();
    addTableCell( tr("Cash/voucher") );
    addTableCell( ":", "bold");
    addTableCell( obPriceCashProfit.currencyFullStringShort(), "right" );

    cCurrency obPriceCardProfit = cCurrency( p_nIncomeCard );

    addTableRow();
    addTableCell( tr("Card") );
    addTableCell( ":", "bold");
    addTableCell( obPriceCardProfit.currencyFullStringShort(), "right" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
QString cReportDaily::_countsumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId, unsigned int *p_uiPricePCSell )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet           = "";
    QString         qsPCCondition   = QString( "(ledgerTypeId=%1 OR ledgerTypeId=%2 OR ledgerTypeId=%3 OR ledgerTypeId=%4)" ).arg(LT_PC_SELL).arg(LT_PC_REFILL).arg(LT_PC_LOST_REPLACE).arg(LT_PC_ASSIGN_PARTNER);
    QString         qsDateCondition = "";

    if( filterIsVisible() )
    {
        qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
    }

    qsQuery = QString("SELECT COUNT(totalPrice), SUM(totalPrice) "
                      "FROM cassahistory, ledger, patientCardTypes WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "cassahistory.cassaId=%1 AND "
                      "%2 AND "
                      "ledger.patientCardTypeId=%3 AND "
                      "ledger.active=1 "
                      "%4 " ).arg(p_qsCassaId).arg(qsPCCondition).arg(p_uiPatientCardTypeId).arg(qsDateCondition);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        if( poQueryResult->value(0).toInt() > 0 )
        {
            qsRet = QString::number( poQueryResult->value(0).toInt() );
            *p_uiPricePCSell = poQueryResult->value(1).toUInt();
        }
    }

    return qsRet;
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nRet = 0;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM cassahistory, ledger, patientCardTypes WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "cassahistory.cassaId=%1 AND "
                      "ledgerTypeId=%2 AND "
                      "ledger.patientCardTypeId=%3 AND "
                      "ledger.active=1 " ).arg(p_qsCassaId).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        nRet = poQueryResult->value(0).toInt();
    }

    return nRet;
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumUserIncome( unsigned int p_uiUserId, QString p_qsStart, QString p_qsStop )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    if( p_qsStop.length() == 0 )
    {
        p_qsStop = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
    }

    qsQuery = QString( "SELECT SUM(totalPrice) "
                       "FROM ledger WHERE "
                       "userId=%1 AND "
                       "ledgerTime>\"%2\" AND "
                       "ledgerTime<\"%3\" " ).arg(p_uiUserId).arg(p_qsStart).arg(p_qsStop);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPanelUse( QString p_qsCassaId, unsigned int p_uiPanelGroupId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsDateCondition = "";

    if( filterIsVisible() )
    {
        qsDateCondition = QString(" AND actionTime>\"%1 00:00:00\" AND actionTime<\"%1 23:59:59\" ").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
    }

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM cassahistory, ledger, panels WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.panelId=panels.panelId AND "
                      "cassahistory.cassaId=%1 AND "
                      "ledgerTypeId=%2 AND "
                      "panelGroupId=%3 AND "
                      "ledger.active=1 "
                      "%4" ).arg( p_qsCassaId ).arg( LT_DEVICE_USAGE ).arg( p_uiPanelGroupId ).arg(qsDateCondition);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPaymentMethod( QString p_qsCassaId, unsigned int p_uiPaymentMethodId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, cassahistory WHERE "
                      "ledger.ledgerId=cassahistory.ledgerId AND "
                      "cassaId=%1 AND "
                      "ledgerTypeId>0 AND "
                      "ledgerTypeId<9 AND "
                      "paymentMethodId=%2 AND "
                      "ledger.active=1 " ).arg(p_qsCassaId).arg(p_uiPaymentMethodId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumPaymentMethodVoucher( QString p_qsCassaId, unsigned int p_uiPaymentMethodId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(voucher) "
                      "FROM ledger, cassahistory WHERE "
                      "ledger.ledgerId=cassahistory.ledgerId AND "
                      "cassaId=%1 AND "
                      "ledgerTypeId>0 AND "
                      "ledgerTypeId<9 AND "
                      "paymentMethodId=%2 AND "
                      "ledger.active=1 " ).arg(p_qsCassaId).arg(p_uiPaymentMethodId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_sumCassaIncome( unsigned int p_uiCassaId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(actionValue) "
                      "FROM cassahistory WHERE "
                      "cassaId>0 AND "
                      "ledgerId>0 AND "
                      "cassaId=%1 ").arg(p_uiCassaId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportDaily::_countPanelUse(QString p_qsCassaId, unsigned int p_uiPanelgroupId, tePanelUse p_tePanelUse, tePanelCountType p_tePanelCountType)
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT * FROM cassa WHERE cassaId=%1 ").arg(p_qsCassaId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    QString qsStart = poQueryResult->value(4).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
    QString qsStop  = poQueryResult->value(5).toDateTime().toString( "yyyy-MM-dd hh:mm:ss" );

    if( filterIsVisible() )
    {
        qsStart = QString("%1 00:00:00").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
        qsStop  = QString("%1 23:59:59").arg( filterDateStart().toString( "yyyy-MM-dd" ) );
    }

    if( qsStop.length() == 0 )
    {
        qsStop = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );
    }

    QString qsData  = "";
    QString qsCond  = "";

    if( p_tePanelCountType == PU_COUNT_GUEST )
    {
        qsData = "COUNT(ledgerDeviceId)";
        switch( p_tePanelUse )
        {
            case PU_USE_WITH_CARD:
                qsCond = "units>0 AND cash=0 AND ";
                break;
            case PU_USE_WITH_CASH:
                qsCond = "units=0 AND cash>0 AND ";
                break;
            case PU_USE_COMBINED:
                qsCond = "units>0 AND cash>0 AND ";
                break;
        }
    }
    else
    {
        qsData = "SUM(units)";
        qsCond = "";
    }

    qsQuery = QString("SELECT %1 FROM "
                      "ledgerdevice, panels WHERE "
                      "ledgerdevice.panelId = panels.panelId AND "
                      "%2 "
                      "panelGroupId=%3 AND "
                      "ledgerTime>\"%4\" AND "
                      "ledgerTime<=\"%5\" ").arg(qsData).arg( qsCond ).arg( p_uiPanelgroupId ).arg( qsStart ).arg( qsStop );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
