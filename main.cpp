#include "mainwindow.h"
#include "const.h"
#include "serial.h"
#include <QApplication>
#include <QtWidgets>
#include <QtCore>
#include <QtWebKitWidgets/QWebView>
#include <fstream>
#include <iostream>

QString listWidgetStyle;
SerialList serialList;

void init() {
    QDir appDir(APPDIR);
    if (!appDir.exists()) appDir.mkdir(APPDIR);

    QFile serials(APPDIR + "/serials.dat");
    serials.open(QIODevice::ReadWrite);
    serials.close();

    QFile listWidgetStyleFile(":/style/ListWidget");
    listWidgetStyleFile.open(QIODevice::ReadOnly);
    listWidgetStyle = listWidgetStyleFile.readAll();
    listWidgetStyleFile.close();

    serialList.load(APPDIR + "/serials.dat");
}

int main(int argc, char *argv[]) {
    init();
    QApplication a(argc, argv);
    MainWindow w;
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *cWidget = new QWidget();
    cWidget->setLayout(layout);
    QVBoxLayout *leftPanel = new QVBoxLayout();
    QHBoxLayout *buttonsPanel = new QHBoxLayout();

    QListWidget *lw_Main = new QListWidget();
    lw_Main->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Main->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Main->setStyleSheet(listWidgetStyle);

    QPushButton *pb_New = new QPushButton();
    pb_New->setText("Добавить");
    pb_New->setFixedHeight(BUTTON_HEIGHT);

    QPushButton *pb_Remove = new QPushButton();
    pb_Remove->setText("Удалить");
    pb_Remove->setFixedHeight(BUTTON_HEIGHT);

    QPushButton *pb_Back = new QPushButton();
    pb_Back->setText("Назад к списку");
    pb_Back->setFixedHeight(BUTTON_HEIGHT);
    pb_Back->setVisible(false);

    QWebView *browser = new QWebView();

    leftPanel->addWidget(lw_Main);
    buttonsPanel->addWidget(pb_New);
    buttonsPanel->addWidget(pb_Remove);
    buttonsPanel->addWidget(pb_Back);
    leftPanel->addLayout(buttonsPanel);
    layout->addLayout(leftPanel);
    layout->addWidget(browser);
    w.setCentralWidget(cWidget);
    w.setMinimumSize(500, 120);
    w.show();

    serialList.add("http://adultmult.tv/as/adventure_time.html", lw_Main);

    return a.exec();
}
