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

int SerialList::add(QString url, QListWidget *list) {
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    list->setEnabled(false);
    auto *manager = new QNetworkAccessManager();
    QUrl serial(url);
    QNetworkRequest request(serial);
    manager->get(request);
    QObject::connect(manager, &QNetworkAccessManager::finished, [=] (QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QString data = codec->toUnicode(reply->readAll());
            if (data.indexOf("Ошибка 404") != -1) return NoSuchSerial;
            Serial serial;
            serial.url = reply->url().toString();

            QRegExp name("<span class=\"name\">((.|[\r\n])*)</span>");
            name.setMinimal(true);
            if (name.indexIn(data) == -1) return NoSuchSerial;
            serial.name = name.cap(0);

            QRegExp seasons("<a href=\"([a-zA-Z0-9_-]+)_([0-9]+)s.html\">");
            seasons.setMinimal(true);
            if (seasons.indexIn(data) == -1) return NoSeasonsAvailable;
            int pos = 0;
            while ((pos = seasons.indexIn(data, pos)) != -1) {
                Season season;
                season.url = "http://adultmult.tv/" + serial.url.split('/')[3] + "/" + seasons.cap(0).split('"')[1];

                auto *episodeManager = new QNetworkAccessManager();
                QUrl seasonUrl(season.url);
                QNetworkRequest seasonRequest(seasonUrl);
                QNetworkReply *seasonReply = episodeManager->get(seasonRequest);
                QEventLoop loop;
                QObject::connect(seasonReply, SIGNAL(finished()), &loop, SLOT(quit()));
                loop.exec();
                QString episodeData = codec->toUnicode(seasonReply->readAll());
                if (episodeData.indexOf("Ошибка 404") != -1) return NoSeasonsAvailable;
                QRegExp episodes("<a href=\"([a-zA-Z0-9_-.]+)\"><span class=\"episode\">(.+)</span>");
                episodes.setMinimal(true);
                int ePos = 0;
                while ((ePos = episodes.indexIn(episodeData, ePos)) != -1) {
                    Episode episode;
                    episode.name = episodes.cap(0).split(':')[1].split('<')[0].trimmed();
                    episode.link = "http://adultmult.tv/" + serial.url.split('/')[3] + "/" + episodes.cap(1);
                    episode.watched = false;
                    qDebug() << episode.name << episode.link;

                    season.episodeList.append(episode);
                    ePos += episodes.matchedLength();
                }

                serial.seasonList.append(season);
                pos += seasons.matchedLength();
            }
            this->vector.append(serial);
            list->addItem(serial.name);
            list->setEnabled(true);
            return NoError;
        } else {
            list->setEnabled(true);
            return NetworkError;
        }
    });
    return 0;
}

//Serial

QDataStream& operator<<(QDataStream &out, const Serial &serial) {
    out << serial.seasonList << serial.name << serial.url;
    return out;
}

QDataStream& operator>>(QDataStream &in, Serial &serial) {
    serial = Serial();
    in >> serial.seasonList >> serial.name >> serial.url;
    return in;
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







