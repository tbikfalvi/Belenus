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
    QString       m_qsReportName;
    QTextDocument m_tdReport;

//    void keyPressEvent ( QKeyEvent *p_poEvent );

protected slots:
    virtual void setReportTitle( const QString &p_qsTitle );

    virtual void refreshReport();
    virtual void printReport();

};

#endif // DLGPREVIEW_H
