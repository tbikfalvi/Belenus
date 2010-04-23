#ifndef DLG_MAINWINDOW_H
#define DLG_MAINWINDOW_H

#include <QtNetwork>

#include "ui_bsa_mainwindow.h"

class cDlgMainWindow : public QDialog, protected Ui::dlgMainWindow
{
    Q_OBJECT

public:
    cDlgMainWindow( QWidget *p_poParent = 0 );
    virtual ~cDlgMainWindow();

public slots:

private:
    int          m_nCurrentId;

    QTcpSocket  *m_ptcpSocket;
    QByteArray   m_obPwdHash;
    quint16      blockSize;
    QString      m_qsMessageIN;
    QString      m_qsMessageOUT;
    QString      m_qsMessageChecksumIN;
    QString      m_qsMessageChecksumOUT;
    int          m_nProcessStep;
    QString      m_qsAction;
    QString      m_qsCommand;
    QString      m_qsParameter;

    bool         m_bRegisterSerialOnServer;

    bool checkMessageConsistency();
    QString getMessageBody();
    QString getMessageAction();
    void ProcessMessage();

    void FillListSerial();
    void FillMessageCombo();
    bool EnableButtonConnect();
    void EnableButtonsServer();

    void msgSend( QString qsMsg );
    void sendMessage();

    void RegisterSerialOnServer();

private slots:
    void on_pbSendCustom_clicked();
    void on_ledAdminPassword_textChanged(QString );
    void on_pbDeleteSerialFromServer_clicked();
    void on_pbRegisterSerialOnServer_clicked();
    void msgReceive();
    void displayError(QAbstractSocket::SocketError socketError);
    void on_pbConnect_clicked();
    void on_ledPort_textChanged(QString );
    void on_ledAddress_textChanged(QString );
    void on_pbEmptySerial_clicked();
    void on_ledSerialNumber_textChanged(QString );
    void on_listSerialNumbers_doubleClicked(QModelIndex index);
    void on_listSerialNumbers_itemSelectionChanged();
    void on_pbDeleteSerial_clicked();
    void on_pbEditSerial_clicked();
    void on_ledAdminPassword_lostFocus();
    void on_pbRegisterSerial_clicked();
    void on_pbExitApplication_clicked();
};

#endif // DLG_MAINWINDOW_H
