#include <serial.h>
#include <QtCore>
#include <QtNetwork>

//SerialList

void SerialList::save(QString filename) {
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QDataStream out(&f);
    out.setVersion(QDataStream::Qt_5_2);
    out << this->vector;
    f.close();
}

void SerialList::load(QString filename) {
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    in.setVersion(QDataStream::Qt_5_2);
    in >> this->vector;
    f.close();
}

void SerialList::toList(QListWidget *list) {
    list->clear();
    for (Serial serial: this->vector) {
        list->addItem(serial.name);
    }
}

Serial* SerialList::add(QString url) {
    Serial serial;
    serial.url = url;
    serial.indexInList = vector.size();
    this->vector.push_back(serial);
    return &vector[vector.size() - 1];
}

//Serial

QDataStream& operator<<(QDataStream &out, const Serial &serial) {
    out << serial.seasonList << serial.name << serial.url << serial.isSingle << serial.iframeUrl;
    return out;
}

QDataStream& operator>>(QDataStream &in, Serial &serial) {
    serial = Serial();
    in >> serial.seasonList >> serial.name >> serial.url >> serial.isSingle >> serial.iframeUrl;
    return in;
}

void Serial::updateSeasons() {
    isUpdated = false;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    auto *manager = new QNetworkAccessManager();
    QUrl serial(url);
    QString hostname(serial.host());

    if (hostname != "adultmult.tv" && hostname != "adultmult.ru" && hostname != "zfilm-hd.com" && hostname != "zhd.life" && hostname != "zfilm-hd.net") {
        QMessageBox::information(0, "Ошибка", "Клиент поддерживает только сайт zfilm-hd.com!", QMessageBox::Ok);
        return;
    }

    QNetworkRequest request(serial);
    manager->get(request);
    QObject::connect(manager, &QNetworkAccessManager::finished, [=] (QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            Serial serial;
            serial.url = reply->url().toString();

            QString data = reply->readAll();

            if (data.indexOf("обнаружена ошибка") != -1) {
                QMessageBox::information(qApp->activeModalWidget(), "Ошибка", "Сериал не найден (обнаружена ошибка).", QMessageBox::Ok);
                isUpdated = true;
                return;
            }

            QRegExp name("<span id=\"dle-speedbar\">(.+)</div>");
            name.setMinimal(true);
            if (name.indexIn(data) == -1)  {
                QMessageBox::information(qApp->activeModalWidget(), "Ошибка", "Сериал не найден (не нашел название).", QMessageBox::Ok);
                isUpdated = true;
                return;
            }

            serial.name = name.cap(1).split("&raquo;").last().trimmed().split('<')[0].trimmed();

            QFile f("html.html");
            f.open(QIODevice::ReadWrite);
            QDataStream ds(&f);
            ds << data;
            f.close();

            QRegExp iframeScript("showFilm\\('(.+)'\\)");
            iframeScript.setMinimal(true);
            if (iframeScript.indexIn(data) == -1)  {
                QMessageBox::information(0, "Ошибка", "Сериал не найден (отсутствует плеер zfilm-hd).", QMessageBox::Ok);
                isUpdated = true;
                return;
            }
            QString iframeSource = QByteArray::fromBase64(iframeScript.cap(0).toUtf8());

            QRegExp iframe("<iframe src=\"http://(.+)/(.+)/iframe");
            iframe.setMinimal(true);
            if (iframe.indexIn(iframeSource) == -1)  {
                QMessageBox::information(0, "Ошибка", "Сериал не найден (не могу найти плеер).", QMessageBox::Ok);
                isUpdated = true;
                return;
            }
            serial.iframeUrl = iframe.cap(0).split('"')[1];

            auto *seasonManager = new QNetworkAccessManager();
            QUrl frame(serial.iframeUrl);
            QNetworkRequest frameRequest(frame);
            auto *freply = seasonManager->get(frameRequest);

            QEventLoop loop;
            QObject::connect(freply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();

            if (freply->error() == QNetworkReply::NoError) {
                QString frameData = freply->readAll();
                QRegExp seasons("<select name=\"season\" id=\"season\"(.+)</select>");
                seasons.setMinimal(true);
                if (seasons.indexIn(frameData) == -1)  {
                    QMessageBox::information(0, "Ошибка", "Не найдено ни одного сезона!", QMessageBox::Ok);
                    isUpdated = true;
                    return;
                }

                QStringList seasonsList = seasons.cap(1).split("</option>");
                int seasonCount = seasonsList.size() - 1;
                for (int i = 0; i < seasonCount; i++) {
                    Season season;

                    QString seasonNameData = seasonsList[i];
                    QRegExp seasonIdRe(".*([0-9]+)");
                    seasonIdRe.indexIn(seasonNameData);
                    int seasonId = seasonIdRe.cap(1).toInt();

                    season.url = serial.iframeUrl + "?season=" + QString::number(seasonId);
                    if (seasonList.size() >= i + 1 && seasonList[i].url == season.url)
                        serial.seasonList.append(seasonList[i]);
                    else
                        serial.seasonList.append(season);
                }

                isUpdated = true;
            } else {
                QMessageBox::information(0, "Ошибка", "Проблема с интернетом. Повторите позже.", QMessageBox::Ok);
                isUpdated = true;
                return;
            }

            this->name = serial.name;
            this->seasonList = serial.seasonList;
        } else {
            QMessageBox::information(0, "Ошибка", "Проблема с интернетом. Повторите позже.", QMessageBox::Ok);
            isUpdated = true;
            return;
        }
        isUpdated = true;
    });
}

void Serial::waitForUpdated() {
    while (!isUpdated) {
        QTimer t;
        t.start(10);
        QEventLoop loop;
        QObject::connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.exec();
    }
}

//Season

QDataStream& operator<<(QDataStream &out, const Season &season) {
    out << season.episodeList << season.url;
    return out;
}

QDataStream& operator>>(QDataStream &in, Season &season) {
    season = Season();
    in >> season.episodeList >> season.url;
    return in;
}

void Season::updateEpisodes() {
    isUpdated = false;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    auto *episodeManager = new QNetworkAccessManager();
    QUrl seasonUrl(url);
    QNetworkRequest seasonRequest(seasonUrl);
    QNetworkReply *seasonReply = episodeManager->get(seasonRequest);

    QEventLoop loop;
    QObject::connect(seasonReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString hostname = seasonUrl.host();
    QString episodeData = seasonReply->readAll();
    if (episodeData.indexOf("обнаружена ошибка") != -1) {
        QMessageBox::information(0, "Ошибка", "Сериал не найден", QMessageBox::Ok);
        isUpdated = true;
        return;
    }

    Season season;
    QRegExp episodes("<select name=\"episode\" id=\"episode\"(.+)</select>");
    episodes.setMinimal(true);
    if (episodes.indexIn(episodeData) == -1)  {
        QMessageBox::information(0, "Ошибка", "Не найдено ни одного эпизода!", QMessageBox::Ok);
        isUpdated = true;
        return;
    }

    int episodeCount = episodes.cap(1).split("option").size() / 2;
    for (int i = 0; i < episodeCount; i++) {
        Episode episode;
        episode.link = url + "&episode=" + QString::number(i + 1);
        episode.flashPlayer = episode.link;
        episode.watched = false;
        episode.name = "";

        if (episodeList.size() >= i + 1) {
            episode.watched = episodeList[i].watched;
        }

        season.episodeList.append(episode);
    }

    episodeList = season.episodeList;
    isUpdated = true;
}

void Season::waitForUpdated() {
    while (!isUpdated) {
        QTimer t;
        t.start(10);
        QEventLoop loop;
        QObject::connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.exec();
    }
}

//Episode

QDataStream& operator<<(QDataStream &out, const Episode &episode) {
    out << episode.name << episode.url240 << episode.url360 << episode.url480 << episode.url720 << episode.link << episode.watched;
    return out;
}

QDataStream& operator>>(QDataStream &in, Episode &episode) {
    episode = Episode();
    in >> episode.name >> episode.url240 >> episode.url360 >> episode.url480 >> episode.url720 >> episode.link >> episode.watched;
    return in;
}

void Episode::updateSources() {
    isUpdated = false;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    auto *manager = new QNetworkAccessManager();
    QUrl url(link);
    isUpdated = true;
    //todo
}

void Episode::waitForUpdated() {
    while (!isUpdated) {
        QTimer t;
        t.start(10);
        QEventLoop loop;
        QObject::connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.exec();
    }
}





