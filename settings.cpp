#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>

#include "settings.h"
#include "ui_settings.h"
#include "choosestack.h"

choosestack* mainWidget3;

void Settings::setCurrentSettings(QJsonObject config, QFontComboBox* fontComboBox) {
    QFont currentFont;
    currentFont.setFamily(config["fontFamily"].toString());
    fontComboBox->setCurrentFont(currentFont);
}

void Settings::saveSettings(QJsonObject config, QFontComboBox* fontComboBox) {
    config["fontFamily"] = QJsonValue(fontComboBox->currentText());
    QJsonDocument doc(config);
    QFile jsonFile("config.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
    jsonFile.close();

    QMessageBox::information(this, "Success", "Settings have been saved successfully.");
    mainWidget3->readConfig();
    this->hide();
    mainWidget3->show();
    mainWidget3->activateWindow();
}

Settings::Settings(QWidget *parent, choosestack* chooseStackWidget, QJsonObject config) : QWidget(parent), ui(new Ui::Settings) {
    ui->setupUi(this);

    mainWidget3 = chooseStackWidget;
    setCurrentSettings(config, ui->fontComboBox);

    // Make connections
    this->connect(ui->savePushButton, &QPushButton::clicked, this, [this, config]() {
        saveSettings(config, ui->fontComboBox);
    });
}

Settings::~Settings()
{
    delete ui;
}
