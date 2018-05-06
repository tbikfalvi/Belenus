#include "dlgprogress.h"
#include "ui_dlgprogress.h"

dlgProgress::dlgProgress(QWidget *parent) : QDialog(parent), ui(new Ui::dlgProgress)
{
    ui->setupUi(this);
}

dlgProgress::~dlgProgress()
{
    delete ui;
}

void dlgProgress::progressShow()
{
    show();
    QApplication::processEvents();
}

void dlgProgress::progressHide()
{
    hide();
}

void dlgProgress::progressInit(QString p_qsText, int p_nMin, int p_nMax)
{
    setProgressText( p_qsText );
    setProgressMin( p_nMin );
    setProgressMax( p_nMax );
}

void dlgProgress::setProgressText( QString p_qsText )
{
    ui->lblProgressText->setText( p_qsText );
}

void dlgProgress::setProgressMin( int p_nMin )
{
    ui->prgIndicator->setMinimum( p_nMin );
}

void dlgProgress::setProgressMax( int p_nMax )
{
    ui->prgIndicator->setMaximum( p_nMax );
}

void dlgProgress::setProgressValue( int p_nValue )
{
    ui->prgIndicator->setValue( p_nValue );
}

void dlgProgress::increaseProgressValue()
{
    ui->prgIndicator->setValue( ui->prgIndicator->value()+1 );
}

void dlgProgress::increaseProgressMax(int p_nMax)
{
    ui->prgIndicator->setMaximum( ui->prgIndicator->maximum() + p_nMax );
}
