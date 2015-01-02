#ifndef SERIAL_H
#define SERIAL_H
#include <QtCore>
#include <QtWidgets>
#include <QVector>

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
};

class Serial {
    public:
        QVector<Season> seasonList;
        QString name;
        QString url;
};

class SerialList {
    public:
        QVector<Serial> vector;
        void save(QString filename);
        void load(QString filename);
        int add(QString url, QListWidget *list);

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
