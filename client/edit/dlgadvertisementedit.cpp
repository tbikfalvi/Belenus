#include <QPushButton>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>

#include "dlgadvertisementedit.h"

cDlgAdvertisementEdit::cDlgAdvertisementEdit(QWidget *p_poParent, cDBAdvertisements *p_poAdvertisement ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Adwertisement" ) );
    setWindowIcon( QIcon("./resources/40x40_advertisement.png") );

    QPushButton  *poBtnSave = new QPushButton( tr( "&Save" ) );
    QPushButton  *poBtnCancel = new QPushButton( tr( "&Cancel" ) );
    btbButtons->addButton( poBtnSave, QDialogButtonBox::AcceptRole );
    btbButtons->addButton( poBtnCancel, QDialogButtonBox::RejectRole );
    poBtnSave->setIcon( QIcon("./resources/40x40_ok.png") );
    poBtnCancel->setIcon( QIcon("./resources/40x40_cancel.png") );

    pbChangePath->setIcon( QIcon("./resources/40x40_directory.png") );
    pbAddImage->setIcon( QIcon("./resources/40x40_plus.png") );
    pbRemoveImage->setIcon( QIcon("./resources/40x40_cancel.png") );
    pbMoveUp->setIcon( QIcon("./resources/40x40_up.png") );
    pbMoveDown->setIcon( QIcon("./resources/40x40_down.png") );

    m_poAdvertisement = p_poAdvertisement;
    if( m_poAdvertisement )
    {
        ledName->setText( m_poAdvertisement->name() );
        ledCaption->setText( m_poAdvertisement->caption() );
        ledTimerLength->setText( QString::number( m_poAdvertisement->timerLength()) );
        QPixmap  obColorIcon( 24, 24 );
        obColorIcon.fill( QColor( m_poAdvertisement->backgroundColor() ) );
        btnMainBackground->setIcon( QIcon( obColorIcon ) );
        ledImagePath->setText( m_poAdvertisement->path() );
        QStringList qslImages = m_poAdvertisement->filenames().split( "#" );
        for( int i=0; i<qslImages.count(); i++ )
        {
            listImages->addItem( qslImages.at(i) );
        }
        chkActive->setChecked( m_poAdvertisement->active() );
    }
    pbRemoveImage->setEnabled( listImages->count() );
}

cDlgAdvertisementEdit::~cDlgAdvertisementEdit()
{
}

void cDlgAdvertisementEdit::accept ()
{
    bool  boCanBeSaved = true;
    if( (ledName->text() == "") )
    {
        boCanBeSaved = false;
        QMessageBox::critical( this, tr( "Error" ), tr( "Advertisement name cannot be empty." ) );
    }

    if( boCanBeSaved )
    {
        try
        {
            m_poAdvertisement->setLicenceId( g_poPrefs->getLicenceId() );
            m_poAdvertisement->setName( ledName->text() );
            m_poAdvertisement->setActive( chkActive->isChecked() );
            m_poAdvertisement->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }

        QDialog::accept();
    }
}

void cDlgAdvertisementEdit::on_btnMainBackground_clicked()
{
    QColor obNewColor = QColorDialog::getColor( QColor( m_poAdvertisement->backgroundColor() ), this );
    if( obNewColor.isValid() ) m_poAdvertisement->setBackgroundColor( obNewColor.name() );

    QPixmap  obColorIcon( 24, 24 );
    obColorIcon.fill( QColor( m_poAdvertisement->backgroundColor() ) );
    btnMainBackground->setIcon( QIcon( obColorIcon ) );
}

void cDlgAdvertisementEdit::on_pbChangePath_clicked()
{
    QString qsDir = QFileDialog::getExistingDirectory( this,
                                                       tr("Select Directory"),
                                                       ledImagePath->text(),
                                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qsDir.replace( '/', '\\' );

    if( qsDir.length() > 0 )
    {
        ledImagePath->setText( qsDir );
        ledImagePath->setToolTip( qsDir );
    }
}

void cDlgAdvertisementEdit::on_pbAddImage_clicked()
{
    QFileDialog dlgFileOpen( this );

    dlgFileOpen.setDirectory( ledImagePath->text() );
    dlgFileOpen.setFileMode( QFileDialog::ExistingFiles );
    dlgFileOpen.setOptions( QFileDialog::DontResolveSymlinks );
    dlgFileOpen.setViewMode( QFileDialog::Detail );

    if( dlgFileOpen.exec() )
    {
        QString qsDir  = dlgFileOpen.directory().absolutePath();
        qsDir.replace( '/', '\\' );
        if( qsDir.right(1).compare("\\") == 0 )
        {
            qsDir = qsDir.left(qsDir.length()-1);
        }
        for( int i=0; i<dlgFileOpen.selectedFiles().count(); i++ )
        {
            QString qsFile = dlgFileOpen.selectedFiles().at(i).right( dlgFileOpen.selectedFiles().at(i).length()-qsDir.length()-1 );

            listImages->addItem( qsFile );
        }
    }
    pbRemoveImage->setEnabled( listImages->count() );
}

void cDlgAdvertisementEdit::on_pbRemoveImage_clicked()
{
    for( int i=listImages->count()-1; i>-1; i-- )
    {
        if( listImages->item(i)->isSelected() )
        {
            listImages->takeItem(i);
            break;
        }
    }
    pbRemoveImage->setEnabled( listImages->count() );
}

void cDlgAdvertisementEdit::on_pbMoveUp_clicked()
{
    int index = listImages->currentRow();

    QListWidgetItem *pItem = listImages->takeItem( index );
    listImages->insertItem( index-1, pItem );
}

void cDlgAdvertisementEdit::on_pbMoveDown_clicked()
{

}
