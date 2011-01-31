//====================================================================================
//
//====================================================================================
#ifndef KIWISUNBERLET_H
#define KIWISUNBERLET_H

//====================================================================================

#include <QDialog>
#include <QString>
#include <QStringList>

//====================================================================================

#include "ui_kiwisunberlet.h"

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
class KiwiSunBerlet : public QDialog, protected Ui_KiwiSunBerlet
{
    Q_OBJECT

public:
    KiwiSunBerlet(QWidget *parent = 0);
    ~KiwiSunBerlet();

private:

    QStringList      m_qslPCUBarcodes;
    QString          m_qsPCUFileName;

    void            _loadPatientCardUsages();

    void            _EnCode( char *str, int size );
    void            _DeCode( char *str, int size );

private slots:
    void on_pbImportPCU_clicked();
    void on_pbSelectPCU_clicked();
};
//====================================================================================

#endif // KIWISUNBERLET_H
