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
#include <QPrinter>

#include "dlgprogress.h"

class cReport : public QWidget
{
    Q_OBJECT

public:

    enum teLedgerType
    {
        LT_NOT_DEFINED = 0,
        LT_DEVICE_USAGE,
        LT_PC_SELL,
        LT_PC_REFILL,
        LT_PROD_SELL,
        LT_PC_LOST_REPLACE,
        LT_PC_ASSIGN_PARTNER,
        LT_OTHER7,
        LT_OTHER8,
        LT_PROD_STORAGE_CHANGE,
        LT_CASSA_EXPENSE
    };

    enum tePanelUse
    {
        PU_USE_WITH_CARD = 1,
        PU_USE_WITH_CASH,
        PU_USE_COMBINED
    };

    enum tePanelCountType
    {
        PU_COUNT_GUEST = 1,
        PU_COUNT_PCUNITS
    };

    explicit cReport(QWidget *parent = 0, QString p_qsReportName = "" );
    ~cReport();

    virtual void         refreshReport();

    QString              name() const;
    QString              description() const;
    int                  index();

    void                 setIndex( int p_nIndex );
    void                 setDescription( const QString &p_qsDescription );

    void                 printReport( QPrinter *p_obPrinter );

    bool                 isDateStartEnabled();
    bool                 isDateStopEnabled();
    bool                 isDataNameEnabled();
    bool                 isDataTypeEnabled();
    bool                 isDataIsVisibleEnabled();

    QString              labelDateStartText() const;
    QString              labelDateStopText() const;
    QString              labelDataNameText() const;
    QString              labelDataTypeText() const;
    QString              labelIsVisibleText() const;

    QDate                filterDateStart() const;
    QDate                filterDateStop() const;
    QString              filterName() const;
    QString              filterType() const;
    bool                 filterIsVisible() const;

    QString              filterTypeList() const;

    void                 setFilterDateStart( const QDate &p_qdDate );
    void                 setFilterDateStop( const QDate &p_qdDate );
    void                 setFilterDataName( const QString &p_qsName );
    void                 setFilterDataType( const QString &p_qsType );
    void                 setFilterIsVisible( const bool p_bIsVisible );

    void                 setFilterDataTypeList( const QString &p_qsTypeList );

    void                 startReport();
    void                 finishReport();
    void                 startSection();
    void                 finishSection();
    void                 addHorizontalLine();
    void                 addSeparator();
    void                 addTitle( QString p_qsTitle );
    void                 addSubTitle( QString p_qsSubTitle );
    void                 addDescription( QString p_qsDescription );
    void                 addTable();
    void                 finishTable();
    void                 addTableRow();
    void                 finishTableRow();
    void                 addTableCell( QString p_qsCellText="", QString p_qsFormat="" );

protected:

    dlgProgress          m_dlgProgress;
    QTextEdit           *m_teReport;
    QTextDocument        m_tdReport;
    QTextCursor         *m_tcReport;
    QVBoxLayout         *mainLayout;
    QString              m_qsReportHtml;

    QString              m_qsReportName;
    QString              m_qsReportDescription;
    int                  m_nIndex;

    bool                 m_bDateStartEnabled;
    bool                 m_bDateStopEnabled;
    bool                 m_bDataNameEnabled;
    bool                 m_bDataTypeEnabled;
    bool                 m_bIsVisibleEnabled;

    QString              m_qsLabelDateStart;
    QString              m_qsLabelDateStop;
    QString              m_qsLabelDataName;
    QString              m_qsLabelDataType;
    QString              m_qsLabelIsVisible;

    QDate                m_qdStartDate;
    QDate                m_qdStopDate;
    QString              m_qsName;
    QString              m_qsType;
    bool                 m_bIsVisible;

    QString              m_qsTypeList;

    void                _setDateStartEnabled( bool bEnabled = false );
    void                _setDateStopEnabled( bool bEnabled = false );
    void                _setDataNameEnabled( bool bEnabled = false );
    void                _setDataTypeEnabled( bool bEnabled = false );
    void                _setDataIsVisibleEnabled( bool bEnabled = false );

    void                _setDateStartLabelText( const QString &p_qsText );
    void                _setDateStopLabelText( const QString &p_qsText );
    void                _setDataNameLabelText( const QString &p_qsText );
    void                _setDataTypeLabelText( const QString &p_qsText );
    void                _setDataIsVisibleText( const QString &p_qsText );

private:

    int                  m_nSectionLevel;
    bool                 m_bIsTableStarted;
    bool                 m_bIsTableRowStarted;

signals:
    
public slots:
    
};

#endif // CREPORT_H
