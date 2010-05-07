/********************************************************************************
** Form generated from reading UI file 'dlgpatientoriginedit.ui'
**
** Created: Fri May 7 14:29:10 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGPATIENTORIGINEDIT_H
#define UI_DLGPATIENTORIGINEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_dlgPatientOriginEdit
{
public:
    QLabel *lblName;
    QLineEdit *ledName;
    QCheckBox *checkIndependent;
    QDialogButtonBox *btbButtons;

    void setupUi(QDialog *dlgPatientOriginEdit)
    {
        if (dlgPatientOriginEdit->objectName().isEmpty())
            dlgPatientOriginEdit->setObjectName(QString::fromUtf8("dlgPatientOriginEdit"));
        dlgPatientOriginEdit->resize(290, 101);
        lblName = new QLabel(dlgPatientOriginEdit);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(10, 10, 41, 20));
        ledName = new QLineEdit(dlgPatientOriginEdit);
        ledName->setObjectName(QString::fromUtf8("ledName"));
        ledName->setGeometry(QRect(50, 10, 230, 20));
        checkIndependent = new QCheckBox(dlgPatientOriginEdit);
        checkIndependent->setObjectName(QString::fromUtf8("checkIndependent"));
        checkIndependent->setGeometry(QRect(50, 40, 220, 20));
        btbButtons = new QDialogButtonBox(dlgPatientOriginEdit);
        btbButtons->setObjectName(QString::fromUtf8("btbButtons"));
        btbButtons->setGeometry(QRect(10, 70, 270, 23));
        btbButtons->setStandardButtons(QDialogButtonBox::NoButton);

        retranslateUi(dlgPatientOriginEdit);
        QObject::connect(btbButtons, SIGNAL(accepted()), dlgPatientOriginEdit, SLOT(accept()));
        QObject::connect(btbButtons, SIGNAL(rejected()), dlgPatientOriginEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(dlgPatientOriginEdit);
    } // setupUi

    void retranslateUi(QDialog *dlgPatientOriginEdit)
    {
        dlgPatientOriginEdit->setWindowTitle(QApplication::translate("dlgPatientOriginEdit", "Dialog", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("dlgPatientOriginEdit", "Name :", 0, QApplication::UnicodeUTF8));
        checkIndependent->setText(QApplication::translate("dlgPatientOriginEdit", "Independent from the local studio.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgPatientOriginEdit: public Ui_dlgPatientOriginEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGPATIENTORIGINEDIT_H
