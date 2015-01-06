#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serial.h>
#include <QtCore>

class QWebView;
class HelpWindow;

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(SerialList &_serialList, QWidget *parent = 0);
        ~MainWindow();

    private:
        SerialList &serialList;
        Serial *selectedSerial;
        Season *selectedSeason;
        Episode *selectedEpisode;

    private:
        QWidget     *cWidget;
        QHBoxLayout *layout;
        QVBoxLayout *leftPanel;
        QHBoxLayout *buttonsPanel;
        QListWidget *lw_Episodes;
        QListWidget *lw_Seasons;
        QListWidget *lw_Main;
        QPushButton *pb_New;
        QPushButton *pb_Remove;
        QPushButton *pb_Back;
        QPushButton *pb_About;
        QWebView    *browser;

        QMenu       *m_menuAdd;
        QMenu       *m_menuAddDelete;

        HelpWindow  *m_helpWindow;

    public slots:
        void lwMainClicked(QListWidgetItem *wdg);
        void lwSeasonsClicked(QListWidgetItem *wdg);
        void lwEpisodesClicked(QListWidgetItem *wdg);
        void lwOnMenu(const QPoint &point);
        void onActionDelete();
        void pbBackClicked();
        void pbNewClicked();
        void pbRemoveClicked();
        void pbAboutClicked();
};

#endif // MAINWINDOW_H
