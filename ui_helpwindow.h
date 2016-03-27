/********************************************************************************
** Form generated from reading UI file 'helpwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPWINDOW_H
#define UI_HELPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(383, 226);
        Dialog->setMinimumSize(QSize(383, 226));
        Dialog->setMaximumSize(QSize(383, 300));
        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(270, 190, 101, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 20, 331, 41));
        QFont font;
        font.setPointSize(26);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 60, 311, 41));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 200, 161, 16));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(40, 110, 291, 16));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 130, 331, 16));

        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", 0));
        label->setText(QApplication::translate("Dialog", "WatchIt 1.02", 0));
        label_2->setText(QApplication::translate("Dialog", "<html><head/><body><p>\320\232\320\273\320\270\320\265\320\275\321\202 \320\264\320\273\321\217 \320\277\321\200\320\276\321\201\320\274\320\276\321\202\321\200\320\260 \321\201\320\265\321\200\320\270\320\260\320\273\320\276\320\262 \321\201 \321\201\320\260\320\271\321\202\320\260 zfilm-hd.net.</p></body></html>", 0));
        label_3->setText(QApplication::translate("Dialog", "\302\251 \320\220\321\200\321\202\321\221\320\274 \320\241\321\203\320\277\321\200\321\203\320\275 (Efog)", 0));
        label_4->setText(QApplication::translate("Dialog", "\320\241\320\262\321\217\320\267\321\214: efog@yandex.ru", 0));
        label_5->setText(QApplication::translate("Dialog", "\320\232\321\200\320\270\321\202\320\270\320\272\320\260, \320\277\321\200\320\265\320\264\320\273\320\276\320\266\320\265\320\275\320\270\321\217 \320\270 \320\277\320\276\320\266\320\265\320\273\320\260\320\275\320\270\321\217 \320\277\321\200\320\270\320\275\320\270\320\274\320\260\321\216\321\202\321\201\321\217.", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPWINDOW_H
