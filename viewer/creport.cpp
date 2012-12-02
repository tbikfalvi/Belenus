

#include "creport.h"

cReport::cReport(QWidget *parent) : QWidget(parent)
{
    m_teReport = new QTextEdit();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget( m_teReport );

    setLayout( mainLayout );
}
