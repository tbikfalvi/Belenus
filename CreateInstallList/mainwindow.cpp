
#include <QFileDialog>
#include <QDir>
#include <QFile>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->listFiles, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_pbDeleteFiles_clicked()) );

    QString qsDir = QDir::currentPath();

    qsDir.replace( "\\", "/" );

    if( qsDir.right(1).compare("/") )
        qsDir.append( "/" );

    ui->ledDirectory->setText( qsDir );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbAddFiles_clicked()
{
    QFileDialog dlgSelectFiles( this );
    QStringList qslSelectedFiles;

    dlgSelectFiles.setFileMode(QFileDialog::ExistingFiles);
    dlgSelectFiles.setViewMode(QFileDialog::Detail);
    dlgSelectFiles.setDirectory( ui->ledDirectory->text() );

    if( dlgSelectFiles.exec() )
    {
        qslSelectedFiles = dlgSelectFiles.selectedFiles();

        for( int i=0; i<qslSelectedFiles.count(); i++ )
        {
            QString qsTemp = QString( qslSelectedFiles.at(i) ).remove( ui->ledDirectory->text() );
            ui->listFiles->addItem( qsTemp );
        }
    }
}

void MainWindow::on_pbDeleteFiles_clicked()
{
    ui->listFiles->takeItem( ui->listFiles->currentIndex().row() );
}

void MainWindow::on_pbSelectDir_clicked()
{
    QString qsDir = ui->ledDirectory->text();

    qsDir = QFileDialog::getExistingDirectory( this, tr("Select directory ..."), qsDir );

    if( qsDir.length() )
    {
        qsDir.replace( "\\", "/" );

        if( qsDir.right(1).compare("/") )
            qsDir.append( "/" );

        ui->ledDirectory->setText( qsDir );
    }
}


void MainWindow::on_pbCreateList_clicked()
{
    QStringList qslFileList;

    for( int i=0; i<ui->listFiles->count(); i++ )
    {
        qslFileList << ui->listFiles->item( i )->text();
    }

    ui->teFileList->setPlainText( qslFileList.join("#") );
}

void MainWindow::on_pbCreateInstallFile_clicked()
{
    QFile   qfInstall( QString("%1install.li").arg(ui->ledDirectory->text()) );

    if( !qfInstall.open(QIODevice::WriteOnly | QIODevice::Text) )
        return;

    qfInstall.write( ui->teFileList->toPlainText().toStdString().c_str() );
    qfInstall.close();
}

void MainWindow::on_pbGenerateUpdateFile_clicked()
{

}

void MainWindow::on_pbDeleteAllFiles_clicked()
{
    ui->listFiles->clear();
}
