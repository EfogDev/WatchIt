#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
    class Dialog;
}

class MainWindow;
struct Settings;

class SettingsWindow: public QDialog {
    Q_OBJECT

    public:
        explicit SettingsWindow(QWidget *parent, MainWindow *w);
        ~SettingsWindow();

    private:
        Ui::Dialog *ui;
        Settings *settings;
        MainWindow *win;

    private slots:
        void onSaveClicked();
        void onCancelClicked();
};

#endif // SETTINGSWINDOW_H
