#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {

}

MainWindow::~MainWindow() {

}

void MainWindow::lwMainClicked(QListWidgetItem *wdg) {
    QListWidget *lw_Seasons = (QListWidget*) gui["lw_Seasons"];
    QListWidget *lw_Main = (QListWidget*) gui["lw_Main"];
    QPushButton *pb_New = (QPushButton*) gui["pb_New"];
    QPushButton *pb_Remove = (QPushButton*) gui["pb_Remove"];
    QPushButton *pb_Back = (QPushButton*) gui["pb_Back"];

    lw_Main->setEnabled(false);
    lw_Seasons->clear();

    Serial *current = &serialList->vector[wdg->listWidget()->row(wdg) + 1];

    selectedSerial = current;

    current->updateSeasons();
    current->waitForUpdated();

    for (int i = 0; i < current->seasonList.size(); i++)
        lw_Seasons->addItem("Сезон " + QString::number(i + 1));

    lw_Main->setEnabled(true);
    pb_New->setVisible(false);
    pb_Remove->setVisible(false);
    pb_Back->setVisible(true);
    lw_Main->setVisible(false);
    lw_Seasons->setVisible(true);
}

void MainWindow::lwSeasonsClicked(QListWidgetItem *wdg) {
    QListWidget *lw_Episodes = (QListWidget*) gui["lw_Episodes"];
    QListWidget *lw_Seasons = (QListWidget*) gui["lw_Seasons"];
    QPushButton *pb_Back = (QPushButton*) gui["pb_Back"];

    lw_Seasons->setEnabled(false);
    pb_Back->setEnabled(false);
    lw_Episodes->clear();

    Season *current = &selectedSerial->seasonList[wdg->listWidget()->row(wdg)];

    selectedSeason = current;

    current->updateEpisodes();
    current->waitForUpdated();

    int i = 0;
    for (Episode episode: current->episodeList) {
        i++;
        lw_Episodes->addItem("Серия " + QString::number(i) + (episode.name.trimmed().isEmpty() ? "" : ": ") + episode.name);
    }

    lw_Seasons->setEnabled(true);
    pb_Back->setEnabled(true);
    lw_Seasons->setVisible(false);
    lw_Episodes->setVisible(true);
}

void MainWindow::pbBackClicked() {
    QListWidget *lw_Episodes = (QListWidget*) gui["lw_Episodes"];
    QListWidget *lw_Seasons = (QListWidget*) gui["lw_Seasons"];
    QListWidget *lw_Main = (QListWidget*) gui["lw_Main"];
    QPushButton *pb_New = (QPushButton*) gui["pb_New"];
    QPushButton *pb_Remove = (QPushButton*) gui["pb_Remove"];
    QPushButton *pb_Back = (QPushButton*) gui["pb_Back"];

    if (lw_Episodes->isVisible()) {
        lw_Seasons->setVisible(true);
        lw_Episodes->setVisible(false);
    } else {
        pb_New->setVisible(true);
        pb_Remove->setVisible(true);
        pb_Back->setVisible(false);
        lw_Main->setVisible(true);
        lw_Seasons->setVisible(false);
        lw_Episodes->setVisible(false);
    }
}

void MainWindow::pbNewClicked() {

}

void MainWindow::pbRemoveClicked() {

}
