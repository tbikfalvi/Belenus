#ifndef DLGEXPORTIMPORT_H
#define DLGEXPORTIMPORT_H

#include <QFile>
#include <QDomDocument>

#include "../belenus.h"
#include "ui_dlgExportImport.h"
#include "dlgprogress.h"

class cDlgExportImport : public QDialog, protected Ui::dlgExportImport
{
    Q_OBJECT

    enum teData
    {
        DD_GUEST = 1,
        DD_PCARD,
        DD_PROD
    };

public:

    enum teProcessType
    {
        PT_EXPORT = 1,
        PT_IMPORT
    };

    cDlgExportImport( QWidget *p_poParent = 0, teProcessType p_teExportImport = PT_EXPORT );
    virtual ~cDlgExportImport();

private:

    teData           m_teData;
    teProcessType    m_teExportImport;
    QString          m_qsFile;
    QString          m_qsDir;
    cDlgProgress    *m_dlgProgress;
    QDomDocument    *obProcessDoc;

    void            _processPage( bool p_bMoveForward = true );
    void            _ExportGuests();
    bool            _LoadImportFile();
    void            _importPatientCardTypes();
    void            _importPanelUses();
    void            _importProductTypes();
    void            _importProducts();
    void            _importDiscounts();
    void            _importSkinTypes();

private slots:

    void on_pbPrev_clicked();
    void on_pbNext_clicked();
    void on_pbExecute_clicked();
    void on_pbExit_clicked();
//    void on_rbGuest_clicked();
//    void on_rbPatientCard_clicked();
//    void on_rbProduct_clicked();
    void on_pbDir_clicked();
    void slot_import_selected();
    void slot_export_selected();
};

#endif
