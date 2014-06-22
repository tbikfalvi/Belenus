
#include "../framework/qtframework.h"
#include "creportguests.h"

cReportGuests::cReportGuests(QWidget *parent, QString p_qsReportName) : cReport(parent,p_qsReportName)
{
    m_qsReportName          = tr(" Guest list ");
    m_qsReportDescription   = tr( "This report shows the summarized list of data of the registered guests." );

    _setDataNameEnabled( true );
    _setDataNameLabelText( tr("Name contains :") );

    _setDataTypeEnabled( true );
    _setDataTypeLabelText( tr("Gender :") );

    QSqlQuery   *poQueryResult;
    QStringList  qslDataTypes;

    poQueryResult = g_poDB->executeQTQuery( "SELECT genderId, genderName FROM genders" );

    poQueryResult->first();
    qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    setFilterDataType( QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() ) );
    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }
    setFilterDataTypeList( qslDataTypes.join("#") );

    _setDataSubTypeEnabled( true );
    _setDataSubTypeLabelText( tr("Age :") );

    qslDataTypes.clear();

    poQueryResult = g_poDB->executeQTQuery( "SELECT ageTypeId, ageTypeName FROM agetypes" );

    poQueryResult->first();
    qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    setFilterDataSubType( QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() ) );
    while( poQueryResult->next() )
    {
        qslDataTypes << QString( "%1|%2" ).arg( poQueryResult->value(0).toString() ).arg( poQueryResult->value(1).toString() );
    }
    setFilterDataSubTypeList( qslDataTypes.join("#") );
}

void cReportGuests::refreshReport()
{
    m_dlgProgress.progressInit( tr("Create selected report ..."), 0, 100 );
    m_dlgProgress.setProgressValue( 0 );
    m_dlgProgress.progressShow();

    cReport::refreshReport();

    m_dlgProgress.increaseProgressValue();

    QStringList qslFilterGender = filterType().split("|");
    QStringList qslFilterAge    = filterSubType().split("|");

    QString qsQuery = QString( "SELECT membership, name, genderName, ageTypeName, email, mobile, address FROM patients, genders, agetypes WHERE "
                               "patients.gender=genders.genderId AND "
                               "patients.ageType=agetypes.ageTypeId AND "
                               "active=1 " );

    if( filterName().length() > 0 )
    {
        qsQuery.append( QString( " AND name LIKE \"\%%1\%\" ").arg( filterName() ) );
    }

    if( qslFilterGender.at(0).toInt() > 0 )
    {
        qsQuery.append( QString(" AND gender=%1 ").arg( qslFilterGender.at(0).toInt() ) );
    }

    if( qslFilterAge.at(0).toInt() > 0 )
    {
        qsQuery.append( QString(" AND ageType=%1 ").arg( qslFilterAge.at(0).toInt() ) );
    }

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
    addTableCell( tr("Membership Id"), "bold" );
    addTableCell( tr("Name"), "bold" );
    if( qslFilterGender.at(0).toInt() == 0 )
        addTableCell( tr("Gender"), "bold" );
    if( qslFilterAge.at(0).toInt() == 0 )
        addTableCell( tr("Age"), "bold" );
    addTableCell( tr("E-Mail"), "bold" );
    addTableCell( tr("Phone"), "bold" );
    addTableCell( tr("Address"), "bold" );

    while( poQueryResult->next() )
    {
        addTableRow();
        addTableCell( poQueryResult->value(0).toString() );
        addTableCell( poQueryResult->value(1).toString() );
        if( qslFilterGender.at(0).toInt() == 0 )
            addTableCell( poQueryResult->value(2).toString() );
        if( qslFilterAge.at(0).toInt() == 0 )
            addTableCell( poQueryResult->value(3).toString() );
        addTableCell( poQueryResult->value(4).toString() );
        addTableCell( poQueryResult->value(5).toString() );
        addTableCell( poQueryResult->value(6).toString() );

        m_dlgProgress.increaseProgressValue();
    }

    finishTable();
    finishSection();

    finishReport();

    m_dlgProgress.hide();
}
