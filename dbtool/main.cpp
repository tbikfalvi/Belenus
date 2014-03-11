#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

QTranslator     *poTransTool;
QTranslator     *poTransQT;
QApplication    *apMainApp;
QString          g_qsCurrentPath;

int main(int argc, char *argv[])
{
    apMainApp = new QApplication(argc, argv);

    g_qsCurrentPath = QDir::currentPath();

    poTransTool = new QTranslator();
    poTransQT = new QTranslator();

    poTransTool->load( QString("%1\\lang\\dbtool_hu.qm").arg(QDir::currentPath()) );
    poTransQT->load( QString("%1\\lang\\qt_hu.qm").arg(QDir::currentPath()) );

    apMainApp->installTranslator( poTransTool );
    apMainApp->installTranslator( poTransQT );

    apMainApp->setWindowIcon( QIcon(":/icons/DBTool.ico") );

    MainWindow w;
    w.show();

    return apMainApp->exec();
}





/*void MainWindow::on_pbExportPCTDat_clicked()
{
    FILE    *file = NULL;

    int      nBerletTypeCount = m_qvPatientCardTypes.size();

    setCursor( Qt::WaitCursor);

    file = fopen( "brlttpsfsv_new.dat", "wb" );
    if( file != NULL )
    {
        fwrite( m_strPatiencardTypeVersion, 10, 1, file );
        fwrite( &nBerletTypeCount, 4, 1, file );

        typ_berlettipus   stTemp;

        for( int i=0; i<nBerletTypeCount; i++ )
        {
            stTemp = m_qvPatientCardTypes.at(i);

            _EnCode( stTemp.strNev, 50 );

            fwrite( &stTemp.nID, 4, 1, file );
            fwrite( &stTemp.nAr, 4, 1, file );
            fwrite( &stTemp.nEgyseg, 4, 1, file );
            fwrite( stTemp.strNev, 50, 1, file );
            fwrite( &stTemp.nErvTolEv, 4, 1, file );
            fwrite( &stTemp.nErvTolHo, 4, 1, file );
            fwrite( &stTemp.nErvTolNap, 4, 1, file );
            fwrite( &stTemp.nErvIgEv, 4, 1, file );
            fwrite( &stTemp.nErvIgHo, 4, 1, file );
            fwrite( &stTemp.nErvIgNap, 4, 1, file );
            fwrite( &stTemp.nErvNapok, 4, 1, file );
            fwrite( &stTemp.bSzolariumHaszn, 1, 1, file );
            if( m_nProgramType == DBTool::Sensolite )
            {
                fwrite( &stTemp.nEgysegIdo, 4, 1, file );
            }
        }
        fclose( file );
        //ui->listLog->addItem( tr( "'brlttpsfsv_new.dat' file created with %2 patientcard types" ).arg(nBerletTypeCount) );
    }
    else
    {
        //ui->listLog->addItem( tr( "Error occured during opening brlttpsfsv_new.dat file." ) );
    }

    setCursor( Qt::ArrowCursor);
}*/
//====================================================================================
/*void MainWindow::_loadPatientCardTypeImport()
{
    QFile file( ui->ledImportFileName->text() );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    if( !in.atEnd() )
    {
        QString qsLine = in.readLine();

        if( qsLine.compare("#Ar,Egyseg,Nev,TolEv,TolHo,TolNap,IgEv,IgHo,IgNap,Napok,Hasznalat,Egysegido") )
        {
            QMessageBox::warning( this, tr("Attention"),
                                  tr("The selected file is not a valid import file.\n"
                                     "Please select a valid file and restart the process.") );
            return;
        }
    }

    int inPCTAppend = 0;

    while( !in.atEnd() )
    {
        QString     qsLine = in.readLine();
        QStringList qslRecord = qsLine.split( QChar(',') );

        if( qslRecord.count() == 12 )
        {
            typ_berlettipus stTemp;

            stTemp.nID              = _getPatientCardTypeId();
            stTemp.nAr              = qslRecord.at(0).toInt();
            stTemp.nEgyseg          = qslRecord.at(1).toInt();
            strcpy( stTemp.strNev, qslRecord.at(2).toStdString().c_str() );
            stTemp.nErvTolEv        = qslRecord.at(3).toInt();
            stTemp.nErvTolHo        = qslRecord.at(4).toInt();
            stTemp.nErvTolNap       = qslRecord.at(5).toInt();
            stTemp.nErvIgEv         = qslRecord.at(6).toInt();
            stTemp.nErvIgHo         = qslRecord.at(7).toInt();
            stTemp.nErvIgNap        = qslRecord.at(8).toInt();
            stTemp.nErvNapok        = qslRecord.at(9).toInt();
            stTemp.bSzolariumHaszn  = qslRecord.at(10).toInt();
            stTemp.nEgysegIdo       = qslRecord.at(11).toInt();

            m_qvPatientCardTypes.append( stTemp );
            inPCTAppend++;
        }
    }

    //ui->listLog->addItem( tr("Importing %1 patientcards finished.").arg(inPCTAppend) );
}*/

/*void MainWindow::on_pbSelectImportFile_clicked()
{
    QString qsFileName = QFileDialog::getOpenFileName(this, tr("Open import file"), "", tr("Import Files (*.imp)"));

    if( qsFileName.length() )
        ui->ledImportFileName->setText( qsFileName );
}*/

/*void MainWindow::on_pbImportPCTText_clicked()
{
    if( ui->ledImportFileName->text().length() < 1 )
    {
        QMessageBox::warning( this, tr("Attention"),
                              tr("No import file selected."));
        return;
    }

    _loadPatientCardTypeImport();
}*/
