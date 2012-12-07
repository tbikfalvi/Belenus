

#include "creport.h"

cReport::cReport(QWidget *parent, QString p_qsReportName) : QWidget(parent)
{
    m_teReport = new QTextEdit();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget( m_teReport );

    setLayout( mainLayout );

    m_qsReportName          = p_qsReportName;
    m_nIndex                = 0;
    m_qsReportDescription   = "";
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


void cReport::setDescription(const QString &p_qsDescription)
{
    m_qsReportDescription = p_qsDescription;
}

void cReport::setIndex(int p_nIndex)
{
    m_nIndex = p_nIndex;
}
