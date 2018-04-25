#include <QPushButton>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QDir>

#include "dlgadvertisementedit.h"

cDlgAdvertisementEdit::cDlgAdvertisementEdit(QWidget *p_poParent,
                                             cDBAdvertisements *p_poAdvertisement,
                                             bool p_bCanBeInactivate ) : QDialog( p_poParent )
{
    setupUi( this );

    setWindowTitle( tr( "Advertisement" ) );
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

    chkActive->setEnabled( p_bCanBeInactivate );

    m_poAdvertisement = p_poAdvertisement;

    if( m_poAdvertisement )
    {
        ledName->setText( m_poAdvertisement->name() );
        ledCaption->setText( m_poAdvertisement->caption() );
        ledTimerLength->setText( QString::number( m_poAdvertisement->timerLength()) );
        QPixmap  obColorIcon( 24, 24 );
        obColorIcon.fill( QColor( m_poAdvertisement->backgroundColor() ) );
        btnMainBackground->setIcon( QIcon( obColorIcon ) );
        ledImagePath->setText( m_poAdvertisement->path().replace("\\","/") );
        if( m_poAdvertisement->filenames().length() > 0 )
        {
            QStringList qslImages = m_poAdvertisement->filenames().split( "#" );
            for( int i=0; i<qslImages.count(); i++ )
            {
                listImages->addItem( qslImages.at(i) );
            }
        }
        chkActive->setChecked( m_poAdvertisement->active() );
    }

    connect( ledName, SIGNAL(textChanged(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledCaption, SIGNAL(textChanged(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledTimerLength, SIGNAL(textChanged(QString)), this, SLOT(slotRefreshWarningColors()) );
    connect( ledImagePath, SIGNAL(textChanged(QString)), this, SLOT(slotRefreshWarningColors()) );

    on_listImages_itemSelectionChanged();

    slotRefreshWarningColors();

    QPoint  qpDlgSize = g_poPrefs->getDialogSize( "EditAdvertisement", QPoint(516,480) );
    resize( qpDlgSize.x(), qpDlgSize.y() );
}

cDlgAdvertisementEdit::~cDlgAdvertisementEdit()
{
    g_poPrefs->setDialogSize( "EditAdvertisement", QPoint( width(), height() ) );
}

void cDlgAdvertisementEdit::accept ()
{
    bool  boCanBeSaved = true;

    QString qsDir = ledImagePath->text();

    qsDir.replace( '\\', '/' );

    if( chkActive->isChecked() )
    {
        if( (ledName->text() == "") )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Advertisement name cannot be empty." ) );
        }

/*        if( (ledCaption->text() == "") )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "Advertisement caption cannot be empty." ) );
        }*/

        ledTimerLength->setText( QString::number(ledTimerLength->text().toInt()) );

        QString qsCurr = QDir::currentPath();

        if( QDir::setCurrent( qsDir ) )
        {
            QDir::setCurrent( qsCurr );
        }
        else
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "The defined image path is not exists." ) );
        }

        if( listImages->count() < 1 )
        {
            boCanBeSaved = false;
            QMessageBox::critical( this, tr( "Error" ), tr( "At least one image must be added to the list." ) );
        }
    }

    if( boCanBeSaved )
    {
        try
        {
            QStringList qslItems;

            for( int i=0; i<listImages->count(); i++ )
            {
                qslItems << listImages->item(i)->text();
            }

            m_poAdvertisement->setLicenceId( g_poPrefs->getLicenceId() );
            m_poAdvertisement->setName( ledName->text() );
            m_poAdvertisement->setCaption( ledCaption->text() );
            m_poAdvertisement->setTimerLength( ledTimerLength->text().toUInt() );
            m_poAdvertisement->setPath( qsDir );
            m_poAdvertisement->setFilenames( qslItems.join( "#" ) );
            m_poAdvertisement->setActive( chkActive->isChecked() );
            m_poAdvertisement->save();
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
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
    qsDir.replace( '\\', '/' );

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
        qsDir.replace( '\\', '/' );
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
    slotRefreshWarningColors();
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

    if( index == 0 )
        return;

    QListWidgetItem *pItem = listImages->takeItem( index );
    listImages->insertItem( index-1, pItem );
    listImages->setCurrentRow( index-1 );
}

void cDlgAdvertisementEdit::on_pbMoveDown_clicked()
{
    int index = listImages->currentRow();

    if( index == listImages->count()-1 )
        return;

    QListWidgetItem *pItem = listImages->takeItem( index );
    listImages->insertItem( index+1, pItem );
    listImages->setCurrentRow( index+1 );
}

void cDlgAdvertisementEdit::on_listImages_itemSelectionChanged()
{
    if( listImages->currentRow() == -1 )
    {
        pbRemoveImage->setEnabled( false );
        pbMoveUp->setEnabled( false );
        pbMoveDown->setEnabled( false );
    }
    else
    {
        pbRemoveImage->setEnabled( true );
        pbMoveUp->setEnabled( true );
        pbMoveDown->setEnabled( true );
    }
    slotRefreshWarningColors();
}

void cDlgAdvertisementEdit::slotRefreshWarningColors()
{
    lblName->setStyleSheet( "QLabel {font: normal;}" );
    lblCaption->setStyleSheet( "QLabel {font: normal;}" );
    lblTimerLength->setStyleSheet( "QLabel {font: normal;}" );
    lblImagePath->setStyleSheet( "QLabel {font: normal;}" );
    lblImages->setStyleSheet( "QLabel {font: normal;}" );

    if( ledName->text().length() == 0 )
        lblName->setStyleSheet( "QLabel {font: bold; color: red;}" );

    if( ledCaption->text().length() == 0 )
        lblCaption->setStyleSheet( "QLabel {font: bold; color: blue;}" );

    if( ledTimerLength->text().length() == 0 )
        lblTimerLength->setStyleSheet( "QLabel {font: bold; color: red;}" );

    QString qsCurr = QDir::currentPath();

    if( QDir::setCurrent( ledImagePath->text() ) )
    {
        QDir::setCurrent( qsCurr );
    }
    else
    {
        lblImagePath->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }

    if( listImages->count() < 1 )
    {
        lblImages->setStyleSheet( "QLabel {font: bold; color: red;}" );
    }
}
