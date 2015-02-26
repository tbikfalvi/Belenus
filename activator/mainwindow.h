#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QModelIndex>
#include <QTreeWidgetItem>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static const int LICENCE_MAX_NUMBER = 60;

    enum licenceType {
        LTYPE_DEMO,
        LTYPE_REGISTERED,
        LTYPE_ACTIVATED,
        LTYPE_EXPIRED,
        LTYPE_INVALID
    };

    enum licenceError {
        ERR_NO_ERROR = 0,
        ERR_KEY_FORMAT_MISMATCH,    // Licence key format is not BLNSxx_ZZZZZZ where 'x' is a digit and 'ZZZZZZ' is a stored string
        ERR_KEY_NUMBER_INCORRECT,   // Licence key format is ok, but number after 'BLNS' is not ok
        ERR_KEY_NOT_EXISTS,         // Licence key format is ok, but licence key is not official
        ERR_ACT_KEY_INCORRECT
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent *p_poEvent );

private:

    Ui::MainWindow              *ui;

    QStringList     m_qslLicenceKeys;
    QStringList     m_qslLicenceCodes;
    QStringList     m_qslCode;
    QStringList     m_qslCodeString;

    licenceType     m_LicenceType;
    QString         m_qsLicenceString;
    QString         m_qsCode;
    QDate           m_qdLastValidated;
    int             m_nLicenceId;
    int             m_nLicenceOrderNumber;

    bool            m_bLicenceModify;

    void            _EnCode( char *str, int size );
    void            _DeCode( char *str, int size );
    bool            _validateLicenceStr( QString p_qsLicenceString );
    void            _fillLicenceTree();
    void            _saveLicenceTree();

private slots:

    void on_pbNewLicenceKey_clicked();
    void on_pbEditLicenceKey_clicked();
    void on_pbDeleteLicenceKey_clicked();
    void on_pbSaveLicence_clicked();
    void on_treeLicences_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pbCancelLicence_clicked();
    void on_ledLicenceKeyName_textChanged(const QString &arg1);
    void on_pbValidateLicence_clicked();
    void on_ledRegistrationCode_textEdited(const QString &arg1);
};

#endif // MAINWINDOW_H
