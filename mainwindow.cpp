#include "const.h"
#include "mainwindow.h"
#include <QtWebKit>
#include <QtWebKitWidgets>

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

    Serial *current = &serialList->vector[wdg->listWidget()->row(wdg)];

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
    lw_Episodes->scroll(0, 0);

    Season *current = &selectedSerial->seasonList[wdg->listWidget()->row(wdg)];

    selectedSeason = current;

    current->updateEpisodes();
    current->waitForUpdated();

    int i = 0;
    for (Episode episode: current->episodeList) {
        i++;
        QListWidgetItem *item = new QListWidgetItem("Серия " + QString::number(i) + (episode.name.trimmed().isEmpty() ? "" : ": ") + episode.name);
        if (episode.watched)
            item->setTextColor(QColor(150, 150, 150));
        lw_Episodes->addItem(item);
    }

    lw_Seasons->setEnabled(true);
    pb_Back->setEnabled(true);
    lw_Seasons->setVisible(false);
    lw_Episodes->setVisible(true);
}

void MainWindow::lwEpisodesClicked(QListWidgetItem *wdg) {
    QWebView *browser = (QWebView*) gui["browser"];
    QListWidget *lw_Episodes = (QListWidget*) gui["lw_Episodes"];
    QPushButton *pb_Back = (QPushButton*) gui["pb_Back"];

    lw_Episodes->setEnabled(false);
    pb_Back->setEnabled(false);

    Episode *current = &selectedSeason->episodeList[wdg->listWidget()->row(wdg)];
    selectedEpisode = current;
    wdg->setTextColor(QColor(150, 150, 150));

    current->updateSources();
    current->waitForUpdated();

    browser->page()->mainFrame()->evaluateJavaScript("document.getElementById('video').src = '" + current->url720 + "';");

    current->watched = true;
    serialList->save(APPDIR + "/serials.dat");

    lw_Episodes->setEnabled(true);
    pb_Back->setEnabled(true);
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
    QListWidget *lw_Main = (QListWidget*) gui["lw_Main"];

    lw_Main->setEnabled(false);

    QInputDialog *dialog = new QInputDialog();
    bool accepted;
    QString link = dialog->getText(0, "Добавление сериала", "Ссылка на сериал/мульт:", QLineEdit::Normal, "", &accepted);
    if (accepted && !link.isEmpty()) {
        if (link.mid(0, 7) != "http://")
            link = "http://" + link;

        Serial *serial = serialList->add(link);
        serial->updateSeasons();
        serial->waitForUpdated();

        serialList->save(APPDIR + "/serials.dat");
        serialList->toList(lw_Main);

    }

    lw_Main->setEnabled(true);
}

void MainWindow::pbRemoveClicked() {
    QListWidget *lw_Main = (QListWidget*) gui["lw_Main"];

    lw_Main->setEnabled(false);

    QStringList list;
    int i = 0;
    for (Serial serial: serialList->vector) {
        i++;
        list.append(QString::number(i) + ". " + serial.name);
    }

    bool accepted;
    QInputDialog *dialog = new QInputDialog();
    QString item = dialog->getItem(0, "Удалить сериал", "Выберите сериал для удаления:", list, 0, false, &accepted);
    if (accepted && !item.isEmpty()) {
        int index = item.split(".")[0].toInt() - 1; //да-да, ужасный костыль, но перед тем, как написать ЭТО, я 2 часа пытался сделать нормально, так что никаких претензий
        delete lw_Main->takeItem(index);
        serialList->vector.remove(index);
        serialList->save(APPDIR + "/serirals.dat");
    }

    lw_Main->setEnabled(true);
}
