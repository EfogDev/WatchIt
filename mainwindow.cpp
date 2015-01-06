#include "const.h"
#include "mainwindow.h"
#include <QtWebKit>
#include <QtWebKitWidgets>
#include <helpwindow.h>

MainWindow::MainWindow(SerialList &_serialList, QWidget *parent) : QMainWindow(parent),
serialList(_serialList), m_helpWindow(0)
{
    QFile listWidgetStyleFile(":/style/ListWidget");
    listWidgetStyleFile.open(QIODevice::ReadOnly);
    QString listWidgetStyle = listWidgetStyleFile.readAll();
    listWidgetStyleFile.close();

    QFile html(":/web/html");
    html.open(QIODevice::ReadOnly);
    QString browserHtml = html.readAll();
    html.close();

    layout = new QHBoxLayout();
    cWidget = new QWidget();
    cWidget->setLayout(layout);
    leftPanel = new QVBoxLayout();
    buttonsPanel = new QHBoxLayout();

    lw_Episodes = new QListWidget();
    lw_Episodes->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Episodes->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Episodes->setStyleSheet(listWidgetStyle);
    lw_Episodes->setVisible(false);
    lw_Episodes->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(lw_Episodes, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(lwEpisodesClicked(QListWidgetItem*)));

    lw_Seasons = new QListWidget();
    lw_Seasons->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Seasons->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Seasons->setStyleSheet(listWidgetStyle);
    lw_Seasons->setVisible(false);

    connect(lw_Seasons, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(lwSeasonsClicked(QListWidgetItem*)));

    lw_Main = new QListWidget();
    lw_Main->setFixedWidth(LISTWIDGET_HEIGHT);
    lw_Main->setGeometry(0, 0, LISTWIDGET_HEIGHT, 0);
    lw_Main->setStyleSheet(listWidgetStyle);
    lw_Main->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lw_Main->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(lw_Main, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(lwMainClicked(QListWidgetItem*)));
    connect(lw_Main, SIGNAL(customContextMenuRequested(QPoint)), SLOT(lwOnMenu(QPoint)));

    pb_New = new QPushButton();
    pb_New->setText("Добавить");
    pb_New->setFixedHeight(BUTTON_HEIGHT);

    connect(pb_New, SIGNAL(clicked()), SLOT(pbNewClicked()));

    pb_Remove = new QPushButton();
    pb_Remove->setText("Удалить");
    pb_Remove->setFixedHeight(BUTTON_HEIGHT);

    connect(pb_Remove, SIGNAL(clicked()), SLOT(pbRemoveClicked()));

    pb_Back = new QPushButton();
    pb_Back->setText("Вернуться");
    pb_Back->setFixedHeight(BUTTON_HEIGHT);
    pb_Back->setVisible(false);

    connect(pb_Back, SIGNAL(clicked()), SLOT(pbBackClicked()));

    pb_About = new QPushButton();
    pb_About->setText("О программе");
    pb_About->setFixedHeight(BUTTON_HEIGHT);

    connect(pb_About, SIGNAL(clicked()), SLOT(pbAboutClicked()));

    browser = new QWebView();
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

    setCentralWidget(cWidget);
    setMinimumSize(500, 120);
    setFixedSize(900, 376);
    setWindowTitle("AdultMult");
    //w.setWindowIcon(QIcon("main.ico"));

    for (Serial &serial: serialList.vector)
    {
        lw_Main->addItem(serial.name);
    }
    lw_Main->setCurrentRow(0);

    QAction *actionNew = new QAction("Добавить", this);
    connect(actionNew, SIGNAL(triggered()), SLOT(pbNewClicked()));

    QAction *actionDel = new QAction("Удалить", this);
    connect(actionNew, SIGNAL(triggered()), SLOT(onActionDelete()));

    m_menuAddDelete = new QMenu(this);
    m_menuAddDelete->addAction(actionNew);
    m_menuAddDelete->addAction(actionDel);

    m_menuAdd = new QMenu(this);
    m_menuAdd->addAction(actionNew);
}

MainWindow::~MainWindow() {

}

void MainWindow::lwMainClicked(QListWidgetItem *wdg) {
    lw_Main->setEnabled(false);
    lw_Seasons->clear();

    Serial *current = &serialList.vector[wdg->listWidget()->row(wdg)];

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
    lw_Episodes->setEnabled(false);
    pb_Back->setEnabled(false);

    Episode *current = &selectedSeason->episodeList[wdg->listWidget()->row(wdg)];
    selectedEpisode = current;
    wdg->setTextColor(QColor(150, 150, 150));

    current->updateSources();
    current->waitForUpdated();

    browser->page()->mainFrame()->evaluateJavaScript("document.getElementById('video').src = '" + current->url720 + "';");

    current->watched = true;
    serialList.save(APPDIR + "serials.dat");

    lw_Episodes->setEnabled(true);
    pb_Back->setEnabled(true);
}

void MainWindow::lwOnMenu(const QPoint &point)
{
    if(lw_Main->itemAt(point))
        m_menuAddDelete->exec(lw_Main->mapToGlobal(point));
    else
        m_menuAdd->exec(lw_Main->mapToGlobal(point));
}

void MainWindow::onActionDelete()
{
    const QString title("Удалить сериал");

    lw_Main->setEnabled(false);

    int index = lw_Main->currentRow();
    if(index >= 0)
    {
        Serial &s = serialList.vector[index];
        QString msg = "Вы точно хотите удалить сериал \'" + s.name + "\'?";

        int ret = QMessageBox::question(this, title, msg, QMessageBox::Yes, QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            delete lw_Main->takeItem(index);
            serialList.vector.remove(index);
            serialList.save(APPDIR + "serials.dat");
        }
    }
    else
        QMessageBox::information(this, title, "Выберите сериал");
}

void MainWindow::pbBackClicked() {
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
    lw_Main->setEnabled(false);

    QInputDialog *dialog = new QInputDialog();
    bool accepted;
    QString link = dialog->getText(0, "Добавление сериала", "Ссылка на сериал/мульт:", QLineEdit::Normal, "", &accepted);
    if (accepted && !link.isEmpty()) {
        if (link.mid(0, 7) != "http://")
            link = "http://" + link;

        Serial *serial = serialList.add(link);
        serial->updateSeasons();
        serial->waitForUpdated();

        if (serial->name.isEmpty()) {
            //qDebug() << "Empty!" << serial->indexInList;
            serialList.vector.remove(serial->indexInList);
        } else {
            serialList.save(APPDIR + "serials.dat");
            serialList.toList(lw_Main);
        }
    }

    lw_Main->setEnabled(true);
}

void MainWindow::pbRemoveClicked() {

    QStringList list;
    int i = 0;
    for (Serial &serial: serialList.vector) {
        i++;
        list.append(QString::number(i) + ". " + serial.name);
    }

    bool accepted;
    QInputDialog *dialog = new QInputDialog();
    QString item = dialog->getItem(0, "Удалить сериал", "Выберите сериал для удаления:", list, 0, false, &accepted);
    if (accepted && !item.isEmpty()) {
        int index = list.indexOf(item);
        delete lw_Main->takeItem(index);
        serialList.vector.remove(index);
        serialList.save(APPDIR + "serials.dat");
    }

    lw_Main->setEnabled(true);

}

void MainWindow::pbAboutClicked() {
    if(m_helpWindow == 0)
        m_helpWindow = new HelpWindow(this);
    m_helpWindow->show();
}
