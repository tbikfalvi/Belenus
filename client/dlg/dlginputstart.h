#ifndef DLGINPUTSTART_H
#define DLGINPUTSTART_H

#include "../belenus.h"
#include "../../belenus-build-desktop/ui_dlginputstart.h"

class cDlgInputStart : public QDialog, protected Ui::dlgInputStart
{
    Q_OBJECT

public:
    cDlgInputStart( QWidget *p_poParent = 0 );
    virtual ~cDlgInputStart();

    void setInitialText( const QString &p_stText );
    void init( const QString &p_qsText = "" );
    QString getEditText();

    bool m_bTime;
    bool m_bCard;
    bool m_bPat;
    bool m_bProd;

private:
    bool     m_bInitCalled;

private slots:
    void on_pbProduct_clicked();
    void on_ledInputStart_returnPressed();
    void on_pbCancel_clicked();
    void on_pbPatient_clicked();
    void on_pbCardcode_clicked();
    void on_pbTime_clicked();
    void on_ledInputStart_textChanged(QString );
};

#endif
