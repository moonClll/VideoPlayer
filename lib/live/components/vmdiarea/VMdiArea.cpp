// You may need to build the project (run Qt uic code generator) to get "ui_VMdiArea.h" resolved

#include "VMdiArea.h"
#include "ui_VMdiArea.h"


VMdiArea::VMdiArea(QWidget *parent) :
        QMdiArea(parent), ui(new Ui::VMdiArea) {
    ui->setupUi(this);
}

VMdiArea::~VMdiArea() {
    delete ui;
}
