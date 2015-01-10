#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent): QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);
    ui->label->setText("AdultMult 0.24");
}

HelpWindow::~HelpWindow() {
    delete ui;
}
