#ifndef DLGINPUTSTART_H
#define DLGINPUTSTART_H

#include "../belenus.h"
#include "../ui_dlginputstart.h"

class cDlgInputStart : public QDialog, protected Ui::dlgInputStart
{
    Q_OBJECT

public:
    cDlgInputStart( QWidget *p_poParent = 0 );
    virtual ~cDlgInputStart();

    void setInitialText( const QString &p_stText );

private slots:
    void on_pbCancel_clicked();
    void on_pbSelect_clicked();
    void on_ledInputStart_textChanged(QString );
};

#endif
