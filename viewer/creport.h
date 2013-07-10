#ifndef CREPORT_H
#define CREPORT_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextBlockFormat>
#include <QDate>
#include <QSqlQuery>

#include "dlgprogress.h"

class cReport : public QWidget
{
    Q_OBJECT

public:

    explicit cReport(QWidget *parent = 0, QString p_qsReportName = "" );
    ~cReport();

    virtual void         refreshReport();

    QString              name() const;
    QString              description() const;
    int                  index();

    void                 setIndex( int p_nIndex );
    void                 setDescription( const QString &p_qsDescription );

    bool                 isDateStartEnabled();
    bool                 isDateStopEnabled();
    bool                 isDataNameEnabled();
    bool                 isDataTypeEnabled();

    QString              labelDateStartText() const;
    QString              labelDateStopText() const;
    QString              labelDataNameText() const;
    QString              labelDataTypeText() const;

    QDate                filterDateStart() const;
    QDate                filterDateStop() const;
    QString              filterName() const;
    QString              filterType() const;

    void                 setFilterDateStart( const QDate &p_qdDate );
    void                 setFilterDateStop( const QDate &p_qdDate );
    void                 setFilterDataName( const QString &p_qsName );
    void                 setFilterDataType( const QString &p_qsType );

protected:

    dlgProgress          m_dlgProgress;
    QTextEdit           *m_teReport;
    QTextDocument        m_tdReport;
    QTextCursor         *m_tcReport;
    QVBoxLayout         *mainLayout;

    QTextCharFormat     *obTitleFormat;
    QTextCharFormat     *obNormalFormat;
    QTextCharFormat     *obBoldFormat;
    QTextCharFormat     *obItalicFormat;
    QTextTableFormat    *obTableFormat;
    QTextBlockFormat    *obLeftCellFormat;
    QTextBlockFormat    *obCenterCellFormat;
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

    QDate                m_qdStartDate;
    QDate                m_qdStopDate;
    QString              m_qsName;
    QString              m_qsType;

    void                _setDateStartEnabled( bool bEnabled = false );
    void                _setDateStopEnabled( bool bEnabled = false );
    void                _setDataNameEnabled( bool bEnabled = false );
    void                _setDataTypeEnabled( bool bEnabled = false );

    void                _setDateStartLabelText( const QString &p_qsText );
    void                _setDateStopLabelText( const QString &p_qsText );
    void                _setDataNameLabelText( const QString &p_qsText );
    void                _setDataTypeLabelText( const QString &p_qsText );

signals:
    
public slots:
    
};

#endif // CREPORT_H
