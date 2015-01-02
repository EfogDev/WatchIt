#ifndef SERIAL_H
#define SERIAL_H
#include <QtCore>
#include <QtWidgets>
#include <QVector>
#include <QObject>

class Episode {
    public:
        QString name;
        QString url240, url360, url480, url720, link;
        bool watched;
};

class Season {
    public:
        QVector<Episode> episodeList;
        QString url;
        QString prefix;
        bool isUpdated = false;
        void updateEpisodes();
        void waitForUpdated();
};

class Serial {
    public:
        QVector<Season> seasonList;
        QString name;
        QString url;
        bool isSingle = false;
        bool isUpdated = false;
        void updateSeasons();
        void waitForUpdated();
};

class SerialList {
    public:
        QVector<Serial> vector;
        void save(QString filename);
        void load(QString filename);
        int add(QString url);

        enum Error {
            NoSuchSerial,
            NoSeasonsAvailable,
            NoEpisodesAvailable,
            NoPlayerAvailable,
            NetworkError,
            NoError
        };
};

QDataStream& operator<<(QDataStream &out, const Serial &serial);
QDataStream& operator>>(QDataStream &in, Serial &serial);

QDataStream& operator<<(QDataStream &out, const Season &serial);
QDataStream& operator>>(QDataStream &in, Season &serial);

QDataStream& operator<<(QDataStream &out, const Episode &serial);
QDataStream& operator>>(QDataStream &in, Episode &serial);

#endif // SERIAL_H
