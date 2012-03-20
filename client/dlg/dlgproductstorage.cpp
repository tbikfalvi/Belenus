#include "dlgproductstorage.h"
#include "ui_dlgproductstorage.h"

dlgProductStorage::dlgProductStorage( QWidget *parent, cDBProduct *p_poProduct ) : QDialog(parent)
{
    setupUi( this );

    m_bInit = true;

    setWindowTitle( tr( "Product storage" ) );
    setWindowIcon( QIcon("./resources/40x40_storage.png") );

    pbSave->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbRefreshNP->setIcon( QIcon("./resources/40x40_refresh.png") );
    pbRefreshSP->setIcon( QIcon("./resources/40x40_refresh.png") );

    cmbCassa->addItem( tr("<General cassa>") );
    cmbCassa->addItem( tr("Daily, user related cassa") );
    cmbCassa->setToolTip( tr("Select appropriate cassa type for product action.\n\n"
                             "<General cassa> is related to the solarium studio\n"
                             "any action will not modify the balance of the actual cassa\n"
                             "<Daily, user related cassa> is the currently used cassa\n"
                             "any action will increase or decrease the balance of the cassa.") );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productActionType WHERE active=1" ) );
    while( poQuery->next() )
    {
        cmbProductAction->addItem( poQuery->value(2).toString(), poQuery->value(0).toInt() );

        QString qsChangeCount = tr("not modify");
        if( poQuery->value(3).toInt() )
        {
            qsChangeCount = tr("increase");
        }
        else if( poQuery->value(4).toInt() )
        {
            qsChangeCount = tr("decrease");
        }
        QString qsToolTip = tr("The action <%1> will %2\n"
                               "the count of the selected product.\n"
                               "Based on the selected cassa type, the given price will be\n"
                               "added to actual cassa or to the general ledger.").arg(poQuery->value(2).toString()).arg(qsChangeCount);

        m_qslActionTooltip << qsToolTip;
    }

    if( p_poProduct )
    {
        ledProductName->setText( p_poProduct->name() );
    }
    ledNetPrice->setText( "0" );
    ledVatPercent->setText( "0" );
    ledSumPrice->setText( "0" );
    ledProductCount->setText( "0" );

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "ProductStorage", QPoint(400,310) );
    resize( qpDlgSize.x(), qpDlgSize.y() );

    m_bInit = false;

    on_cmbProductAction_currentIndexChanged( 0 );
}

dlgProductStorage::~dlgProductStorage()
{
    g_poPrefs->setDialogSize( "ProductStorage", QPoint( width(), height() ) );
}

void dlgProductStorage::on_pbSave_clicked()
{
    bool  boCanBeSaved = true;

    if( boCanBeSaved )
    {
        try
        {
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        QDialog::accept();
    }
}

void dlgProductStorage::on_pbCancel_clicked()
{
    QDialog::reject();
}

void dlgProductStorage::on_ledNetPrice_textEdited(QString )
{
    int nNetPrice = ledNetPrice->text().toInt();
    int nVatPercent = ledVatPercent->text().toInt();

    int nSumPrice = nNetPrice + (nNetPrice * nVatPercent) / 100;

    ledSumPrice->setText( QString::number(nSumPrice) );
}

void dlgProductStorage::on_ledSumPrice_textEdited(QString )
{
    int nSumPrice = ledSumPrice->text().toInt();
    int nVatPercent = ledVatPercent->text().toInt();

    int nNetPrice = nSumPrice  * 100 / (100 + nVatPercent);

    ledNetPrice->setText( QString::number(nNetPrice) );
}

void dlgProductStorage::on_pbRefreshNP_clicked()
{
    on_ledSumPrice_textEdited( ledSumPrice->text() );
}

void dlgProductStorage::on_pbRefreshSP_clicked()
{
    on_ledNetPrice_textEdited( ledNetPrice->text() );
}

void dlgProductStorage::on_cmbProductAction_currentIndexChanged(int index)
{
    if( !m_bInit )
        cmbProductAction->setToolTip( m_qslActionTooltip.at(index) );
}
