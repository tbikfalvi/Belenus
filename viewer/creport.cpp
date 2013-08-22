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
