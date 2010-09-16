#ifndef CDLGMAIN_H
#define CDLGMAIN_H

#include <QDialog>

#ifdef __WIN32__
    #include <windows.h>
#endif

#include "ui_cdlgmain.h"

//namespace Ui {
//    class cDlgMain;
//}

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
   bool  bSzolariumHaszn;
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
// USER structure
//====================================================================================
typedef struct _typ_user
{
   unsigned int     dID;
   char             strAzonosito[20];
   char             strLoginNev[20];
   char             strNevCsalad[100];
   char             strJelszo[20];
   char             strMegjegyzes[1000];
   int              nUserLevel;
} typ_user;
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
class cDlgMain : public QDialog, protected Ui_cDlgMain
{
    Q_OBJECT

public:
    explicit cDlgMain(QWidget *parent = 0);
    ~cDlgMain();

private:
    QString     m_qsDATPath;
    QString     m_qsSQLPath;
    QString     m_qsAppPath;
    QString     m_qsFullName;

    QString     m_qsPatientCardTypes;
    QString     m_qsPatientCards;
    QString     m_qsUsers;
    QString     m_qsPatientCardUse;

    void            EnCode( char *str, int size );
    void            DeCode( char *str, int size );
    bool            checkFile( QString p_qsFileName);
    bool            createPCTFile();
    bool            createPCFile();
    unsigned int    patientCardId( QString p_qsBarcode );

private slots:
    void on_pbImportUsers_clicked();
    void on_pbExportDatabase_clicked();
    void on_pbImportPatientCards_clicked();
    void on_pbImportPatientCardTypes_clicked();
    void on_pbExit_clicked();
    void on_pbCheckFiles_clicked();
    void on_pbImportPatientCardUsages_clicked();
};

#endif // CDLGMAIN_H
