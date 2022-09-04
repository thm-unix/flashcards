#ifndef SINGLEEDITOR_H
#define SINGLEEDITOR_H

#include <QWidget>
#include <QJsonObject>
#include <QString>

#include "stackviewer.h"

namespace Ui {
class SingleEditor;
}

class SingleEditor : public QWidget
{
    Q_OBJECT

public:
    void save(QJsonObject stack,
              QString stackPath,
              int i,
              QString front,
              QString back,
              stackviewer* viewer);
    explicit SingleEditor(QWidget *parent = nullptr,
                          stackviewer* viewer = nullptr,
                          QJsonObject stack = QJsonObject(),
                          QString stackPath = "",
                          int i = 0);
    ~SingleEditor();

private:
    Ui::SingleEditor *ui;
};

#endif // SINGLEEDITOR_H
