#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDir>
#include <QtSql/QSqlDatabase>

#include "dlgprogress.h"

//====================================================================================
// BERLETTIPUS structure
//====================================================================================
typedef struct _typ_berlettipus
{
   int   nID;
   int   nNewID;
   int   nAr;
   int   nEgyseg;
   char  strNev[50];
   int   nErvTolEv;
   int   nErvTolHo;
   int   nErvTolNap;
   int   nErvIgEv;
   int   nErvIgHo;
   int   nErvIgNap;
   int   nErvNapok;
   char  bSzolariumHaszn;
   int   nEgysegIdo;
} typ_berlettipus;

//====================================================================================
// BERLET structure
//====================================================================================
typedef struct _typ_berlet
{
   char  strVonalkod[20];
   char  strMegjegyzes[50];
   int   nBerletTipus;
   int   nEgyseg;
   int   nErvEv;
   int   nErvHo;
   int   nErvNap;
   int   nPin;
} typ_berlet;
//====================================================================================
//    BERLETHASZNALAT structure
//====================================================================================
typedef struct _typ_berlethasznalat
{
   char  strVonalkod[20];
   int   nEv;
   int   nHo;
   int   nNap;
   int   nOra;
   int   nPerc;
   int   nEgyseg;
} typ_berlethasznalat;
//====================================================================================
//    TERMEK structure
//====================================================================================
typedef struct _typ_termek
{
   int   nID;
   int   nNewID;
   char  strVonalkod[20];
   char  strNev[100];
   int   nAr;
   int   nDarab;
   int   nArBeszerzes;
} typ_termek;
//====================================================================================
//    TERMEKTIPUS structure
//====================================================================================
typedef struct _typ_termektipus
{
   int   nID;
   int   nNewID;
   char  strNev[100];
} typ_termektipus;
//====================================================================================
//    TERMEKTIPUSASSIGN structure
//====================================================================================
typedef struct _typ_termektipusassign
{
   int  nTermekID;
   int  nTTipusID;
} typ_termektipusassign;
//====================================================================================
//  USER structure
//====================================================================================
typedef struct _typ_user
{
   int      nID;
   char     strAzonosito[20];
   char     strLoginNev[20];
   char     strNevCsalad[100];
   char     strJelszo[20];
   char     strMegjegyzes[1000];
   int      nUserLevel;
} typ_user;
//====================================================================================

namespace Ui {
    class MainWindow;
}

class DBTool
{
public:
    enum prgType
    {
        KiwiSun = 1,
        Sensolite
    };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent *p_poEvent );

private:
    void                        _loadPatientCardTypes();
    void                        _loadPatientCards();
    void                        _loadProductTypes();
    void                        _loadProducts();
    void                        _loadProductAssign();
    void                        _loadUsers();
    void                        _EnCode( char *str, int size );
    void                        _DeCode( char *str, int size );
    void                        _loadPatientCardTypeImport();
    int                         _getPatientCardTypeId();
    int                         _getTimeLeft(int p_nBerletTipus, int p_nEgyseg );
    void                        _exportToBelenusPatientCardTypes();
    void                        _exportToBelenusPatientCards();
    void                        _exportToBelenusProductTypes();
    void                        _exportToBelenusProducts();
    void                        _exportToBelenusUsers();

    int                         _getPatientCardTypeNewId( int p_nID );
    int                         _getProductTypeNewId( int p_nID );
    int                         _getProductNewId( int p_nID );

    void                        _logAction( QString p_qsLogMessage );

    Ui::MainWindow              *ui;

    QDir                         m_qdExpCurrentDir;
    int                          m_nProgramType;

    int                          m_nCountItems;

    QVector<typ_berlet>          m_qvPatientCards;
    QVector<typ_berlettipus>     m_qvPatientCardTypes;
    QVector<typ_termektipus>     m_qvProductTypes;
    QVector<typ_termek>          m_qvProducts;
    QVector<typ_termektipusassign>  m_qvProductAssigns;
    QVector<typ_user>            m_qvUsers;

    QString                      m_qsPCFileName;
    QString                      m_qsPCTFileName;
    QString                      m_qsPTFileName;
    QString                      m_qsPFileName;
    QString                      m_qsPAFileName;
    QString                      m_qsUFileName;

    char                         m_strPatiencardTypeVersion[10];
    int                          m_nLicenceId;

    QSqlDatabase                *m_poDB;
    cDlgProgress                *m_dlgProgress;

    QString                      m_qsLogFileName;

private slots:
    void on_pbExportProcess_clicked();
    void on_pbPExportConnect_clicked();
    void slotProgramSelected();
//    void on_pbExportPCTDat_clicked();
//    void on_pbImportPCTText_clicked();
//    void on_pbSelectImportFile_clicked();
    void on_pbImportDB_clicked();
    void on_pbExpSelectDir_clicked();
//    void on_rbProgramSensolite_clicked();
    void on_rbProgramKiwiSun_clicked();
};

#endif // MAINWINDOW_H
