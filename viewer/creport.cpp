

#include "creport.h"

cReport::cReport(QWidget *parent, QString p_qsReportName) : QWidget(parent)
{
    m_teReport = new QTextEdit();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget( m_teReport );

    setLayout( mainLayout );

    obTitleFormat->setFontPointSize( 14.0 );
    obTitleFormat->setFontWeight( QFont::Bold );

    obNormalFormat->setFontPointSize( 10.0 );
    obNormalFormat->setFontWeight( QFont::Normal );

    obBoldFormat->setFontPointSize( 10.0 );
    obBoldFormat->setFontWeight( QFont::Bold );

    obTableFormat->setHeaderRowCount( 1 );
    obTableFormat->setBorderStyle( QTextFrameFormat::BorderStyle_None );
    obTableFormat->setAlignment( Qt::AlignHCenter );

    obLeftCellFormat->setLeftMargin( 10 );
    obLeftCellFormat->setRightMargin( 10 );
    obLeftCellFormat->setAlignment( Qt::AlignLeft );

    obRightCellFormat->setLeftMargin( 10 );
    obRightCellFormat->setRightMargin( 10 );
    obRightCellFormat->setAlignment( Qt::AlignRight );

    m_qsReportName          = p_qsReportName;
    m_nIndex                = 0;
    m_qsReportDescription   = "";

    setDateStartEnabled();
    setDateStopEnabled();
    setDataNameEnabled();
    setDataTypeEnabled();
}

QString cReport::name() const
{
    return m_qsReportName;
}

QString cReport::description() const
{
    return m_qsReportDescription;
}

int cReport::index()
{
    return m_nIndex;
}

bool cReport::isDateStartEnabled()
{
    return m_bDateStartEnabled;
}

bool cReport::isDateStopEnabled()
{
    return m_bDateStopEnabled;
}

bool cReport::isDataNameEnabled()
{
    return m_bDataNameEnabled;
}

bool cReport::isDataTypeEnabled()
{
    return m_bDataTypeEnabled;
}

void cReport::setDescription(const QString &p_qsDescription)
{
    m_qsReportDescription = p_qsDescription;
}

void cReport::setIndex(int p_nIndex)
{
    m_nIndex = p_nIndex;
}

void cReport::setDateStartEnabled(bool bEnabled)
{
    m_bDateStartEnabled = bEnabled;
}

void cReport::setDateStopEnabled(bool bEnabled)
{
    m_bDateStopEnabled = bEnabled;
}

void cReport::setDataNameEnabled(bool bEnabled)
{
    m_bDataNameEnabled = bEnabled;
}

void cReport::setDataTypeEnabled(bool bEnabled)
{
    m_bDataTypeEnabled = bEnabled;
}

