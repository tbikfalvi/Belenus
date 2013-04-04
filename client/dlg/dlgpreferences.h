#ifndef DLGPREFERENCES_H
#define DLGPREFERENCES_H

#include "../belenus.h"
//#include "../../build/belenus/ui_dlgpreferences.h"
#include "../../build/Belenus/ui_dlgpreferences.h"

class cDlgPreferences : public QDialog, private Ui::dlgPreferences
{
    Q_OBJECT

public:
    cDlgPreferences( QWidget *p_poParent = 0 );
    virtual ~cDlgPreferences();

public slots:
    virtual void accept();

private:
    int            m_inLangIdx;

    QPushButton   *m_poBtnSystem;

private slots:
    void on_btnSecondaryBackground_clicked();
    void on_pbSecondaryWindow_toggled(bool checked);
    void on_pbPanelSettings_clicked();
    void on_chkAutoCloseCassa_clicked();
    void on_sliConsoleLogLevel_valueChanged( int p_inValue );
    void on_sliDBLogLevel_valueChanged( int p_inValue );
    void on_sliGUILogLevel_valueChanged( int p_inValue );
    void on_sliFileLogLevel_valueChanged( int p_inValue );
    void on_btnMainBackground_clicked( bool p_boClicked );
    void on_spbBarcodeLen_valueChanged( int p_inValue );
    void on_ledPCLostPrice_textChanged(const QString &arg1);
    void on_ledPCPartnerPrice_textChanged(const QString &arg1);
};

#endif
