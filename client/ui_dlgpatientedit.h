/********************************************************************************
** Form generated from reading UI file 'dlgpatientedit.ui'
**
** Created: Fri May 7 17:44:14 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPATIENTEDIT_H
#define UI_DLGPATIENTEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_dlgPatientEdit
{
public:
    QGroupBox *gbIdentification;
    QLabel *lblName;
    QLineEdit *ledName;
    QLabel *lblGender;
    QRadioButton *rbGenderMale;
    QRadioButton *rbGenderFemale;
    QLabel *lblBirthDate;
    QDateEdit *dateEdit;
    QLabel *lblUniqueId;
    QLineEdit *ledUniqueId;
    QCheckBox *checkIndependent;
    QPushButton *pbFinishLater;
    QGroupBox *gbAddress;
    QLabel *lblCountry;
    QLineEdit *ledCountry;
    QLabel *lblRegion;
    QLineEdit *ledRegion;
    QLabel *lblCity;
    QLineEdit *ledCity;
    QLabel *lblZip;
    QLineEdit *ledZip;
    QLabel *lblAddress;
    QLineEdit *ledAddress;
    QGroupBox *gbAdditionalData;
    QLabel *lblPhone;
    QLineEdit *ledPhone;
    QLabel *lblEmail;
    QLineEdit *ledEmail;
    QLabel *lblComment;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pbSave;
    QPushButton *pbCancel;
    QGroupBox *gbVisit;
    QLabel *lblPatientOrigin;
    QComboBox *cmbPatientOrigin;
    QLabel *lblReasonToVisit;
    QComboBox *cmbReasonToVisit;
    QPushButton *pbAttendances;

    void setupUi(QDialog *dlgPatientEdit)
    {
        if (dlgPatientEdit->objectName().isEmpty())
            dlgPatientEdit->setObjectName(QString::fromUtf8("dlgPatientEdit"));
        dlgPatientEdit->resize(728, 503);
        gbIdentification = new QGroupBox(dlgPatientEdit);
        gbIdentification->setObjectName(QString::fromUtf8("gbIdentification"));
        gbIdentification->setGeometry(QRect(10, 10, 550, 80));
        lblName = new QLabel(gbIdentification);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(10, 20, 40, 20));
        ledName = new QLineEdit(gbIdentification);
        ledName->setObjectName(QString::fromUtf8("ledName"));
        ledName->setGeometry(QRect(90, 20, 250, 20));
        lblGender = new QLabel(gbIdentification);
        lblGender->setObjectName(QString::fromUtf8("lblGender"));
        lblGender->setGeometry(QRect(350, 20, 46, 20));
        rbGenderMale = new QRadioButton(gbIdentification);
        rbGenderMale->setObjectName(QString::fromUtf8("rbGenderMale"));
        rbGenderMale->setGeometry(QRect(400, 20, 60, 20));
        rbGenderFemale = new QRadioButton(gbIdentification);
        rbGenderFemale->setObjectName(QString::fromUtf8("rbGenderFemale"));
        rbGenderFemale->setGeometry(QRect(470, 20, 60, 20));
        lblBirthDate = new QLabel(gbIdentification);
        lblBirthDate->setObjectName(QString::fromUtf8("lblBirthDate"));
        lblBirthDate->setGeometry(QRect(10, 50, 70, 20));
        dateEdit = new QDateEdit(gbIdentification);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setGeometry(QRect(90, 50, 110, 22));
        lblUniqueId = new QLabel(gbIdentification);
        lblUniqueId->setObjectName(QString::fromUtf8("lblUniqueId"));
        lblUniqueId->setGeometry(QRect(210, 50, 110, 20));
        ledUniqueId = new QLineEdit(gbIdentification);
        ledUniqueId->setObjectName(QString::fromUtf8("ledUniqueId"));
        ledUniqueId->setGeometry(QRect(320, 50, 220, 20));
        checkIndependent = new QCheckBox(dlgPatientEdit);
        checkIndependent->setObjectName(QString::fromUtf8("checkIndependent"));
        checkIndependent->setGeometry(QRect(10, 476, 190, 20));
        pbFinishLater = new QPushButton(dlgPatientEdit);
        pbFinishLater->setObjectName(QString::fromUtf8("pbFinishLater"));
        pbFinishLater->setGeometry(QRect(580, 110, 130, 31));
        gbAddress = new QGroupBox(dlgPatientEdit);
        gbAddress->setObjectName(QString::fromUtf8("gbAddress"));
        gbAddress->setGeometry(QRect(10, 100, 550, 110));
        lblCountry = new QLabel(gbAddress);
        lblCountry->setObjectName(QString::fromUtf8("lblCountry"));
        lblCountry->setGeometry(QRect(10, 20, 50, 20));
        ledCountry = new QLineEdit(gbAddress);
        ledCountry->setObjectName(QString::fromUtf8("ledCountry"));
        ledCountry->setGeometry(QRect(90, 20, 190, 20));
        lblRegion = new QLabel(gbAddress);
        lblRegion->setObjectName(QString::fromUtf8("lblRegion"));
        lblRegion->setGeometry(QRect(290, 20, 50, 20));
        ledRegion = new QLineEdit(gbAddress);
        ledRegion->setObjectName(QString::fromUtf8("ledRegion"));
        ledRegion->setGeometry(QRect(350, 20, 190, 20));
        lblCity = new QLabel(gbAddress);
        lblCity->setObjectName(QString::fromUtf8("lblCity"));
        lblCity->setGeometry(QRect(10, 50, 46, 20));
        ledCity = new QLineEdit(gbAddress);
        ledCity->setObjectName(QString::fromUtf8("ledCity"));
        ledCity->setGeometry(QRect(90, 50, 190, 20));
        lblZip = new QLabel(gbAddress);
        lblZip->setObjectName(QString::fromUtf8("lblZip"));
        lblZip->setGeometry(QRect(290, 50, 46, 20));
        ledZip = new QLineEdit(gbAddress);
        ledZip->setObjectName(QString::fromUtf8("ledZip"));
        ledZip->setGeometry(QRect(350, 50, 60, 20));
        lblAddress = new QLabel(gbAddress);
        lblAddress->setObjectName(QString::fromUtf8("lblAddress"));
        lblAddress->setGeometry(QRect(10, 80, 70, 20));
        ledAddress = new QLineEdit(gbAddress);
        ledAddress->setObjectName(QString::fromUtf8("ledAddress"));
        ledAddress->setGeometry(QRect(90, 80, 450, 20));
        gbAdditionalData = new QGroupBox(dlgPatientEdit);
        gbAdditionalData->setObjectName(QString::fromUtf8("gbAdditionalData"));
        gbAdditionalData->setGeometry(QRect(10, 310, 550, 160));
        lblPhone = new QLabel(gbAdditionalData);
        lblPhone->setObjectName(QString::fromUtf8("lblPhone"));
        lblPhone->setGeometry(QRect(10, 20, 80, 20));
        ledPhone = new QLineEdit(gbAdditionalData);
        ledPhone->setObjectName(QString::fromUtf8("ledPhone"));
        ledPhone->setGeometry(QRect(90, 20, 130, 20));
        lblEmail = new QLabel(gbAdditionalData);
        lblEmail->setObjectName(QString::fromUtf8("lblEmail"));
        lblEmail->setGeometry(QRect(230, 20, 80, 20));
        ledEmail = new QLineEdit(gbAdditionalData);
        ledEmail->setObjectName(QString::fromUtf8("ledEmail"));
        ledEmail->setGeometry(QRect(310, 20, 230, 20));
        lblComment = new QLabel(gbAdditionalData);
        lblComment->setObjectName(QString::fromUtf8("lblComment"));
        lblComment->setGeometry(QRect(10, 50, 60, 20));
        plainTextEdit = new QPlainTextEdit(gbAdditionalData);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(90, 50, 450, 100));
        pbSave = new QPushButton(dlgPatientEdit);
        pbSave->setObjectName(QString::fromUtf8("pbSave"));
        pbSave->setGeometry(QRect(580, 20, 130, 30));
        pbCancel = new QPushButton(dlgPatientEdit);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));
        pbCancel->setGeometry(QRect(580, 54, 130, 30));
        gbVisit = new QGroupBox(dlgPatientEdit);
        gbVisit->setObjectName(QString::fromUtf8("gbVisit"));
        gbVisit->setGeometry(QRect(10, 220, 550, 80));
        lblPatientOrigin = new QLabel(gbVisit);
        lblPatientOrigin->setObjectName(QString::fromUtf8("lblPatientOrigin"));
        lblPatientOrigin->setGeometry(QRect(10, 20, 150, 20));
        cmbPatientOrigin = new QComboBox(gbVisit);
        cmbPatientOrigin->setObjectName(QString::fromUtf8("cmbPatientOrigin"));
        cmbPatientOrigin->setGeometry(QRect(170, 20, 220, 22));
        lblReasonToVisit = new QLabel(gbVisit);
        lblReasonToVisit->setObjectName(QString::fromUtf8("lblReasonToVisit"));
        lblReasonToVisit->setGeometry(QRect(10, 50, 110, 20));
        cmbReasonToVisit = new QComboBox(gbVisit);
        cmbReasonToVisit->setObjectName(QString::fromUtf8("cmbReasonToVisit"));
        cmbReasonToVisit->setGeometry(QRect(170, 50, 220, 22));
        pbAttendances = new QPushButton(dlgPatientEdit);
        pbAttendances->setObjectName(QString::fromUtf8("pbAttendances"));
        pbAttendances->setGeometry(QRect(580, 170, 130, 30));

        retranslateUi(dlgPatientEdit);

        QMetaObject::connectSlotsByName(dlgPatientEdit);
    } // setupUi

    void retranslateUi(QDialog *dlgPatientEdit)
    {
        dlgPatientEdit->setWindowTitle(QApplication::translate("dlgPatientEdit", "Dialog", 0, QApplication::UnicodeUTF8));
        gbIdentification->setTitle(QApplication::translate("dlgPatientEdit", " Identification ", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("dlgPatientEdit", "Name :", 0, QApplication::UnicodeUTF8));
        lblGender->setText(QApplication::translate("dlgPatientEdit", "Gender :", 0, QApplication::UnicodeUTF8));
        rbGenderMale->setText(QApplication::translate("dlgPatientEdit", "Male", 0, QApplication::UnicodeUTF8));
        rbGenderFemale->setText(QApplication::translate("dlgPatientEdit", "Female", 0, QApplication::UnicodeUTF8));
        lblBirthDate->setText(QApplication::translate("dlgPatientEdit", "Date of birth :", 0, QApplication::UnicodeUTF8));
        lblUniqueId->setText(QApplication::translate("dlgPatientEdit", "Unique identification :", 0, QApplication::UnicodeUTF8));
        checkIndependent->setText(QApplication::translate("dlgPatientEdit", "Independent from the local studio.", 0, QApplication::UnicodeUTF8));
        pbFinishLater->setText(QApplication::translate("dlgPatientEdit", "Finish later ...", 0, QApplication::UnicodeUTF8));
        gbAddress->setTitle(QApplication::translate("dlgPatientEdit", " Address ", 0, QApplication::UnicodeUTF8));
        lblCountry->setText(QApplication::translate("dlgPatientEdit", "Country :", 0, QApplication::UnicodeUTF8));
        lblRegion->setText(QApplication::translate("dlgPatientEdit", "Region :", 0, QApplication::UnicodeUTF8));
        lblCity->setText(QApplication::translate("dlgPatientEdit", "City: ", 0, QApplication::UnicodeUTF8));
        lblZip->setText(QApplication::translate("dlgPatientEdit", "Zip :", 0, QApplication::UnicodeUTF8));
        lblAddress->setText(QApplication::translate("dlgPatientEdit", "Address :", 0, QApplication::UnicodeUTF8));
        gbAdditionalData->setTitle(QApplication::translate("dlgPatientEdit", " Additional data ", 0, QApplication::UnicodeUTF8));
        lblPhone->setText(QApplication::translate("dlgPatientEdit", "Phone number :", 0, QApplication::UnicodeUTF8));
        lblEmail->setText(QApplication::translate("dlgPatientEdit", "E-Mail address :", 0, QApplication::UnicodeUTF8));
        lblComment->setText(QApplication::translate("dlgPatientEdit", "Comment :", 0, QApplication::UnicodeUTF8));
        pbSave->setText(QApplication::translate("dlgPatientEdit", "Save", 0, QApplication::UnicodeUTF8));
        pbCancel->setText(QApplication::translate("dlgPatientEdit", "Cancel", 0, QApplication::UnicodeUTF8));
        gbVisit->setTitle(QApplication::translate("dlgPatientEdit", " Statistical data ", 0, QApplication::UnicodeUTF8));
        lblPatientOrigin->setText(QApplication::translate("dlgPatientEdit", "Place the patient comes from :", 0, QApplication::UnicodeUTF8));
        lblReasonToVisit->setText(QApplication::translate("dlgPatientEdit", "Reason of visit :", 0, QApplication::UnicodeUTF8));
        pbAttendances->setText(QApplication::translate("dlgPatientEdit", "Attendances ...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgPatientEdit: public Ui_dlgPatientEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPATIENTEDIT_H
