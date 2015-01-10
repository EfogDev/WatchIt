/********************************************************************************
** Form generated from reading UI file 'prefswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFSWINDOW_H
#define UI_PREFSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrefsWindow
{
public:
    QComboBox *comboBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QCheckBox *checkBox;
    QLabel *label;

    void setupUi(QWidget *PrefsWindow)
    {
        if (PrefsWindow->objectName().isEmpty())
            PrefsWindow->setObjectName(QStringLiteral("PrefsWindow"));
        PrefsWindow->resize(400, 125);
        comboBox = new QComboBox(PrefsWindow);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(20, 40, 361, 22));
        pushButton = new QPushButton(PrefsWindow);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(310, 90, 75, 23));
        pushButton_2 = new QPushButton(PrefsWindow);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(230, 90, 75, 23));
        checkBox = new QCheckBox(PrefsWindow);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 93, 131, 17));
        label = new QLabel(PrefsWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 141, 16));

        retranslateUi(PrefsWindow);

        QMetaObject::connectSlotsByName(PrefsWindow);
    } // setupUi

    void retranslateUi(QWidget *PrefsWindow)
    {
        PrefsWindow->setWindowTitle(QApplication::translate("PrefsWindow", "Form", 0));
        pushButton->setText(QApplication::translate("PrefsWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        pushButton_2->setText(QApplication::translate("PrefsWindow", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
        checkBox->setText(QApplication::translate("PrefsWindow", "\320\237\320\276\320\262\320\265\321\200\321\205 \320\262\321\201\320\265\321\205 \320\276\320\272\320\276\320\275", 0));
        label->setText(QApplication::translate("PrefsWindow", "\320\232\320\260\321\207\320\265\321\201\321\202\320\262\320\276 \320\264\320\273\321\217 \321\201\320\272\320\260\321\207\320\270\320\262\320\260\320\275\320\270\321\217:", 0));
    } // retranslateUi

};

namespace Ui {
    class PrefsWindow: public Ui_PrefsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFSWINDOW_H
