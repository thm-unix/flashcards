#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QJsonObject>
#include <QFontComboBox>

#include "choosestack.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    void setCurrentSettings(QJsonObject config, QFontComboBox* fontComboBox);
    void saveSettings(QJsonObject config, QFontComboBox* fontComboBox);
    explicit Settings(QWidget *parent = nullptr,
                      choosestack* chooseStackWidget = nullptr,
                      QJsonObject config = QJsonObject());
    ~Settings();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
