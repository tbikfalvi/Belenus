#ifndef CREPORT_H
#define CREPORT_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class cReport : public QWidget
{
    Q_OBJECT

public:
    explicit cReport(QWidget *parent = 0);

    QTextEdit   *m_teReport;
    QVBoxLayout *mainLayout;

signals:
    
public slots:
    
};

#endif // CREPORT_H
