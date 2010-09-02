#ifndef DLGZIPREGIONCITYSELECT_H
#define DLGZIPREGIONCITYSELECT_H

#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>

#include "dlgzipregioncity.h"

class cDlgZipRegionCitySelect : public cDlgZipRegionCity
{
    Q_OBJECT

public:
    QPushButton *pbSelect;
    QPushButton *pbCancel;

    cDlgZipRegionCitySelect( QWidget *p_poParent = 0, QString p_qsCity = "" );
    virtual ~cDlgZipRegionCitySelect();

    unsigned int selected();

private:
    QString m_qsCity;

protected:
    virtual void setupTableView();
    virtual void enableButtons();

protected slots:
    virtual void refreshTable();
    virtual void newClicked( bool );
    virtual void deleteClicked( bool );
    virtual void editClicked( bool );
    virtual void on_pbSelect_clicked();
};

#endif // DLGZIPREGIONCITYSELECT_H
