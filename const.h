#include <qglobal.h>
#ifndef CONST
#define CONST

#define VERSION "1.13"

#define WINDIR   QString(QDir::homePath() + "\\.adultmult\\")
#define LINUXDIR QString(QDir::homePath() + "/.adultmult/")

#ifdef Q_OS_WIN32
#define APPDIR WINDIR
#endif

#ifdef Q_OS_LINUX
#define APPDIR LINUXDIR
#endif

#define BUTTON_HEIGHT     35
#define LISTWIDGET_HEIGHT 240

#endif // CONST
