#ifndef CREPORT_H
#define CREPORT_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>

class cReport : public QWidget
{
    Q_OBJECT

public:

    explicit cReport(QWidget *parent = 0, QString p_qsReportName = "" );

    QString              name() const;
    QString              description() const;
    int                  index();

    void                 setIndex( int p_nIndex );
    void                 setDescription( const QString &p_qsDescription );

    bool                 isDateStartEnabled();
    bool                 isDateStopEnabled();
    bool                 isDataNameEnabled();
    bool                 isDataTypeEnabled();

protected:

    QTextEdit           *m_teReport;
    QVBoxLayout         *mainLayout;

    QTextCharFormat     *obTitleFormat;
    QTextCharFormat     *obNormalFormat;
    QTextCharFormat     *obBoldFormat;
    QTextTableFormat    *obTableFormat;
    QTextBlockFormat    *obLeftCellFormat;
    QTextBlockFormat    *obRightCellFormat;

    QString              m_qsReportName;
    QString              m_qsReportDescription;
    int                  m_nIndex;

    bool                 m_bDateStartEnabled;
    bool                 m_bDateStopEnabled;
    bool                 m_bDataNameEnabled;
    bool                 m_bDataTypeEnabled;

    QString              m_qsLabelDateStart;
    QString              m_qsLabelDateStop;
    QString              m_qsLabelDataName;
    QString              m_qsLabelDataType;

    void                 setDateStartEnabled( bool bEnabled = false );
    void                 setDateStopEnabled( bool bEnabled = false );
    void                 setDataNameEnabled( bool bEnabled = false );
    void                 setDataTypeEnabled( bool bEnabled = false );

signals:
    
public slots:
    
};

#endif // CREPORT_H
