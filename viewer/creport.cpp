//====================================================================================
//
// Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2012
//
//====================================================================================
//
// Filename    : creport.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Report-ok szulo osztalya
//====================================================================================

#include "creport.h"

//====================================================================================
cReport::cReport(QWidget *parent, QString p_qsReportName) : QWidget(parent)
//====================================================================================
{
    //-----------------------------------------------------
    // Valtozok inicializalasa
    //-----------------------------------------------------
    m_qsReportName          = p_qsReportName;
    m_nIndex                = 0;
    m_qsReportDescription   = "";
    m_nSectionLevel         = 0;
    m_bIsTableStarted       = false;
    m_bIsTableRowStarted    = false;

    //-----------------------------------------------------
    // Report gui elemek inicializalasa
    //-----------------------------------------------------
    m_teReport = new QTextEdit();
    mainLayout = new QVBoxLayout;

    mainLayout->addWidget( m_teReport );
    setLayout( mainLayout );

    //-----------------------------------------------------
    // Report elemek beallitasa
    //-----------------------------------------------------
    m_tcReport = new QTextCursor( &m_tdReport );
    m_teReport->setDocument( &m_tdReport );

    //-----------------------------------------------------
    // Report filter gui elemek beallitasa
    //-----------------------------------------------------
    _setDateStartEnabled();
    _setDateStopEnabled();
    _setDataNameEnabled();
    _setDataTypeEnabled();

    //-----------------------------------------------------
    // Report formazo elemek beallitasa
    //-----------------------------------------------------
    obTitleFormat = new QTextCharFormat();
    obTitleFormat->setFontPointSize( 14.0 );
    obTitleFormat->setFontWeight( QFont::Bold );

    obNormalFormat = new QTextCharFormat();
    obNormalFormat->setFontPointSize( 10.0 );
    obNormalFormat->setFontWeight( QFont::Normal );
    obNormalFormat->setFontItalic( false );

    obBoldFormat = new QTextCharFormat();
    obBoldFormat->setFontPointSize( 10.0 );
    obBoldFormat->setFontWeight( QFont::Bold );

    obItalicFormat = new QTextCharFormat();
    obItalicFormat->setFontPointSize( 10.0 );
    obItalicFormat->setFontItalic( true );

    obTableFormat = new QTextTableFormat();
    obTableFormat->setHeaderRowCount( 1 );
    obTableFormat->setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormat->setAlignment( Qt::AlignHCenter );

    obLeftCellFormat = new QTextBlockFormat();
    obLeftCellFormat->setLeftMargin( 10 );
    obLeftCellFormat->setRightMargin( 10 );
    obLeftCellFormat->setAlignment( Qt::AlignLeft );

    obCenterCellFormat = new QTextBlockFormat();
    obCenterCellFormat->setLeftMargin( 10 );
    obCenterCellFormat->setRightMargin( 10 );
    obCenterCellFormat->setAlignment( Qt::AlignHCenter );

    obRightCellFormat = new QTextBlockFormat();
    obRightCellFormat->setLeftMargin( 10 );
    obRightCellFormat->setRightMargin( 10 );
    obRightCellFormat->setAlignment( Qt::AlignRight );
}
//------------------------------------------------------------------------------------
cReport::~cReport()
//------------------------------------------------------------------------------------
{
    delete m_tcReport;
    delete mainLayout;
    delete m_teReport;
}
//------------------------------------------------------------------------------------
void cReport::refreshReport()
//------------------------------------------------------------------------------------
{
    m_tdReport.clear();
    m_tdReport.setMetaInformation( QTextDocument::DocumentTitle, m_qsReportName );
    m_qsReportHtml = "";
}
//=================================================================================================
//=================================================================================================
// Adatokat szolgaltato fuggvenyek
//------------------------------------------------------------------------------------
QString cReport::name() const               { return m_qsReportName; }
QString cReport::description() const        { return m_qsReportDescription; }
int     cReport::index()                    { return m_nIndex; }
bool    cReport::isDateStartEnabled()       { return m_bDateStartEnabled; }
bool    cReport::isDateStopEnabled()        { return m_bDateStopEnabled; }
bool    cReport::isDataNameEnabled()        { return m_bDataNameEnabled; }
bool    cReport::isDataTypeEnabled()        { return m_bDataTypeEnabled; }
QString cReport::labelDateStartText() const { return m_qsLabelDateStart; }
QString cReport::labelDateStopText() const  { return m_qsLabelDateStop; }
QString cReport::labelDataNameText() const  { return m_qsLabelDataName; }
QString cReport::labelDataTypeText() const  { return m_qsLabelDataType; }
QDate   cReport::filterDateStart() const    { return m_qdStartDate; }
QDate   cReport::filterDateStop() const     { return m_qdStopDate; }
QString cReport::filterName() const         { return m_qsName; }
QString cReport::filterType() const         { return m_qsType; }
//=================================================================================================
//=================================================================================================
// Adatokat beallito fuggvenyek
//------------------------------------------------------------------------------------
void cReport::setIndex(int p_nIndex)
//------------------------------------------------------------------------------------
{
    m_nIndex = p_nIndex;
}
//------------------------------------------------------------------------------------
void cReport::setDescription(const QString &p_qsDescription)
//------------------------------------------------------------------------------------
{
    m_qsReportDescription = p_qsDescription;
}
//------------------------------------------------------------------------------------
void cReport::setFilterDateStart( const QDate &p_qdDate )
//------------------------------------------------------------------------------------
{
    m_qdStartDate = p_qdDate;
}
//------------------------------------------------------------------------------------
void cReport::setFilterDateStop( const QDate &p_qdDate )
//------------------------------------------------------------------------------------
{
    m_qdStopDate = p_qdDate;
}
//------------------------------------------------------------------------------------
void cReport::setFilterDataName( const QString &p_qsName )
//------------------------------------------------------------------------------------
{
    m_qsName = p_qsName;
}
//------------------------------------------------------------------------------------
void cReport::setFilterDataType( const QString &p_qsType )
//------------------------------------------------------------------------------------
{
    m_qsType = p_qsType;
}
//=================================================================================================
//=================================================================================================
// Report formazo fuggvenyek
//------------------------------------------------------------------------------------
void cReport::startReport()
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "<html><body>" );
}
//------------------------------------------------------------------------------------
void cReport::finishReport()
//------------------------------------------------------------------------------------
{
    while( m_nSectionLevel > 0 )
    {
        finishSection();
    }
    m_qsReportHtml.append( "</body></html>" );

    m_tcReport->insertHtml( m_qsReportHtml );
}
//------------------------------------------------------------------------------------
void cReport::startSection()
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "<div>" );
    m_nSectionLevel++;
}
//------------------------------------------------------------------------------------
void cReport::finishSection()
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "</div>");
    m_nSectionLevel--;
}
//------------------------------------------------------------------------------------
void cReport::addHorizontalLine()
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "<hr>" );
}
//------------------------------------------------------------------------------------
void cReport::addSeparator()
//------------------------------------------------------------------------------------
{
    startSection();
    m_qsReportHtml.append( "<br>" );
    finishSection();
}
//------------------------------------------------------------------------------------
void cReport::addTitle( QString p_qsTitle )
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( QString("<div style=\"font-size:24px;font-weight:bold;\">%1</div>").arg( p_qsTitle ) );
}
//------------------------------------------------------------------------------------
void cReport::addSubTitle( QString p_qsSubTitle )
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( QString("<div style=\"font-size:14px;font-weight:bold;\">%1</div>").arg( p_qsSubTitle ) );
}
//------------------------------------------------------------------------------------
void cReport::addTable()
//------------------------------------------------------------------------------------
{
    if( m_bIsTableStarted )
    {
        m_qsReportHtml.append( "</table>" );
    }
    m_qsReportHtml.append( "<table>" );
    m_bIsTableStarted = true;
}
//------------------------------------------------------------------------------------
void cReport::finishTable()
//------------------------------------------------------------------------------------
{
    if( m_bIsTableRowStarted )
    {
        finishTableRow();
    }
    m_qsReportHtml.append( "</table>" );
    m_bIsTableStarted = false;
}
//------------------------------------------------------------------------------------
void cReport::addTableRow()
//------------------------------------------------------------------------------------
{
    if( m_bIsTableRowStarted )
    {
        m_qsReportHtml.append( "</tr>" );
    }
    m_qsReportHtml.append( "<tr>" );
    m_bIsTableRowStarted = true;
}
//------------------------------------------------------------------------------------
void cReport::finishTableRow()
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "</tr>" );
    m_bIsTableRowStarted = false;
}
//------------------------------------------------------------------------------------
void cReport::addTableCell(QString p_qsCellText, QString p_qsFormat)
//------------------------------------------------------------------------------------
{
    m_qsReportHtml.append( "<td" );

    if( p_qsFormat.contains("center") )         m_qsReportHtml.append( " align=\"center\" " );
    else if( p_qsFormat.contains("right") )     m_qsReportHtml.append( " align=\"right\" " );
    else                                        m_qsReportHtml.append( " align=\"left\" " );

    if( p_qsFormat.contains("middle") )         m_qsReportHtml.append( " valign=\"middle\" " );
    else if( p_qsFormat.contains("bottom") )    m_qsReportHtml.append( " valign=\"bottom\" " );
    else                                        m_qsReportHtml.append( " valign=\"top\" " );

    m_qsReportHtml.append( ">" );
    m_qsReportHtml.append( "<div style=\"margin:2px 10px 2px 10px;\">" );

    if( p_qsFormat.contains("bold") )   m_qsReportHtml.append( "<b>" );
    if( p_qsFormat.contains("italic") )   m_qsReportHtml.append( "<i>" );

    m_qsReportHtml.append( p_qsCellText );

    if( p_qsFormat.contains("italic") )   m_qsReportHtml.append( "</i>" );
    if( p_qsFormat.contains("bold") )   m_qsReportHtml.append( "</b>" );

    m_qsReportHtml.append( "</div>" );
    m_qsReportHtml.append( "</td>" );
}
//=================================================================================================
//=================================================================================================
// Protected, belso fuggvenyek
//------------------------------------------------------------------------------------
void cReport::_setDateStartEnabled(bool bEnabled)
//------------------------------------------------------------------------------------
{
    m_bDateStartEnabled = bEnabled;
}
//------------------------------------------------------------------------------------
void cReport::_setDateStopEnabled(bool bEnabled)
//------------------------------------------------------------------------------------
{
    m_bDateStopEnabled = bEnabled;
}
//------------------------------------------------------------------------------------
void cReport::_setDataNameEnabled(bool bEnabled)
//------------------------------------------------------------------------------------
{
    m_bDataNameEnabled = bEnabled;
}
//------------------------------------------------------------------------------------
void cReport::_setDataTypeEnabled(bool bEnabled)
//------------------------------------------------------------------------------------
{
    m_bDataTypeEnabled = bEnabled;
}
//------------------------------------------------------------------------------------
void cReport::_setDateStartLabelText( const QString &p_qsText )
//------------------------------------------------------------------------------------
{
    m_qsLabelDateStart = p_qsText;
}
//------------------------------------------------------------------------------------
void cReport::_setDateStopLabelText( const QString &p_qsText )
//------------------------------------------------------------------------------------
{
    m_qsLabelDateStop = p_qsText;
}
//------------------------------------------------------------------------------------
void cReport::_setDataNameLabelText( const QString &p_qsText )
//------------------------------------------------------------------------------------
{
    m_qsLabelDataName = p_qsText;
}
//------------------------------------------------------------------------------------
void cReport::_setDataTypeLabelText( const QString &p_qsText )
//------------------------------------------------------------------------------------
{
    m_qsLabelDataType = p_qsText;
}
//------------------------------------------------------------------------------------
