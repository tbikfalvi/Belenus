#ifndef DLGPREVIEW_H
#define DLGPREVIEW_H

#include <QDialog>
#include <QTextDocument>
#include <QString>

#include "ui_dlgpreview.h"

class cDlgPreview : public QDialog, protected Ui::dlgPreview
{
    Q_OBJECT

public:
    cDlgPreview( QWidget *parent = 0 );
    ~cDlgPreview();

protected:
    QString         m_qsReportName;
    QString         m_qsReportFileName;
    QTextDocument   m_tdReport;

    QString         intTimeToString( const int p_inTime );

    void keyPressEvent( QKeyEvent *p_poEvent );

protected slots:
    virtual void setReportTitle( const QString &p_qsTitle );
    virtual void setReportFileName( const QString &p_qsFileName );

    virtual void refreshReport();
    virtual void printReport();
    virtual void saveReportToFile();

};

#endif // DLGPREVIEW_H
