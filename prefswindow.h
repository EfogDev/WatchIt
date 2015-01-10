#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
#include <QDialog>
#include <mainwindow.h>

namespace Ui {
    class PrefsWindow;
}

class MainWindow;
struct Settings;

class PrefsWindow: public QDialog {
    Q_OBJECT

    public:
        explicit PrefsWindow(QWidget *parent, MainWindow *w);
        ~PrefsWindow();

    private:
        Ui::PrefsWindow *ui;
        Settings *settings;
        MainWindow *win;

    private slots:
        void onSaveClicked();
        void onCancelClicked();
};

#endif // SETTINGSWINDOW_H
