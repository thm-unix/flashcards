#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QFileDialog>

#include "choosestack.h"
#include "stackviewer.h"
#include "export.h"
#include "editor.h"
#include "settings.h"
#include "about.h"
#include "ui_choosestack.h"

QJsonObject config;
stackviewer* viewer;

void choosestack::readConfig() {
    QFile file;
    QString data;

    file.setFileName("config.json");
    file.open(QFile::ReadWrite | QIODevice::Text);
    data = file.readAll();
    file.close();

    config = QJsonDocument::fromJson(data.toUtf8()).object();
}

void choosestack::showViewer(QListWidgetItem* stackItem) {
    QString stackPath = stackItem->text();

    viewer = new stackviewer(nullptr, this, stackPath, config["fontFamily"].toString());
    viewer->show();
    viewer->activateWindow();

    this->hide();
}

void choosestack::listStacks(QListWidget* stacksListWidget) {
    QJsonArray stacksList = config["stacks"].toArray();
    for (int i = 0; i < stacksList.count(); ++i) {
        if (QFile::exists(stacksList.at(i).toString())) {
            stacksListWidget->addItem(stacksList.at(i).toString());
        }
    }
}

void choosestack::exportStack(QListWidgetItem* stackItem) {
    Export* e = new Export(stackItem->text(), config["fontFamily"].toString());
    delete e;
}

void choosestack::editStack(QListWidgetItem* stackItem) {
    QString stackPath = stackItem->text();

    Editor* ed = new Editor(nullptr, this, stackPath);
    ed->show();
    ed->activateWindow();

    this->hide();
}

void choosestack::settings() {
    Settings* s = new Settings(nullptr, this, config);
    s->show();
    s->activateWindow();

    this->hide();
}

void choosestack::makeConnections(Ui::choosestack* ui) {
    this->connect(ui->learnPushButton, &QPushButton::clicked, this, [this, ui]() {
        if (!ui->stacksListWidget->selectedItems().empty()) {
            showViewer(ui->stacksListWidget->selectedItems()[0]);
        }
    });

    this->connect(ui->exportPushButton, &QPushButton::clicked, this, [this, ui]() {
        if (!ui->stacksListWidget->selectedItems().empty()) {
            exportStack(ui->stacksListWidget->selectedItems()[0]);
        }
    });

    this->connect(ui->editPushButton, &QPushButton::clicked, this, [this, ui]() {
        if (!ui->stacksListWidget->selectedItems().empty()) {
            editStack(ui->stacksListWidget->selectedItems()[0]);
        }
    });

    this->connect(ui->settingsPushButton, &QPushButton::clicked, this, [this]() {
        settings();
    });

    this->connect(ui->newPushButton, &QPushButton::clicked, this, [this, ui]() {
        QString stackPath = QFileDialog::getSaveFileName(this, "New stack", getenv("HOME"), "Stacks (.json)");
        if (!stackPath.isEmpty()) {
            if (!stackPath.endsWith(".json")) {
                stackPath += ".json";
            }

            QFile file(stackPath);
            file.open(QFile::WriteOnly);
            file.write("{ \"count\": 0 }");
            file.close();

            QJsonArray stacks = config["stacks"].toArray();
            stacks.append(QJsonValue(stackPath));
            config["stacks"] = QJsonValue(stacks);

            QJsonDocument doc(config);
            QFile jsonFile("config.json");
            jsonFile.open(QFile::WriteOnly);
            jsonFile.write(doc.toJson(QJsonDocument::Indented));
            jsonFile.close();

            editStack(new QListWidgetItem(stackPath));

            readConfig();
            ui->stacksListWidget->clear();
            listStacks(ui->stacksListWidget);
        }
    });

    this->connect(ui->openPushButton, &QPushButton::clicked, this, [this, ui]() {
        QString stackPath = QFileDialog::getOpenFileName(this, "Open stack", getenv("HOME"), "Stacks (*.json)");

        if (!stackPath.isEmpty()) {
            QJsonArray stacks = config["stacks"].toArray();
            stacks.append(QJsonValue(stackPath));
            config["stacks"] = QJsonValue(stacks);

            QJsonDocument doc(config);
            QFile jsonFile("config.json");
            jsonFile.open(QFile::WriteOnly);
            jsonFile.write(doc.toJson(QJsonDocument::Indented));
            jsonFile.close();

            showViewer(new QListWidgetItem(stackPath));

            readConfig();
            ui->stacksListWidget->clear();
            listStacks(ui->stacksListWidget);
        }
    });

    this->connect(ui->aboutPushButton, &QPushButton::clicked, this, []() {
        About* a = new About;
        a->show();
    });
}

choosestack::choosestack(QWidget *parent) : QWidget(parent), ui(new Ui::choosestack) {
    ui->setupUi(this);
    readConfig();
    listStacks(ui->stacksListWidget);
    makeConnections(ui);
}

choosestack::~choosestack() {
    delete ui;
    delete viewer;
}

