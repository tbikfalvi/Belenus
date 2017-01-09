
#include "../framework/qtframework.h"
#include "creportledger.h"
#include "currency.h"

cReportLedger::cReportLedger(QWidget *parent, QString p_qsReportName, bool p_bIsAdmin) : cReport(parent,p_qsReportName,p_bIsAdmin)
{
    m_qsReportName          = tr(" Main ledger ");
    m_qsReportDescription   = tr( "This report shows the bookkeepings, the cash book and every accounting data for the selected date intervall. "
                                  "Please select the first and last day of the date intervall you interested in." );

    _setDateStartEnabled( true );
    _setDateStartLabelText( tr("First date of intervall :") );
    m_qdStartDate = QDate::currentDate().addMonths(-1);

    _setDateStopEnabled( true );
    _setDateStopLabelText( tr("Last date of intervall :") );
    m_qdStopDate = QDate::currentDate();

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Cassa owner :") );

    QStringList qslDataTypes;

    qslDataTypes << QString( "0|%1" ).arg( tr("All users") );

    QSqlQuery   *poQueryResult = g_poDB->executeQTQuery( "SELECT userId, realName FROM users WHERE userId>1 AND active=1" );

    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }

    setFilterDataType( QString( "0|%1" ).arg( tr("All users") ) );
    setFilterDataTypeList( qslDataTypes.join("#") );
}
//------------------------------------------------------------------------------------
void cReportLedger::refreshReport()
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
    addSubTitle( QString( "%1 %2 -> %3" ).arg( tr( "Date intervall:" ) ).arg( filterDateStart().toString( "yyyy MMM dd" ) ).arg( filterDateStop().toString( "yyyy MMM dd" ) ) );
    addHorizontalLine();

    addSeparator();
    addSubTitle( tr( "Shift list" ) );
    _reportPartCassaList();

    if( m_qslCassaIds.count() > 0 )
    {
        // Product sold
        addSeparator();
        addSubTitle( tr( "Products sold" ) );
        unsigned int uiProductSellTotal = _reportPartProductSell();

        // Patientcards sold
        addSeparator();
        addSubTitle( tr( "Patientcards sold / refilled / attached" ) );
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
        _reportPartIncomeSummary( uiProductSellTotal, uiPatientCardTotal, uiDeviceUsagesTotal, nExpenses, nIncomeCard );
    }

    finishReport();

    m_dlgProgress.hide();
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartCassaList()
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;
    QString     qsCondition     = "";
    QStringList qslFilterType   = filterType().split("|");

    if( qslFilterType.at(0).toInt() > 0 )
    {
        qsCondition = QString( "cassa.userId=%1 AND " ).arg( qslFilterType.at(0).toInt() );
    }

    qsQuery = QString( "SELECT cassaId, currentBalance, startDateTime, stopDateTime, cassa.modified, realName, cassa.userId "
                       "FROM cassa, users "
                       "WHERE %1 cassa.userId=users.userId AND ("
                       " (DATE(cassa.startDateTime)>=\"%2\" AND DATE(cassa.startDateTime)<=\"%3\") OR "
                       " (DATE(cassa.stopDateTime)>=\"%2\" AND DATE(cassa.stopDateTime)<=\"%3\") OR "
                       " (DATE(cassa.modified)>=\"%2\" AND DATE(cassa.modified)<=\"%3\") "
                       ") ORDER BY startDateTime" ).arg(qsCondition)
                                                   .arg(filterDateStart().toString( "yyyy-MM-dd" ))
                                                   .arg(filterDateStop().toString( "yyyy-MM-dd" ));
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
unsigned int cReportLedger::_reportPartProductSell()
//------------------------------------------------------------------------------------
{
//    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell( tr("Product name"), "bold" );
    addTableCell( tr("Count"), "center bold" );
    addTableCell( tr("Amount"), "right bold" );

//    for( int i=0; i<m_qslCassaIds.count(); i++ )
//    {
//        unsigned int    uiCassaId   = m_qslCassaIds.at(i).toUInt();
//        QString         qsUserName  = m_qslCassaOwners.at(i);

//        addTableRow();
//        addTableCell( qsUserName, "bold" );
//        addTableCell();
//        addTableCell();
//        addTableCell();

        QSqlQuery  *poQueryProducts = g_poDB->executeQTQuery( QString( "SELECT name, SUM(itemCount), SUM(totalPrice) "
                                                                       "FROM cassahistory, ledger WHERE "
                                                                       "cassahistory.ledgerId=ledger.ledgerId AND "
                                                                       "cassahistory.ledgerId>0 AND "
                                                                       "cassaId IN( %1 ) AND "
                                                                       "ledgerTypeId=%2 AND "
                                                                       "ledger.active=1 "
                                                                       "GROUP BY productId" ).arg( m_qslCassaIds.join(",") ).arg( LT_PROD_SELL ) );
        unsigned int uiTotalCassa = 0;
        while( poQueryProducts->next() )
        {
            cCurrency   obPrice( poQueryProducts->value(2).toInt() );
            uiTotalCassa += poQueryProducts->value(2).toInt();

            addTableRow();
            addTableCell( poQueryProducts->value(0).toString() );
            addTableCell( poQueryProducts->value(1).toString(), "center" );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
        cCurrency   obTotalPriceAm( uiTotalCassa );

        addTableRow();
        addTableCell( tr("Sum"), "bold" );
        addTableCell();
        addTableCell( obTotalPriceAm.currencyFullStringShort(), "right bold" );

//        uiTotal += uiTotalCassa;
//    }

//    cCurrency   obTotalPrice( uiTotal );

//    addTableRow();
//    addTableCell( tr("Sum total"), "bold" );
//    addTableCell();
//    addTableCell();
//    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotalCassa;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPatientCardSell()
//------------------------------------------------------------------------------------
{
    unsigned int    uiTotalCardSell = 0;
    QSqlQuery      *poQueryResult;
//    unsigned int    uiCountPCSell[ m_qslCassaIds.count() ];
//    unsigned int    uiSumPCSell[ m_qslCassaIds.count() ];

    startSection();
    addTable();

//    addTableRow();
//    addTableCell();
//    for( int i=0; i<m_qslCassaOwners.count(); i++ )
//    {
//        uiCountPCSell[ i ] = 0;
//        uiSumPCSell[ i ] = 0;
//        addTableCell( m_qslCassaOwners.at(i), "center bold" );
//    }
//    addTableCell( tr("Sum"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM patientcardtypes WHERE patientCardTypeId>1" );

    while( poQueryResult->next() )
    {
        unsigned int    uiCountPCTSum = 0;
        unsigned int    uiPricePCTSum = 0;
        QString         qsPricePCTSum = "";
//        QStringList     qslCells = QStringList();

//        for( int i=0; i<m_qslCassaIds.count(); i++ )
//        {
            unsigned int uiPricePCSell = 0;
//            QString      qsCountPCSell = _countsumPatientCardTypeSell( m_qslCassaIds.at(i), poQueryResult->value(0).toUInt(), &uiPricePCSell );
            QString      qsCountPCSell = _countsumPatientCardTypeSell( m_qslCassaIds.join(","), poQueryResult->value(0).toUInt(), &uiPricePCSell );

//            if( uiPricePCSell > 0 )
//            {
//                cCurrency   obPricePCSell( uiPricePCSell );
//
//                qslCells << QString( "%1 / %2" ).arg( qsCountPCSell ).arg( obPricePCSell.currencyFullStringShort() );
//            }
//            else
//            {
//                qslCells << "";
//            }
//
//            uiCountPCSell[ i ] += qsCountPCSell.toInt();
//            uiSumPCSell[ i ]   += uiPricePCSell;

            uiCountPCTSum += qsCountPCSell.toInt();
            uiPricePCTSum += uiPricePCSell;
//        }

        if( uiPricePCTSum > 0 || uiCountPCTSum > 0 )
        {
            addTableRow();
            addTableCell( poQueryResult->value(2).toString() );

//            for( int i=0; i<qslCells.count(); i++ )
//            {
//                addTableCell( qslCells.at(i), "right" );
//            }

            cCurrency   obPricePCTSum( uiPricePCTSum );

            qsPricePCTSum = obPricePCTSum.currencyFullStringShort();
            addTableCell( QString( "%1 / %2" ).arg( uiCountPCTSum ).arg( qsPricePCTSum ) );
        }

        uiTotalCardSell += uiPricePCTSum;
    }

    cCurrency obTotalCardSell( uiTotalCardSell );

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
//    for( int i=0; i<m_qslCassaIds.count(); i++ )
//    {
//        cCurrency   obPricePCTSum( uiSumPCSell[ i ] );

//        addTableCell( QString( "%1 / %2" ).arg( uiCountPCSell[ i ] ).arg( obPricePCTSum.currencyFullStringShort() ), "right bold" );
//    }
    addTableCell( obTotalCardSell.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotalCardSell;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartPanelUse()
//------------------------------------------------------------------------------------
{
    unsigned int    uiPriceSumTotal = 0;
    QSqlQuery      *poQueryResult;
//    unsigned int    uiPricePanelUse[ m_qslCassaIds.count() ];

    startSection();
    addTable();

    addTableRow();
    addTableCell();
//    for( int i=0; i<m_qslCassaOwners.count(); i++ )
//    {
//        addTableCell( m_qslCassaOwners.at(i), "center bold" );
//        uiPricePanelUse[ i ] = 0;
//    }
    addTableCell( tr("Amount"), "center bold" );

    poQueryResult = g_poDB->executeQTQuery( "SELECT * FROM panelgroups WHERE panelGroupId>0" );

    while( poQueryResult->next() )
    {
        unsigned int    uiPricePanelTotal = 0;
        QString         qsPricePanelTotal = "";
        QString         qsTitlePanel = poQueryResult->value(2).toString();

        addTableRow();
        addTableCell( qsTitlePanel );

//        for( int i=0; i<m_qslCassaIds.count(); i++ )
//        {
            int             nPricePanel  = _sumPanelUse( m_qslCassaIds.join(","), poQueryResult->value(0).toUInt() );
//            QString         qsPricePanel = "";

//            if( nPricePanel > 0 )
//            {
//                cCurrency obPrice( nPricePanel );
//
//                qsPricePanel = obPrice.currencyFullStringShort();
//            }
//            addTableCell( qsPricePanel, "right" );

//            uiPricePanelUse[ i ] += nPricePanel;
            uiPricePanelTotal += nPricePanel;
//        }

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
//    for( int i=0; i<m_qslCassaIds.count(); i++ )
//    {
//        cCurrency   obPricePanel( uiPricePanelUse[ i ] );
//
//        addTableCell( obPricePanel.currencyFullStringShort(), "right bold" );
//    }
    addTableCell( obPriceSumTotal.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiPriceSumTotal;
}
//------------------------------------------------------------------------------------
unsigned int cReportLedger::_reportPartExpenses()
//------------------------------------------------------------------------------------
{
    unsigned int uiTotal = 0;

    startSection();
    addTable();
    addTableRow();
    addTableCell( tr("Description"), "bold" );
    addTableCell( tr("Amount"), "right bold" );

//    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
//        unsigned int    uiCassaId   = m_qslCassaIds.at(i).toUInt();
//        QString         qsUserName  = m_qslCassaOwners.at(i);

//        addTableRow();
//        addTableCell( qsUserName, "bold" );
//        addTableCell();
//        addTableCell();

        QSqlQuery  *poQueryResults = g_poDB->executeQTQuery( QString( "SELECT comment, actionValue "
                                                                      "FROM cassahistory WHERE "
                                                                      "parentId=0 AND "
                                                                      "cassaId IN ( %1 ) AND "
                                                                      "actionValue<0 AND "
                                                                      "actionBalance<>0 AND "
                                                                      "ledgerId>0 " ).arg( m_qslCassaIds.join(",") ) );
        unsigned int uiTotalCassa = 0;
        while( poQueryResults->next() )
        {
            cCurrency   obPrice( poQueryResults->value(1).toInt() );
            uiTotalCassa += poQueryResults->value(1).toInt();

            addTableRow();
            addTableCell( poQueryResults->value(0).toString() );
            addTableCell( obPrice.currencyFullStringShort(), "right" );
        }
        cCurrency   obTotalPrice( uiTotalCassa );

        addTableRow();
        addTableCell( tr("Sum"), "bold" );
        addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

        uiTotal += uiTotalCassa;
    }

//    cCurrency   obTotalPrice( uiTotal );

//    addTableRow();
//    addTableCell( tr("Sum total"), "bold" );
//    addTableCell();
//    addTableCell( obTotalPrice.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return uiTotal;
}
//------------------------------------------------------------------------------------
/*void cReportLedger::_reportPartPanelUseType(tePanelUse p_tePanelUse)
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nCountTotal = 0;

    startSection();
    addTable();

    addTableRow();
    addTableCell();
    addTableCell( tr("Count"), "center bold" );

    qsQuery = QString( "SELECT * FROM paneltypes WHERE panelTypeId>0" );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    while( poQueryResult->next() )
    {
        int nCount = _countPanelUse( poQueryResult->value(0).toUInt(), p_tePanelUse );

        nCountTotal += nCount;

        addTableRow();
        addTableCell( poQueryResult->value(2).toString() );
        addTableCell( QString::number( nCount ), "center" );
    }

    addTableRow();
    addTableCell( tr("Sum total"), "bold" );
    addTableCell( QString::number(nCountTotal), "center bold" );

    finishTable();
    finishSection();
}*/
//------------------------------------------------------------------------------------
int cReportLedger::_reportPartPaymentMethods()
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

//    for( int i=0; i<m_qslCassaIds.count(); i++ )
    {
//        unsigned int    uiCassaId   = m_qslCassaIds.at(i).toUInt();
//        QString         qsUserName  = m_qslCassaOwners.at(i);

//        addTableRow();
//        addTableCell( qsUserName, "bold" );
//        addTableCell();
//        addTableCell();
//        addTableCell();
//        addTableCell();

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
                                                                      "cassaId IN( %1 ) "
                                                                      "GROUP BY ledgertypes.ledgerTypeId" ).arg( m_qslCassaIds.join(",") ).arg( LT_PROD_STORAGE_CHANGE ) );
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
            addTableCell( obCard.currencyFullStringShort(), "right" );
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
        addTableCell( obCard.currencyFullStringShort(), "right bold" );
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
    addTableCell( obCard.currencyFullStringShort(), "right bold" );
    addTableCell( obVoucher.currencyFullStringShort(), "right bold italic" );
    addTableCell( obTotal.currencyFullStringShort(), "right bold" );

    finishTable();
    finishSection();

    return inTotalCard;
}
//------------------------------------------------------------------------------------
void cReportLedger::_reportPartIncomeSummary( unsigned int p_uiProductSoldTotal, unsigned int p_uiPatientCardTotal, unsigned int p_uiDeviceUsagesTotal, int p_nExpenses, int p_nIncomeCard )
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

    cCurrency obPriceCardProfit = cCurrency( obTotal.currencyValue().toInt()-obPriceCashProfit.currencyValue().toInt() );

    addTableRow();
    addTableCell( tr("Card") );
    addTableCell( ":", "bold");
    addTableCell( obPriceCardProfit.currencyFullStringShort(), "right" );

    finishTable();
    finishSection();
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    int             nRet = 0;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "ledger.patientCardTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        nRet = poQueryResult->value(0).toInt();
    }

    return nRet;
}
//------------------------------------------------------------------------------------
QString cReportLedger::_countPatientCardTypeSell( unsigned int p_uiPatientCardTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";

    qsQuery = QString("SELECT COUNT(totalPrice) "
                      "FROM ledger, patientCardTypes WHERE "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "ledger.patientCardTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_PC_SELL).arg(p_uiPatientCardTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );

    if( poQueryResult->size() > 0 )
    {
        poQueryResult->first();
        if( poQueryResult->value(0).toInt() > 0 )
        {
            qsRet = QString::number( poQueryResult->value(0).toInt() );
        }
    }

    return qsRet;
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPanelUse( unsigned int p_uiPanelTypeId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger, panels WHERE "
                      "ledger.panelId=panels.panelId AND "
                      "ledgerTime>\"%1 00:00:00\" AND "
                      "ledgerTime<\"%2 24:00:00\" AND "
                      "ledgerTypeId=%3 AND "
                      "panelTypeId=%4 AND "
                      "ledger.active=1 " ).arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(LT_DEVICE_USAGE).arg(p_uiPanelTypeId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_countPanelUse(unsigned int p_uiPanelTypeId, tePanelUse p_tePanelUse)
//------------------------------------------------------------------------------------
{
    QString     qsQuery;
    QSqlQuery  *poQueryResult;
    QString     qsCond;

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

    qsQuery = QString("SELECT COUNT(ledgerDeviceId) FROM "
                      "ledgerdevice, panels WHERE "
                      "ledgerdevice.panelId = panels.panelId AND "
                      "%1 "
                      "panelTypeId=%2 AND "
                      "ledgerTime>\"%3 00:00:00\" AND "
                      "ledgerTime<\"%4 24:00:00\" ").arg( qsCond ).arg( p_uiPanelTypeId ).arg( filterDateStart().toString("yyyy-MM-dd") ).arg( filterDateStop().toString("yyyy-MM-dd") );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumPaymentMethod( unsigned int p_uiPaymentMethodId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM ledger WHERE "
                      "modified>\"%1 00:00:00\" AND "
                      "modified<\"%2 24:00:00\" AND "
                      "ledgerTypeId>0 AND "
                      "ledgerTypeId<9 AND "
                      "paymentMethodId=%3 AND "
                      "ledger.active=1 ").arg(filterDateStart().toString("yyyy-MM-dd")).arg(filterDateStop().toString("yyyy-MM-dd")).arg(p_uiPaymentMethodId);
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
int cReportLedger::_sumCassaIncome( unsigned int p_uiCassaId )
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
QString cReportLedger::_countsumPatientCardTypeSell( QString p_qsCassaId, unsigned int p_uiPatientCardTypeId, unsigned int *p_uiPricePCSell )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;
    QString         qsRet = "";
    QString         qsPCCondition = QString( "(ledgerTypeId=%1 OR ledgerTypeId=%2 OR ledgerTypeId=%3 OR ledgerTypeId=%4 OR ledgerTypeId=%5 OR ledgerTypeId=%6)" )
                                            .arg(LT_PC_SELL)
                                            .arg(LT_PC_REFILL)
                                            .arg(LT_PC_LOST_REPLACE)
                                            .arg(LT_PC_ASSIGN_PARTNER)
                                            .arg(LT_PC_ONLINE_SELL)
                                            .arg(LT_PC_ONLINE_REFILL);

    qsQuery = QString("SELECT COUNT(totalPrice), SUM(totalPrice) "
                      "FROM cassahistory, ledger, patientCardTypes WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.patientCardTypeId=patientCardTypes.patientCardTypeId AND "
                      "cassahistory.cassaId IN( %1 ) AND "
                      "%2 AND "
                      "ledger.patientCardTypeId=%3 AND "
                      "ledger.active=1 " ).arg(p_qsCassaId).arg(qsPCCondition).arg(p_uiPatientCardTypeId);
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
int cReportLedger::_sumPanelUse( QString p_qsCassaId, unsigned int p_uiPanelGroupId )
//------------------------------------------------------------------------------------
{
    QString         qsQuery;
    QSqlQuery      *poQueryResult;

    qsQuery = QString("SELECT SUM(totalPrice) "
                      "FROM cassahistory, ledger, panels WHERE "
                      "cassahistory.ledgerId=ledger.ledgerId AND "
                      "ledger.panelId=panels.panelId AND "
                      "cassahistory.cassaId IN ( %1 ) AND "
                      "ledgerTypeId=%2 AND "
                      "panelGroupId=%3 AND "
                      "ledger.active=1 " ).arg( p_qsCassaId ).arg( LT_DEVICE_USAGE ).arg( p_uiPanelGroupId );
    poQueryResult = g_poDB->executeQTQuery( qsQuery );
    poQueryResult->first();

    return poQueryResult->value(0).toInt();
}
//------------------------------------------------------------------------------------
