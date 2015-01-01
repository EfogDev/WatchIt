#ifndef CONST
#define CONST

#define WINDIR   QString("%appdata%/adultmult/")
#define LINUXDIR QString(QDir::homePath() + "/.adultmult/")

#ifdef Q_OS_WIN32
#define APPDIR WINDIR
#endif

#ifdef Q_OS_LINUX
#define APPDIR LINUXDIR
#endif

#endif // CONST
