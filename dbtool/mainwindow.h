#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QDir>

//====================================================================================
// BERLETTIPUS structure
//====================================================================================
typedef struct _typ_berlettipus
{
   int   nID;
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
    void                        _EnCode( char *str, int size );
    void                        _DeCode( char *str, int size );
    void                        _loadPatientCardTypeImport();
    int                         _getPatientCardTypeId();

    Ui::MainWindow              *ui;

    QDir                         m_qdExpCurrentDir;
    int                          m_nProgramType;

    QVector<typ_berlet>          m_qvPatientCards;
    QVector<typ_berlettipus>     m_qvPatientCardTypes;

    QString                      m_qsPCFileName;
    QString                      m_qsPCTFileName;

    char                         m_strPatiencardTypeVersion[10];

private slots:
    void slotProgramSelected();
    void on_pbExportPCTDat_clicked();
    void on_pbImportPCTText_clicked();
    void on_pbSelectImportFile_clicked();
    void on_pbImportDB_clicked();
    void on_pbExpSelectDir_clicked();
    void on_rbProgramSensolite_clicked();
    void on_rbProgramKiwiSun_clicked();
};

#endif // MAINWINDOW_H
