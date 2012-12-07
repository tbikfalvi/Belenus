#ifndef CREPORT_H
#define CREPORT_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class cReport : public QWidget
{
    Q_OBJECT

public:
    explicit cReport(QWidget *parent = 0, QString p_qsReportName = "" );

    QTextEdit       *m_teReport;
    QVBoxLayout     *mainLayout;

    QString          m_qsReportName;
    QString          m_qsReportDescription;
    int              m_nIndex;

    QString          name() const;
    QString          description() const;
    int              index();

    void             setIndex( int p_nIndex );
    void             setDescription( const QString &p_qsDescription );

signals:
    
public slots:
    
};

#endif // CREPORT_H
