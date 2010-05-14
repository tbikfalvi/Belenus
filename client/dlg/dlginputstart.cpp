#include <QPushButton>
#include <QMessageBox>

#include "dlginputstart.h"

cDlgInputStart::cDlgInputStart( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Entering ...") );
    lblAction->setText( tr("Entering code ...") );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
}

cDlgInputStart::~cDlgInputStart()
{
}

void cDlgInputStart::setInitialText( const QString &p_stText )
{
    ledInputStart->setText( p_stText );
}

void cDlgInputStart::on_ledInputStart_textChanged(QString )
{
    bool boIsANumber = false;
    ledInputStart->text().toUInt( &boIsANumber );

    if( (ledInputStart->text().length() < 4) && (boIsANumber) )
    {
        lblAction->setText( tr("Entering time period ...") );
    }
    else if( ledInputStart->text().length() > 3 )
    {
        lblAction->setText( tr("Entering barcode ...") );
    }
    else
    {
        lblAction->setText( tr("Entering code ...") );
    }
}

void cDlgInputStart::on_pbSelect_clicked()
{
    QDialog::accept();

}

void cDlgInputStart::on_pbCancel_clicked()
{
    QDialog::reject();
}
