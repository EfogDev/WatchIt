#include "const.h"
#include "mainwindow.h"
#include "serial.h"
#include <QApplication>
#include <QtWidgets>
#include <QtCore>
#include <QWebView>
#include <fstream>
#include <iostream>
#include <QNetworkDiskCache>

QString listWidgetStyle;
QString browserHtml;
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

    QFile html(":/web/html");
    html.open(QIODevice::ReadOnly);
    browserHtml = html.readAll();
    html.close();

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

    QObject::connect(lw_Episodes, SIGNAL(itemClicked(QListWidgetItem*)), &w, SLOT(lwEpisodesClicked(QListWidgetItem*)));

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
    lw_Main->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    QPushButton *pb_About = new QPushButton();
    pb_About->setText("О программе");
    pb_About->setFixedHeight(BUTTON_HEIGHT);

    QObject::connect(pb_About, SIGNAL(clicked()), &w, SLOT(pbAboutClicked()));

    QWebView *browser = new QWebView();
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    browser->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    browser->setHtml(browserHtml);

    QNetworkAccessManager* manager = new QNetworkAccessManager();
    QNetworkDiskCache* diskCache = new QNetworkDiskCache();
    QString location = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    diskCache->setCacheDirectory(location);
    manager->setCache(diskCache);
    browser->page()->setNetworkAccessManager(manager);

    w.gui["layout"] = layout;
    w.gui["leftPanel"] = leftPanel;
    w.gui["buttonsPanel"] = buttonsPanel;
    w.gui["lw_Episodes"] = lw_Episodes;
    w.gui["lw_Seasons"] = lw_Seasons;
    w.gui["lw_Main"] = lw_Main;
    w.gui["pb_New"] = pb_New;
    w.gui["pb_Remove"] = pb_Remove;
    w.gui["pb_Back"] = pb_Back;
    w.gui["pb_About"] = pb_About;
    w.gui["browser"] = browser;

    leftPanel->addWidget(lw_Main);
    leftPanel->addWidget(lw_Seasons);
    leftPanel->addWidget(lw_Episodes);
    buttonsPanel->addWidget(pb_New);
    buttonsPanel->addWidget(pb_Remove);
    buttonsPanel->addWidget(pb_Back);
    leftPanel->addLayout(buttonsPanel);
    leftPanel->addWidget(pb_About);
    layout->addLayout(leftPanel);
    layout->addWidget(browser);
    w.setCentralWidget(cWidget);
    w.setMinimumSize(500, 120);
    w.setFixedSize(900, 376);
    w.setWindowTitle("AdultMult");
    w.show();

    for (Serial serial: serialList.vector) {
        lw_Main->addItem(serial.name);
    }

    return a.exec();
}
