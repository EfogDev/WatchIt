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
    w.serialList = &serialList;
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *cWidget = new QWidget();
    cWidget->setLayout(layout);
    QVBoxLayout *leftPanel = new QVBoxLayout();
    QHBoxLayout *buttonsPanel = new QHBoxLayout();

    QListWidget *lw_Episodes = new QListWidget();
    lw_Episodes->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Episodes->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Episodes->setStyleSheet(listWidgetStyle);
    lw_Episodes->setVisible(false);
    lw_Episodes->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QListWidget *lw_Seasons = new QListWidget();
    lw_Seasons->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Seasons->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Seasons->setStyleSheet(listWidgetStyle);
    lw_Seasons->setVisible(false);

    QObject::connect(lw_Seasons, SIGNAL(itemClicked(QListWidgetItem*)), &w, SLOT(lwSeasonsClicked(QListWidgetItem*)));

    QListWidget *lw_Main = new QListWidget();
    lw_Main->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Main->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Main->setStyleSheet(listWidgetStyle);
    lw_Episodes->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QObject::connect(lw_Main, SIGNAL(itemClicked(QListWidgetItem*)), &w, SLOT(lwMainClicked(QListWidgetItem*)));

    QPushButton *pb_New = new QPushButton();
    pb_New->setText("Добавить");
    pb_New->setFixedHeight(BUTTON_HEIGHT);

    QObject::connect(pb_New, SIGNAL(clicked()), &w, SLOT(pbNewClicked()));

    QPushButton *pb_Remove = new QPushButton();
    pb_Remove->setText("Удалить");
    pb_Remove->setFixedHeight(BUTTON_HEIGHT);

    QObject::connect(pb_Remove, SIGNAL(clicked()), &w, SLOT(pbRemoveClicked()));

    QPushButton *pb_Back = new QPushButton();
    pb_Back->setText("Вернуться");
    pb_Back->setFixedHeight(BUTTON_HEIGHT);
    pb_Back->setVisible(false);

    QObject::connect(pb_Back, SIGNAL(clicked()), &w, SLOT(pbBackClicked()));

    QWebView *browser = new QWebView();

    w.gui["layout"] = layout;
    w.gui["leftPanel"] = leftPanel;
    w.gui["buttonsPanel"] = buttonsPanel;
    w.gui["lw_Episodes"] = lw_Episodes;
    w.gui["lw_Seasons"] = lw_Seasons;
    w.gui["lw_Main"] = lw_Main;
    w.gui["pb_New"] = pb_New;
    w.gui["pb_Remove"] = pb_Remove;
    w.gui["pb_Back"] = pb_Back;
    w.gui["browser"] = browser;

    leftPanel->addWidget(lw_Main);
    leftPanel->addWidget(lw_Seasons);
    leftPanel->addWidget(lw_Episodes);
    buttonsPanel->addWidget(pb_New);
    buttonsPanel->addWidget(pb_Remove);
    buttonsPanel->addWidget(pb_Back);
    leftPanel->addLayout(buttonsPanel);
    layout->addLayout(leftPanel);
    layout->addWidget(browser);
    w.setCentralWidget(cWidget);
    w.setMinimumSize(500, 120);
    w.show();

    serialList.add("http://adultmult.tv/html/simpsons_start.html");
    lw_Main->addItem("Симпсоны");

    serialList.add("http://adultmult.tv/other/gravity_falls.html");
    lw_Main->addItem("Gravity Falls");

    serialList.add("http://adultmult.tv/as/good_vibes.html");
    lw_Main->addItem("На одной волне");

    return a.exec();
}
