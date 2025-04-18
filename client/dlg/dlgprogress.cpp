//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlgprogress.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Hosszabb process-ek alatt megjeleno dialog, homokora helyett
//====================================================================================

#include <QPushButton>
#include <QMessageBox>
#include <QDesktopWidget>

//====================================================================================

#include "dlgprogress.h"
#include "../framework/tracer.h"

//====================================================================================
cDlgProgress::cDlgProgress( QWidget *p_poParent ) : QDialog( p_poParent )
//------------------------------------------------------------------------------------
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/belenus.ico") );
    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );
    progressBar->setVisible( false );

    m_poParent = p_poParent;
}
//====================================================================================
cDlgProgress::~cDlgProgress()
//------------------------------------------------------------------------------------
{
}
//====================================================================================
void cDlgProgress::showProgress()
//------------------------------------------------------------------------------------
{
    lblCaption->setStyleSheet( "QLabel {font: normal;}" );
    lblCaption->setStyleSheet( "QLabel {font: bold;}" );
    lblCaption->setText( tr("Please wait while the requested process ends ...") );
    m_poParent->setCursor( Qt::WaitCursor);
    _centerWindow();
    show();
    QApplication::processEvents();
}
//====================================================================================
void cDlgProgress::showError(QString p_qsMessage)
//------------------------------------------------------------------------------------
{
    lblCaption->setStyleSheet( "QLabel {font: bold; color: red;}" );
    lblCaption->setText( p_qsMessage );
    _centerWindow();
    show();
    QApplication::processEvents();
}
//====================================================================================
void cDlgProgress::showWarning(QString p_qsMessage)
//------------------------------------------------------------------------------------
{
    lblCaption->setStyleSheet( "QLabel {font: bold; color: blue;}" );
    lblCaption->setText( p_qsMessage );
    _centerWindow();
    show();
    QApplication::processEvents();
}
//====================================================================================
void cDlgProgress::showInformation(QString p_qsMessage)
//------------------------------------------------------------------------------------
{
    lblCaption->setStyleSheet( "QLabel {font: normal;}" );
    lblCaption->setStyleSheet( "QLabel {font: bold;}" );
    lblCaption->setText( p_qsMessage );
    _centerWindow();
    show();
    QApplication::processEvents();
}
//====================================================================================
void cDlgProgress::showProgressBar( int p_nMax )
//------------------------------------------------------------------------------------
{
    progressBar->setVisible( true );
    progressBar->setMaximum( p_nMax );
    progressBar->setValue( 0 );
}
//====================================================================================
void cDlgProgress::stepProgressBar()
//------------------------------------------------------------------------------------
{
    progressBar->setValue( progressBar->value()+1 );
}
//====================================================================================
void cDlgProgress::setValue( int p_nValue )
//------------------------------------------------------------------------------------
{
    progressBar->setValue( p_nValue );
}
//====================================================================================
void cDlgProgress::setMax( int p_nMax )
//------------------------------------------------------------------------------------
{
    progressBar->setMaximum( p_nMax );
}
//====================================================================================
void cDlgProgress::hideProgressBar()
//------------------------------------------------------------------------------------
{
    progressBar->setVisible( false );
}
//====================================================================================
void cDlgProgress::hideProgress()
//------------------------------------------------------------------------------------
{
    hide();
    resize( 450, 80 );
    lblCaption->setText( "" );
    m_poParent->setCursor( Qt::ArrowCursor);
}
//====================================================================================
void cDlgProgress::_centerWindow()
//------------------------------------------------------------------------------------
{
//    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth  = m_poParent->width();
    int screenHeight = m_poParent->height();
    int x = m_poParent->x() + ( screenWidth - width() ) / 2;
    int y = m_poParent->y() + ( screenHeight - height() ) / 2;

    move( x, y );
}
//====================================================================================
