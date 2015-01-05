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
    out << serial.seasonList << serial.name << serial.url << serial.isSingle;
    return out;
}

QDataStream& operator>>(QDataStream &in, Serial &serial) {
    serial = Serial();
    in >> serial.seasonList >> serial.name >> serial.url >> serial.isSingle;
    return in;
}

void Serial::updateSeasons() {
    isUpdated = false;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    auto *manager = new QNetworkAccessManager();
    QUrl serial(url);
    QNetworkRequest request(serial);
    manager->get(request);
    QObject::connect(manager, &QNetworkAccessManager::finished, [=] (QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QString data = codec->toUnicode(reply->readAll());
            if (data.indexOf("Ошибка 404") != -1) {
                QMessageBox::information(0, "Ошибка", "Сериал не найден", QMessageBox::Ok);
                isUpdated = true;
                return;
            }
            Serial serial;
            serial.url = reply->url().toString();

            QRegExp name("<span class=\"name\">((.|[\r\n])*)</span>");
            name.setMinimal(true);
            if (name.indexIn(data) == -1)  {
                QMessageBox::information(0, "Ошибка", "Сериал не найден", QMessageBox::Ok);
                isUpdated = true;
                return;
            }
            serial.name = name.cap(0).split("<br>")[1].trimmed();

            QRegExp seasons("<a href=\"([a-zA-Z0-9_-]+)_([0-9]+)s.html\">");
            seasons.setMinimal(true);
            if (seasons.indexIn(data) == -1)  {
                QMessageBox::information(0, "Ошибка", "Не найдено ни одного сезона", QMessageBox::Ok);
                isUpdated = true;
                return;
            }
            int pos = 0;
            int i = 0;
            while ((pos = seasons.indexIn(data, pos)) != -1) {
                Season season;
                season.url = "http://adultmult.tv/" + serial.url.split('/')[3] + "/" + seasons.cap(0).split('"')[1];
                season.prefix = serial.url.split('/')[3];
                if (seasonList.size() >= i + 1 && seasonList[i].url == season.url)
                    serial.seasonList.append(seasonList[i]);
                else
                    serial.seasonList.append(season);
                pos += seasons.matchedLength();
                i++;
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

    QString episodeData = codec->toUnicode(seasonReply->readAll());
    if (episodeData.indexOf("Ошибка 404") != -1)  {
        QMessageBox::information(0, "Ошибка", "Ошибка в одном из сезонов.", QMessageBox::Ok);
        return;
    }

    prefix = url.split('/')[3];

    Season season;
    QRegExp episodes("<a href=\"([a-zA-Z0-9_-.]+)\"><span class=\"(episode|стиль33)\">(.+)</span>");
    episodes.setMinimal(true);
    int ePos = 0;
    int i = 0;
    while ((ePos = episodes.indexIn(episodeData, ePos)) != -1) {
        Episode episode;
        episode.name = episodes.cap(0).split(':')[1].split('<')[0].trimmed();
        episode.link = "http://adultmult.tv/" + prefix + "/" + episodes.cap(1);
        episode.watched = false;

        if (episodeList.size() >= i + 1) {
            episode.watched = episodeList[i].watched;
        }

        season.episodeList.append(episode);
        ePos += episodes.matchedLength();
        i++;
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
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString data = codec->toUnicode(reply->readAll());
    if (data.indexOf("Ошибка 404") != -1)  {
        QMessageBox::information(0, "Ошибка", "Ошибка серии.", QMessageBox::Ok);
        return;
    }

    Episode episode;
    QRegExp video("<iframe src=\"(.+)vk.com/(.+)\"");
    video.setMinimal(true);
    video.indexIn(data);

    QString frame = "http://vk.com/" + video.cap(2);


    auto *manager2 = new QNetworkAccessManager();
    QUrl url2(frame);
    QNetworkRequest request2(url2);
    QNetworkReply *reply2 = manager2->get(request2);

    QEventLoop loop2;
    QObject::connect(reply2, SIGNAL(finished()), &loop2, SLOT(quit()));
    loop2.exec();

    data = reply2->readAll();

    QRegExp url240rx("\"url240\":\"(.+)\"");
    QRegExp url360rx("\"url360\":\"(.+)\"");
    QRegExp url480rx("\"url480\":\"(.+)\"");
    QRegExp url720rx("\"url720\":\"(.+)\"");

    url240rx.setMinimal(true);
    url360rx.setMinimal(true);
    url480rx.setMinimal(true);
    url720rx.setMinimal(true);

    url240rx.indexIn(data);
    url360rx.indexIn(data);
    url480rx.indexIn(data);
    url720rx.indexIn(data);

    url240 = url240rx.cap(1).replace("\\", "");
    url360 = url360rx.cap(1).replace("\\", "");
    url480 = url480rx.cap(1).replace("\\", "");
    url720 = frame;//url720rx.cap(1).replace("\\", "");

    isUpdated = true;
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





