#include <serial.h>
#include <QtCore>
#include <QtNetwork>

//SerialList

void SerialList::save(QString filename) {
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QDataStream out(&f);
    out.setVersion(QDataStream::Qt_5_3);
    out << this->vector;
    f.close();
}

void SerialList::load(QString filename) {
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QDataStream in(&f);
    in.setVersion(QDataStream::Qt_5_3);
    in >> this->vector;
    f.close();
}

int SerialList::add(QString url) {
    Serial serial;
    serial.url = url;
    this->vector.push_back(serial);
    //serial.update();
    return 0;
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
                return;
            }
            Serial serial;
            serial.url = reply->url().toString();

            QRegExp name("<span class=\"name\">((.|[\r\n])*)</span>");
            name.setMinimal(true);
            if (name.indexIn(data) == -1)  {
                QMessageBox::information(0, "Ошибка", "Сериал не найден", QMessageBox::Ok);
                return;
            }
            serial.name = name.cap(0).split("<br>")[1].trimmed();

            QRegExp seasons("<a href=\"([a-zA-Z0-9_-]+)_([0-9]+)s.html\">");
            seasons.setMinimal(true);
            if (seasons.indexIn(data) == -1)  {
                QMessageBox::information(0, "Ошибка", "Не найдено ни одного сезона", QMessageBox::Ok);
                return;
            }
            int pos = 0;
            while ((pos = seasons.indexIn(data, pos)) != -1) {
                Season season;
                season.url = "http://adultmult.tv/" + serial.url.split('/')[3] + "/" + seasons.cap(0).split('"')[1];
                season.prefix = serial.url.split('/')[3];
                serial.seasonList.append(season);
                pos += seasons.matchedLength();
            }
            this->name = serial.name;
            this->seasonList = serial.seasonList;
        } else {
            QMessageBox::information(0, "Ошибка", "Проблема с интернетом. Повторите позже.", QMessageBox::Ok);
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

    Season season;
    QRegExp episodes("<a href=\"([a-zA-Z0-9_-.]+)\"><span class=\"(episode|стиль33)\">(.+)</span>");
    episodes.setMinimal(true);
    int ePos = 0;
    while ((ePos = episodes.indexIn(episodeData, ePos)) != -1) {
        Episode episode;
        episode.name = episodes.cap(0).split(':')[1].split('<')[0].trimmed();
        episode.link = "http://adultmult.tv/" + prefix + "/" + episodes.cap(1);
        episode.watched = false;

        if (episodeList.size() >= ePos + 1)
            episode.watched = episodeList[ePos].watched;

        season.episodeList.append(episode);
        ePos += episodes.matchedLength();
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







