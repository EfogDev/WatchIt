#include "const.h"
#include "mainwindow.h"
#include "serial.h"
#include <QApplication>
#include <QtWidgets>
#include <QtCore>

SerialList serialList;

void init() {
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);

    QDir appDir(APPDIR);
    if (!appDir.exists()) appDir.mkdir(APPDIR);

    QFile serials(APPDIR + "serials.dat");
    serials.open(QIODevice::ReadWrite);
    serials.close();

    serialList.load(APPDIR + "serials.dat");
}

int main(int argc, char *argv[]) {
    init();
    QApplication a(argc, argv);
    MainWindow w(serialList);
    w.show();

    return a.exec();
}
