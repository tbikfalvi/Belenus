#include <QPushButton>
#include <QMessageBox>

#include "dlginputstart.h"

cDlgInputStart::cDlgInputStart( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Entering ...") );
    pbSelect->setIcon( QIcon("./resources/40x40_ok.gif") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.gif") );
}

cDlgInputStart::~cDlgInputStart()
{
}

void cDlgInputStart::on_ledInputStart_textChanged(QString )
{
    if( ledInputStart->text().length() < 4 && containsOnlyNumbers(ledInputStart->text()) )
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

bool cDlgInputStart::containsOnlyNumbers( QString p_stTemp )
{
    bool bRet = true;

    for( int i=0; i<p_stTemp.length(); i++ )
    {
        if( !p_stTemp.at(i).isDigit() )
        {
            bRet = false;
            break;
        }
    }

    return bRet;
}
