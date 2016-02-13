#ifndef CDLGTEST_H
#define CDLGTEST_H

#include <QDialog>

namespace Ui {
class cDlgTest;
}

class cDlgTest : public QDialog
{
    Q_OBJECT
    
public:
    explicit cDlgTest(QWidget *parent = 0);
    ~cDlgTest();
    
private slots:
    void on_pbCalculate_clicked();

    void on_pbImageClick_clicked();

    void on_pbCreateLicenceCodes_clicked();

    void on_pbGenerateMd5Hash_clicked();

private:
    Ui::cDlgTest *ui;
};

#endif // CDLGTEST_H
