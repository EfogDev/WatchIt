#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent): QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    //ui->label->setText("WatchIt 1.02");
}

HelpWindow::~HelpWindow() {
    delete ui;
}
