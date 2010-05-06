/********************************************************************************
** Form generated from reading UI file 'dlgreasontovisitedit.ui'
**
** Created: Thu May 6 20:34:03 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGREASONTOVISITEDIT_H
#define UI_DLGREASONTOVISITEDIT_H

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

class Ui_dlgReasonToVisitEdit
{
public:
    QLabel *lblName;
    QLineEdit *ledName;
    QCheckBox *checkIndependent;
    QDialogButtonBox *btbButtons;

    void setupUi(QDialog *dlgReasonToVisitEdit)
    {
        if (dlgReasonToVisitEdit->objectName().isEmpty())
            dlgReasonToVisitEdit->setObjectName(QString::fromUtf8("dlgReasonToVisitEdit"));
        dlgReasonToVisitEdit->resize(290, 101);
        lblName = new QLabel(dlgReasonToVisitEdit);
        lblName->setObjectName(QString::fromUtf8("lblName"));
        lblName->setGeometry(QRect(10, 10, 41, 20));
        ledName = new QLineEdit(dlgReasonToVisitEdit);
        ledName->setObjectName(QString::fromUtf8("ledName"));
        ledName->setGeometry(QRect(50, 10, 230, 20));
        checkIndependent = new QCheckBox(dlgReasonToVisitEdit);
        checkIndependent->setObjectName(QString::fromUtf8("checkIndependent"));
        checkIndependent->setGeometry(QRect(50, 40, 220, 20));
        btbButtons = new QDialogButtonBox(dlgReasonToVisitEdit);
        btbButtons->setObjectName(QString::fromUtf8("btbButtons"));
        btbButtons->setGeometry(QRect(10, 70, 270, 23));
        btbButtons->setStandardButtons(QDialogButtonBox::NoButton);

        retranslateUi(dlgReasonToVisitEdit);
        QObject::connect(btbButtons, SIGNAL(accepted()), dlgReasonToVisitEdit, SLOT(accept()));
        QObject::connect(btbButtons, SIGNAL(rejected()), dlgReasonToVisitEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(dlgReasonToVisitEdit);
    } // setupUi

    void retranslateUi(QDialog *dlgReasonToVisitEdit)
    {
        dlgReasonToVisitEdit->setWindowTitle(QApplication::translate("dlgReasonToVisitEdit", "Dialog", 0, QApplication::UnicodeUTF8));
        lblName->setText(QApplication::translate("dlgReasonToVisitEdit", "Name :", 0, QApplication::UnicodeUTF8));
        checkIndependent->setText(QApplication::translate("dlgReasonToVisitEdit", "Independent from the local studio.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class dlgReasonToVisitEdit: public Ui_dlgReasonToVisitEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGREASONTOVISITEDIT_H
