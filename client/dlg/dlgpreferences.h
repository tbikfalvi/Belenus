#ifndef DLGPREFERENCES_H
#define DLGPREFERENCES_H

#include "../belenus.h"
#include "ui_dlgpreferences.h"
#include "dlgprogress.h"

class cDlgPreferences : public QDialog, private Ui::dlgPreferences
{
    Q_OBJECT

public:
    cDlgPreferences( QWidget *p_poParent = 0 );
    virtual ~cDlgPreferences();

public slots:
    virtual void accept();

private:
    int              m_inLangIdx;
    QPushButton     *m_poBtnSystem;
    cDlgProgress    *m_dlgProgress;
    int              m_nTimer;

    void            _updateDatabaseLanguage();
    void            _increasePatientCardBarcodes();
    void            _decreasePatientCardBarcodes( bool p_bCutBegin );

protected:
    void timerEvent( QTimerEvent *p_poEvent );

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
    void on_chkAutoOpenNewCassa_clicked();
    void on_chkEnableDatabaseBackup_clicked();
    void on_pbChangeBinaryLocation_clicked();
    void on_pbBackupLocation_clicked();
    void on_cmbDateFormat_currentIndexChanged(const QString &arg1);
    void on_pbTestHttpConnection_clicked();
    void on_chkEnableHttp_clicked(bool checked);
    void on_btnSecondaryFrame_clicked();
    void on_pbModifyDevAdminPsw_clicked();
    void on_pbCancelModifyPsw_clicked();
    void on_pbCaptionBackgroundActive_clicked();
    void on_pbTextColorActive_clicked();
    void on_pbModifyExpDate_clicked();
};

#endif
