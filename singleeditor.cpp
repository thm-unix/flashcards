#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "singleeditor.h"
#include "ui_singleeditor.h"

#include "stackviewer.h"

void SingleEditor::save(QJsonObject stack, QString stackPath, int i, QString front, QString back, stackviewer* viewer) {
    QVariantHash flashcard;
    flashcard["front"] = front;
    flashcard["back"] = back;
    stack[QString::number(i)] = QJsonObject::fromVariantHash(flashcard);

    QJsonDocument doc(stack);
    QFile jsonFile(stackPath);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
    jsonFile.close();

    viewer->readStack(stackPath, i);
    viewer->showFlashcard(Side::Front, viewer->ui);

    this->hide();
}

SingleEditor::SingleEditor(QWidget *parent,
                           stackviewer* viewer,
                           QJsonObject stack,
                           QString stackPath,
                           int i) : QWidget(parent), ui(new Ui::SingleEditor) {
    ui->setupUi(this);

    ui->frontTextEdit->setText(stack[QString::number(i)].toObject()["front"].toString());
    ui->backTextEdit->setText(stack[QString::number(i)].toObject()["back"].toString());

    this->connect(ui->savePushButton, &QPushButton::clicked, this, [this, stack, stackPath, i, viewer]() {
        save(stack,
             stackPath,
             i,
             ui->frontTextEdit->toPlainText(),
             ui->backTextEdit->toPlainText(),
             viewer);
    });

    this->setWindowTitle("Editing flashcard: " + QString::number(i+1));
}

SingleEditor::~SingleEditor()
{
    delete ui;
}
