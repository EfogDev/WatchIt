#include "mainwindow.h"
#include "const.h"
#include <QApplication>
#include <QtWidgets>
#include <QtCore>
#include <QtWebKitWidgets/QWebView>

QString listWidgetStyle;

void checkAndCreate() {
    QDir appDir(APPDIR);
    if (!appDir.exists()) appDir.mkdir(APPDIR);

    QFile serials(APPDIR + "/serials.dat");
    serials.open(QIODevice::ReadWrite);
    serials.close();

    QFile listWidgetStyleFile(":/style/ListWidget");
    listWidgetStyleFile.open(QIODevice::ReadOnly);
    listWidgetStyle = listWidgetStyleFile.readAll();
    listWidgetStyleFile.close();
}

int main(int argc, char *argv[]) {
    checkAndCreate();
    QApplication a(argc, argv);
    MainWindow w;
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *cWidget = new QWidget();
    cWidget->setLayout(layout);

    QVBoxLayout *leftPanel = new QVBoxLayout();
    QHBoxLayout *buttonsPanel = new QHBoxLayout();

    QListWidget *lw_Main = new QListWidget();
    lw_Main->setFixedWidth(200);
    lw_Main->setGeometry(0, 0, 200, 120);
    lw_Main->setStyleSheet(listWidgetStyle);
    lw_Main->addItem("Gravity Falls");
    lw_Main->addItem("Доктор Кто");
    lw_Main->addItem("Эврика");
    lw_Main->addItem("Агенты Щ.И.Т.");

    QPushButton *pb_New = new QPushButton();
    pb_New->setText("Добавить");
    pb_New->setFixedHeight(25);

    QPushButton *pb_Remove = new QPushButton();
    pb_Remove->setText("Удалить");
    pb_Remove->setFixedHeight(25);

    QPushButton *pb_Back = new QPushButton();
    pb_Back->setText("Назад к списку");
    pb_Back->setFixedHeight(25);
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
    return a.exec();
}
