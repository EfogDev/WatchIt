#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <serial.h>
#include <settingswindow.h>
#include <QtCore>

class QWebView;
class HelpWindow;

struct Settings {
    int quality = 3;
    bool onTop = false;


    void save(QString filename);
    void load(QString filename);
};

class MainWindow: public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(SerialList &_serialList, QWidget *parent = 0);
        Settings settings;
        ~MainWindow();

    private:
        SerialList &serialList;
        Serial *selectedSerial;
        Season *selectedSeason;
        Episode *selectedEpisode;
        bool firstLoad = true;

    private:
        QWidget     *cWidget;
        QHBoxLayout *layout;
        QVBoxLayout *leftPanel;
        QHBoxLayout *buttonsPanel;
        QHBoxLayout *bottomPanel;
        QListWidget *lw_Episodes;
        QListWidget *lw_Seasons;
        QListWidget *lw_Main;
        QPushButton *pb_New;
        QPushButton *pb_Remove;
        QPushButton *pb_Back;
        QPushButton *pb_About;
        QPushButton *pb_Settings;
        QPushButton *pb_Download;
        QWebView    *browser;
        QMenu       *m_menuAdd;
        QMenu       *m_menuAddDelete;
        QMenu       *m_Episode;
        HelpWindow  *m_helpWindow;

    public slots:
        void lwMainClicked(QListWidgetItem *wdg);
        void lwSeasonsClicked(QListWidgetItem *wdg);
        void lwEpisodesClicked(QListWidgetItem *wdg);
        void lwOnMenu(const QPoint &point);
        void lwEpisodesOnMenu(const QPoint &point);
        void onActionDelete();
        void episodeNotViewed();
        void pbBackClicked();
        void pbNewClicked();
        void pbRemoveClicked();
        void pbAboutClicked();
        void pbSettingsClicked();
        void pbDownloadClicked();
        void updateSettings();
        void browserLoaded();
};

QDataStream& operator<<(QDataStream &out, const Settings &settings);
QDataStream& operator>>(QDataStream &in, Settings &settings);

#endif // MAINWINDOW_H
