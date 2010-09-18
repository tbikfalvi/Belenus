/********************************************************************************
** Form generated from reading UI file 'cdlgmain.ui'
**
** Created: 
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CDLGMAIN_H
#define UI_CDLGMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cDlgMain
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QGroupBox *gbSQLConnect;
    QFormLayout *formLayout_2;
    QLabel *lblDatabase;
    QLineEdit *ledDatabase;
    QLabel *lblUser;
    QLineEdit *ledUser;
    QLabel *lblPassword;
    QLineEdit *ledPassword;
    QGroupBox *gbDatabaseSelect;
    QVBoxLayout *verticalLayout_7;
    QRadioButton *rbBelenusConvert;
    QRadioButton *rbBelenus;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbConnect;
    QPushButton *pbDisconnect;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *gbLocation;
    QFormLayout *formLayout;
    QLabel *lblImportPath;
    QLineEdit *ledImportPath;
    QLabel *lblExportPath;
    QLineEdit *ledExportPath;
    QGroupBox *gbUserDefined;
    QFormLayout *formLayout_3;
    QLabel *lblLicence;
    QLineEdit *ledLicenceId;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pbCheckFiles;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pbClearDatabase;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pbImportPatientCardTypes;
    QPushButton *pbImportPatientCards;
    QPushButton *pbImportPatientCardUsages;
    QCheckBox *chkImportToDb;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pbImportUsers;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_4;
    QGroupBox *gbInfo;
    QVBoxLayout *verticalLayout;
    QListWidget *listLog;
    QGroupBox *gbExit;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pbExit;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *cDlgMain)
    {
        if (cDlgMain->objectName().isEmpty())
            cDlgMain->setObjectName(QString::fromUtf8("cDlgMain"));
        cDlgMain->resize(915, 511);
        horizontalLayout_4 = new QHBoxLayout(cDlgMain);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(cDlgMain);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_4 = new QVBoxLayout(tab);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gbSQLConnect = new QGroupBox(tab);
        gbSQLConnect->setObjectName(QString::fromUtf8("gbSQLConnect"));
        formLayout_2 = new QFormLayout(gbSQLConnect);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        lblDatabase = new QLabel(gbSQLConnect);
        lblDatabase->setObjectName(QString::fromUtf8("lblDatabase"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, lblDatabase);

        ledDatabase = new QLineEdit(gbSQLConnect);
        ledDatabase->setObjectName(QString::fromUtf8("ledDatabase"));
        ledDatabase->setMinimumSize(QSize(200, 0));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, ledDatabase);

        lblUser = new QLabel(gbSQLConnect);
        lblUser->setObjectName(QString::fromUtf8("lblUser"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, lblUser);

        ledUser = new QLineEdit(gbSQLConnect);
        ledUser->setObjectName(QString::fromUtf8("ledUser"));
        ledUser->setMinimumSize(QSize(200, 0));
        ledUser->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, ledUser);

        lblPassword = new QLabel(gbSQLConnect);
        lblPassword->setObjectName(QString::fromUtf8("lblPassword"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, lblPassword);

        ledPassword = new QLineEdit(gbSQLConnect);
        ledPassword->setObjectName(QString::fromUtf8("ledPassword"));
        ledPassword->setMinimumSize(QSize(200, 0));
        ledPassword->setEchoMode(QLineEdit::Password);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, ledPassword);


        horizontalLayout->addWidget(gbSQLConnect);

        gbDatabaseSelect = new QGroupBox(tab);
        gbDatabaseSelect->setObjectName(QString::fromUtf8("gbDatabaseSelect"));
        verticalLayout_7 = new QVBoxLayout(gbDatabaseSelect);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        rbBelenusConvert = new QRadioButton(gbDatabaseSelect);
        rbBelenusConvert->setObjectName(QString::fromUtf8("rbBelenusConvert"));

        verticalLayout_7->addWidget(rbBelenusConvert);

        rbBelenus = new QRadioButton(gbDatabaseSelect);
        rbBelenus->setObjectName(QString::fromUtf8("rbBelenus"));

        verticalLayout_7->addWidget(rbBelenus);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_5);


        horizontalLayout->addWidget(gbDatabaseSelect);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 79, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pbConnect = new QPushButton(tab);
        pbConnect->setObjectName(QString::fromUtf8("pbConnect"));

        horizontalLayout_2->addWidget(pbConnect);

        pbDisconnect = new QPushButton(tab);
        pbDisconnect->setObjectName(QString::fromUtf8("pbDisconnect"));

        horizontalLayout_2->addWidget(pbDisconnect);


        verticalLayout_4->addLayout(horizontalLayout_2);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gbLocation = new QGroupBox(tab_2);
        gbLocation->setObjectName(QString::fromUtf8("gbLocation"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gbLocation->sizePolicy().hasHeightForWidth());
        gbLocation->setSizePolicy(sizePolicy);
        formLayout = new QFormLayout(gbLocation);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        lblImportPath = new QLabel(gbLocation);
        lblImportPath->setObjectName(QString::fromUtf8("lblImportPath"));

        formLayout->setWidget(0, QFormLayout::LabelRole, lblImportPath);

        ledImportPath = new QLineEdit(gbLocation);
        ledImportPath->setObjectName(QString::fromUtf8("ledImportPath"));
        ledImportPath->setMinimumSize(QSize(300, 0));

        formLayout->setWidget(0, QFormLayout::FieldRole, ledImportPath);

        lblExportPath = new QLabel(gbLocation);
        lblExportPath->setObjectName(QString::fromUtf8("lblExportPath"));

        formLayout->setWidget(1, QFormLayout::LabelRole, lblExportPath);

        ledExportPath = new QLineEdit(gbLocation);
        ledExportPath->setObjectName(QString::fromUtf8("ledExportPath"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ledExportPath);


        verticalLayout_2->addWidget(gbLocation);

        gbUserDefined = new QGroupBox(tab_2);
        gbUserDefined->setObjectName(QString::fromUtf8("gbUserDefined"));
        formLayout_3 = new QFormLayout(gbUserDefined);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        lblLicence = new QLabel(gbUserDefined);
        lblLicence->setObjectName(QString::fromUtf8("lblLicence"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, lblLicence);

        ledLicenceId = new QLineEdit(gbUserDefined);
        ledLicenceId->setObjectName(QString::fromUtf8("ledLicenceId"));
        ledLicenceId->setMaximumSize(QSize(50, 16777215));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, ledLicenceId);


        verticalLayout_2->addWidget(gbUserDefined);

        verticalSpacer_3 = new QSpacerItem(20, 77, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        pbCheckFiles = new QPushButton(tab_2);
        pbCheckFiles->setObjectName(QString::fromUtf8("pbCheckFiles"));

        horizontalLayout_3->addWidget(pbCheckFiles);


        verticalLayout_2->addLayout(horizontalLayout_3);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        pbClearDatabase = new QPushButton(tab_3);
        pbClearDatabase->setObjectName(QString::fromUtf8("pbClearDatabase"));

        horizontalLayout_7->addWidget(pbClearDatabase);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);


        verticalLayout_6->addLayout(horizontalLayout_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        pbImportPatientCardTypes = new QPushButton(tab_3);
        pbImportPatientCardTypes->setObjectName(QString::fromUtf8("pbImportPatientCardTypes"));

        horizontalLayout_5->addWidget(pbImportPatientCardTypes);

        pbImportPatientCards = new QPushButton(tab_3);
        pbImportPatientCards->setObjectName(QString::fromUtf8("pbImportPatientCards"));

        horizontalLayout_5->addWidget(pbImportPatientCards);

        pbImportPatientCardUsages = new QPushButton(tab_3);
        pbImportPatientCardUsages->setObjectName(QString::fromUtf8("pbImportPatientCardUsages"));

        horizontalLayout_5->addWidget(pbImportPatientCardUsages);

        chkImportToDb = new QCheckBox(tab_3);
        chkImportToDb->setObjectName(QString::fromUtf8("chkImportToDb"));

        horizontalLayout_5->addWidget(chkImportToDb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_6->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        pbImportUsers = new QPushButton(tab_3);
        pbImportUsers->setObjectName(QString::fromUtf8("pbImportUsers"));

        horizontalLayout_6->addWidget(pbImportUsers);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_6->addLayout(horizontalLayout_6);

        verticalSpacer_4 = new QSpacerItem(20, 172, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_3, QString());

        verticalLayout_3->addWidget(tabWidget);

        gbInfo = new QGroupBox(cDlgMain);
        gbInfo->setObjectName(QString::fromUtf8("gbInfo"));
        verticalLayout = new QVBoxLayout(gbInfo);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        listLog = new QListWidget(gbInfo);
        listLog->setObjectName(QString::fromUtf8("listLog"));

        verticalLayout->addWidget(listLog);


        verticalLayout_3->addWidget(gbInfo);


        horizontalLayout_4->addLayout(verticalLayout_3);

        gbExit = new QGroupBox(cDlgMain);
        gbExit->setObjectName(QString::fromUtf8("gbExit"));
        verticalLayout_5 = new QVBoxLayout(gbExit);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pbExit = new QPushButton(gbExit);
        pbExit->setObjectName(QString::fromUtf8("pbExit"));

        verticalLayout_5->addWidget(pbExit);

        verticalSpacer = new QSpacerItem(20, 421, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        horizontalLayout_4->addWidget(gbExit);


        retranslateUi(cDlgMain);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(cDlgMain);
    } // setupUi

    void retranslateUi(QDialog *cDlgMain)
    {
        cDlgMain->setWindowTitle(QApplication::translate("cDlgMain", "Belenus DB Tool application (c) Pagony Multimedia Studio Bt.", 0, QApplication::UnicodeUTF8));
        gbSQLConnect->setTitle(QApplication::translate("cDlgMain", " Settings ", 0, QApplication::UnicodeUTF8));
        lblDatabase->setText(QApplication::translate("cDlgMain", "Database : ", 0, QApplication::UnicodeUTF8));
        lblUser->setText(QApplication::translate("cDlgMain", "User name : ", 0, QApplication::UnicodeUTF8));
        lblPassword->setText(QApplication::translate("cDlgMain", "Password : ", 0, QApplication::UnicodeUTF8));
        gbDatabaseSelect->setTitle(QApplication::translate("cDlgMain", " Select database ", 0, QApplication::UnicodeUTF8));
        rbBelenusConvert->setText(QApplication::translate("cDlgMain", "Belenus convert", 0, QApplication::UnicodeUTF8));
        rbBelenus->setText(QApplication::translate("cDlgMain", "Belenus main", 0, QApplication::UnicodeUTF8));
        pbConnect->setText(QApplication::translate("cDlgMain", "Connect to database", 0, QApplication::UnicodeUTF8));
        pbDisconnect->setText(QApplication::translate("cDlgMain", "Disconnect", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("cDlgMain", "SQL Connection", 0, QApplication::UnicodeUTF8));
        gbLocation->setTitle(QApplication::translate("cDlgMain", " Path of ... ", 0, QApplication::UnicodeUTF8));
        lblImportPath->setText(QApplication::translate("cDlgMain", "DAT files : ", 0, QApplication::UnicodeUTF8));
        lblExportPath->setText(QApplication::translate("cDlgMain", "SQL files :", 0, QApplication::UnicodeUTF8));
        gbUserDefined->setTitle(QApplication::translate("cDlgMain", " User defined data ", 0, QApplication::UnicodeUTF8));
        lblLicence->setText(QApplication::translate("cDlgMain", "Licence Id : ", 0, QApplication::UnicodeUTF8));
        pbCheckFiles->setText(QApplication::translate("cDlgMain", "Check parameters", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("cDlgMain", "Import settings", 0, QApplication::UnicodeUTF8));
        pbClearDatabase->setText(QApplication::translate("cDlgMain", "Clear database (apply only BelenusConvert)", 0, QApplication::UnicodeUTF8));
        pbImportPatientCardTypes->setText(QApplication::translate("cDlgMain", "Import Patientcard Types", 0, QApplication::UnicodeUTF8));
        pbImportPatientCards->setText(QApplication::translate("cDlgMain", "Import Patientcards", 0, QApplication::UnicodeUTF8));
        pbImportPatientCardUsages->setText(QApplication::translate("cDlgMain", "Import Patientcard Uses", 0, QApplication::UnicodeUTF8));
        chkImportToDb->setText(QApplication::translate("cDlgMain", "Import to SQL DB", 0, QApplication::UnicodeUTF8));
        pbImportUsers->setText(QApplication::translate("cDlgMain", "Import Users", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("cDlgMain", "Import", 0, QApplication::UnicodeUTF8));
        gbInfo->setTitle(QApplication::translate("cDlgMain", " Log information ", 0, QApplication::UnicodeUTF8));
        gbExit->setTitle(QString());
        pbExit->setText(QApplication::translate("cDlgMain", "Exit application", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class cDlgMain: public Ui_cDlgMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CDLGMAIN_H
