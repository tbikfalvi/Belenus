
#include <QPushButton>
#include <QMessageBox>

#include "dlgchangepcvalidity.h"

//====================================================================================
cPCUnit::cPCUnit(QWidget */*p_poParent*/, QString p_qsText)
{
    m_qsCondition   = "";

    QStringList qslText = p_qsText.split( "#" );
    QStringList qslCond = qslText.at(1).split( "|" );

    setText( qslText.at(0) );

    m_qsCondition.append( QString( "(unitTime='%1' AND " ).arg( qslCond.at(0) ) );
    m_qsCondition.append( QString( "validDateTo=\"%1\" AND " ).arg( qslCond.at(1) ) );
    m_qsCondition.append( QString( "patientCardTypeId='%1') " ).arg( qslCond.at(2) ) );

    setToolTip( m_qsCondition );
}

//====================================================================================
//
//
//
//====================================================================================
cDlgChangePCValidity::cDlgChangePCValidity(QWidget *p_poParent , unsigned int p_uiCardId) : QDialog( p_poParent )
{
    m_selValidity       = SV_NONE;
    m_qsUnitCondition   = "";

    setupUi( this );

    setWindowTitle( tr("Change validity") );
    setWindowIcon( QIcon("./resources/40x40_clock.png") );

    pbOk->setIcon( QIcon("./resources/40x40_ok.png") );
    pbCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    connect( rbOneDay, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbOneWeek, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbTwoWeek, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbOneMonth, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbThreeMonths, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbHalfYear, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );
    connect( rbCustom, SIGNAL(toggled(bool)), this, SLOT(selectionChanged()) );

    deCustom->setDate( QDate::currentDate() );

    try
    {
        QStringList qslUnits = g_obGen.getPatientCardUnusedUnits( p_uiCardId );

        for( int i=0;i<qslUnits.count();i++)
        {
            cPCUnit *pPCUnit = new cPCUnit( this, qslUnits.at(i) );
            vlUnits->insertWidget( qvPCUnits.count(), pPCUnit );
            qvPCUnits.append( pPCUnit );
        }
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

}

cDlgChangePCValidity::selValidity cDlgChangePCValidity::selectionValidity(QDate *p_qdDate)
{
    if( m_selValidity == SV_CUSTOM )
    {
        p_qdDate->setDate( deCustom->date().year(), deCustom->date().month(), deCustom->date().day() );
    }
    return m_selValidity;
}

void cDlgChangePCValidity::selectionChanged()
{
    deCustom->setEnabled( false );

    if( rbOneDay->isChecked() )
        m_selValidity = SV_ONEDAY;
    else if( rbOneWeek->isChecked() )
        m_selValidity = SV_ONEWEEK;
    else if( rbTwoWeek->isChecked() )
        m_selValidity = SV_TWOWEEKS;
    else if( rbOneMonth->isChecked() )
        m_selValidity = SV_ONEMONTH;
    else if( rbThreeMonths->isChecked() )
        m_selValidity = SV_THREEMONTHS;
    else if( rbHalfYear->isChecked() )
        m_selValidity = SV_HALFYEAR;
    else if( rbCustom->isChecked() )
    {
        deCustom->setEnabled( true );
        m_selValidity = SV_CUSTOM;
    }
}

void cDlgChangePCValidity::on_pbOk_clicked()
{
    if( m_selValidity == SV_NONE )
    {
        QMessageBox::warning( this, tr( "Warning" ),
                              tr( "You must select a date or time intervall." ) );
        return;
    }

    if( isUnitsNotSelected() )
    {
        QMessageBox::warning( this, tr( "Warning" ),
                              tr( "You must select at least on type of patientcard units." ) );
        return;
    }

    if( m_selValidity == SV_CUSTOM && deCustom->date() < QDate::currentDate() )
    {
        QMessageBox::warning( this, tr( "Warning" ),
                              tr( "Incorrect validation date (%1).\n"
                                  "Validation of an active patientcard could not end in the past." ).arg( deCustom->date().toString(g_poPrefs->getDateFormat()) ) );
        return;
    }

    m_qsUnitCondition = "";

    for( int i=0; i<qvPCUnits.size(); i++ )
    {
        if( qvPCUnits.at(i)->isChecked() )
        {
            if( m_qsUnitCondition.length() > 0 )    m_qsUnitCondition.append( " OR " );

            m_qsUnitCondition.append( qvPCUnits.at(i)->unitCondition() );
        }
    }

    QDialog::accept();
}

bool cDlgChangePCValidity::isUnitsNotSelected()
{
    for( int i=0; i<qvPCUnits.size(); i++ )
    {
        if( qvPCUnits.at(i)->isChecked() )
        {
            return false;
        }
    }

    return true;
}

