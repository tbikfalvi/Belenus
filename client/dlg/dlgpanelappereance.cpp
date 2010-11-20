#include <QPushButton>
#include <QMessageBox>
#include <QColor>
#include <QColorDialog>

#include "dlgpanelappereance.h"

cDlgPanelAppereance::cDlgPanelAppereance( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr("Panel settings") );
    setWindowIcon( QIcon("./resources/40x40_device.png") );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( QString("#00ff00") ) );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );
}

cDlgPanelAppereance::~cDlgPanelAppereance()
{
}


void cDlgPanelAppereance::on_pbBackgroundColor_clicked()
{
    QColor obNewColor = QColorDialog::getColor( QColor( QString("#00ff00") ), this );
//    if( obNewColor.isValid() ) g_poPrefs->setMainBackground( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( obNewColor.name() ) );
    pbBackgroundColor->setIcon( QIcon( obColorIcon ) );
}
