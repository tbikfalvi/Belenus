//====================================================================================
//
//====================================================================================
#ifndef KIWISUNBERLET_H
#define KIWISUNBERLET_H

//====================================================================================

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>

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
class KiwiSunBerlet : public QDialog, public Ui_KiwiSunBerlet
{
    Q_OBJECT

public:
    KiwiSunBerlet(QWidget *parent = 0);
    ~KiwiSunBerlet();

private:

    QStringList                  m_qslPCUBarcodes;
    QString                      m_qsPCFileName;
    QString                      m_qsPCTFileName;
    QString                      m_qsPCUFileName;
    QVector<typ_berlet>          m_qvPatientCards;
    QVector<typ_berlettipus>     m_qvPatientCardTypes;
    QVector<typ_berlethasznalat> m_qvPatientCardUsage;

    void                        _enableImportButton();
    void                        _loadPatientCards();
    void                        _loadPatientCardTypes();
    void                        _loadPatientCardUsages();
    void                        _addToPatientCards( typ_berlethasznalat p_stPCU );
    void                        _fillPatientCard( QString p_qsBarcode, int p_nPCTId, int p_nMaxEgyseg, int p_nUsedEgyseg );
    void                        _savePatientCardsToFile();
    int                         _getPatientCardUsages( QString p_qsBarcode );
    int                         _getMaxUsages( int p_nPCTId );

    void                        _EnCode( char *str, int size );
    void                        _DeCode( char *str, int size );

private slots:
    void on_pbSelectPC_clicked();
    void on_pbSelectPCT_clicked();
    void on_pbSelectPCU_clicked();
    void on_pbImport_clicked();
    void on_dateImportStart_dateChanged( QDate );
    void on_pbProcessPCU_clicked();
    void on_pbCreatePC_clicked();
};
//====================================================================================

#endif // KIWISUNBERLET_H
