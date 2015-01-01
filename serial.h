#ifndef SERIAL_H
#define SERIAL_H
#include <QtCore>

class Episode {
    public:
        Episode();
        QString name;
        QString url360;
        QString url480;
        QString url720;
        bool watched;
        ~Episode();
};

class Season {
    public:
        Season();
        int size();
        Episode operator[](int index);
        QString url;
        ~Season();

    private:
        Episode list[200]; //да, быдлокод, но иначе я заебусь
        int count;
};

class Serial {
    public:
        Serial();
        int size();
        Season operator[](int index);
        QString name;
        QString url;
        ~Serial();

    private:
        Season list[20]; //читай выше
        int count;
};

#endif // SERIAL_H
