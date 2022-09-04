#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) : QWidget(parent), ui(new Ui::About) {
    ui->setupUi(this);
    this->connect(ui->closePushButton, &QPushButton::clicked, this, [this]() {
        this->hide();
    });
}

About::~About() {
    delete ui;
}
