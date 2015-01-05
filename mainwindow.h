#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serial.h>
#include <QtCore>

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        QMap<QString, QObject*> gui;
        SerialList *serialList;
        Serial *selectedSerial;
        Season *selectedSeason;
        Episode *selectedEpisode;
        ~MainWindow();

    public slots:
        void lwMainClicked(QListWidgetItem *wdg);
        void lwSeasonsClicked(QListWidgetItem *wdg);
        void lwEpisodesClicked(QListWidgetItem *wdg);
        void pbBackClicked();
        void pbNewClicked();
        void pbRemoveClicked();
        void pbAboutClicked();
};

#endif // MAINWINDOW_H
