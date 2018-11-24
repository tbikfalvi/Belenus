#ifndef DLGEMAILSEDIT_H
#define DLGEMAILSEDIT_H

#include "belenus.h"
#include "ui_dlgemailsedit.h"
#include "db/dbsendmail.h"

class cDlgEmailsEdit : public QDialog, protected Ui::dlgEmailsEdit
{
    Q_OBJECT

public:
    cDlgEmailsEdit( QWidget *p_poParent = 0, cDBSendMail *p_poEmails = NULL );
    virtual ~cDlgEmailsEdit();

public slots:
    virtual void accept ();

private:
//    void checkRegionZipCity();

protected:
    cDBSendMail *m_poEmails;

private slots:
//    void on_pbCitySearch_clicked();
//    void on_ledZip_textEdited(QString );
};

#endif
