/********************************************************************************
** Form generated from reading UI file 'dlgserialreg.ui'
**
** Created: Wed Jun 23 23:14:34 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGSERIALREG_H
#define UI_DLGSERIALREG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dlgSerialReg
{
public:
    QPushButton *pbCancel;
    QLabel *lblSerial;
    QPushButton *pbValidate;
    QLineEdit *ledSerial;

    void setupUi(QDialog *dlgSerialReg)
    {
        if (dlgSerialReg->objectName().isEmpty())
            dlgSerialReg->setObjectName(QString::fromUtf8("dlgSerialReg"));
        dlgSerialReg->resize(290, 100);
        pbCancel = new QPushButton(dlgSerialReg);
        pbCancel->setObjectName(QString::fromUtf8("pbCancel"));
        pbCancel->setGeometry(QRect(150, 60, 130, 30));
        pbCancel->setIconSize(QSize(20, 20));
        lblSerial = new QLabel(dlgSerialReg);
        lblSerial->setObjectName(QString::fromUtf8("lblSerial"));
        lblSerial->setGeometry(QRect(10, 10, 270, 20));
        pbValidate = new QPushButton(dlgSerialReg);
        pbValidate->setObjectName(QString::fromUtf8("pbValidate"));
        pbValidate->setGeometry(QRect(10, 60, 130, 30));
        pbValidate->setIconSize(QSize(20, 20));
        ledSerial = new QLineEdit(dlgSerialReg);
        ledSerial->setObjectName(QString::fromUtf8("ledSerial"));
        ledSerial->setGeometry(QRect(10, 30, 270, 20));
        QWidget::setTabOrder(ledSerial, pbValidate);
        QWidget::setTabOrder(pbValidate, pbCancel);

        retranslateUi(dlgSerialReg);

        QMetaObject::connectSlotsByName(dlgSerialReg);
    } // setupUi

    void retranslateUi(QDialog *dlgSerialReg)
    {
        dlgSerialReg->setWindowTitle(QApplication::translate("dlgSerialReg", "Dialog", 0, QApplication::UnicodeUTF8));
        pbCancel->setText(QApplication::translate("dlgSerialReg", "Cancel", 0, QApplication::UnicodeUTF8));
        lblSerial->setText(QApplication::translate("dlgSerialReg", "Serial key", 0, QApplication::UnicodeUTF8));
        pbValidate->setText(QApplication::translate("dlgSerialReg", "Validate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgSerialReg: public Ui_dlgSerialReg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGSERIALREG_H
