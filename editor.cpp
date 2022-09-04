#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

#include "editor.h"
#include "ui_editor.h"


QWidget* mainWidget2;
QString editStackPath;
QJsonObject stackToEdit;
int countWidgets;
QList<FlashcardWidget> widgets;
QJsonObject newStack;

void Editor::readStack() {
    QFile file;
    QString data;

    file.setFileName(editStackPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    stackToEdit = QJsonDocument::fromJson(data.toUtf8()).object();
    countWidgets = stackToEdit["count"].toInt();

    widgets.clear();
    foreach (QString key, newStack.keys()) {
        newStack.remove(key);
    }
}

QWidget* Editor::createFlashcardWidget(int i) {
    QWidget* flashcardWidget = new QWidget;
    QHBoxLayout* flashcardLayout = new QHBoxLayout;

    flashcardWidget->setLayout(flashcardLayout);
    flashcardLayout->setContentsMargins(1, 1, 1, 1);

    QLabel* numberLabel = new QLabel(QString::number(i+1));
    flashcardLayout->addWidget(numberLabel);

    QTextEdit* frontTextEdit = new QTextEdit;
    frontTextEdit->setMinimumHeight(120);
    if (stackToEdit.contains(QString::number(i))) {
        frontTextEdit->setText(stackToEdit[QString::number(i)].toObject()["front"].toString());
    }
    flashcardLayout->addWidget(frontTextEdit);

    QTextEdit* backTextEdit = new QTextEdit;
    backTextEdit->setMinimumHeight(120);
    if (stackToEdit.contains(QString::number(i))) {
        backTextEdit->setText(stackToEdit[QString::number(i)].toObject()["back"].toString());
    }
    flashcardLayout->addWidget(backTextEdit);

    QPushButton* removePushButton = new QPushButton("X");
    removePushButton->setMaximumWidth(25);
    removePushButton->setMinimumHeight(120);
    flashcardLayout->addWidget(removePushButton);

    widgets.append({flashcardWidget, frontTextEdit, backTextEdit});

    this->connect(removePushButton, &QPushButton::clicked, this,
                  [i, flashcardWidget, flashcardLayout, numberLabel,
                  frontTextEdit, backTextEdit, removePushButton]() {
        widgets.removeAt(i);
        delete numberLabel;
        delete frontTextEdit;
        delete backTextEdit;
        delete removePushButton;
        delete flashcardLayout;
        delete flashcardWidget;

        --countWidgets;
    });

    return flashcardWidget;
}

void addFlashcardWidget(QVBoxLayout* scrollAreaLayout, QWidget* flashcardWidget) {
    scrollAreaLayout->addWidget(flashcardWidget);
}

QVBoxLayout* setupScrollArea(QScrollArea* scrollArea) {
    QWidget* scrollAreaWidget = new QWidget;
    QVBoxLayout* scrollAreaLayout = new QVBoxLayout;

    scrollArea->setWidget(scrollAreaWidget);
    scrollAreaWidget->setLayout(scrollAreaLayout);

    return scrollAreaLayout;
}

void Editor::saveStack() {
    int count = 0;
    for (int i = 0; i < widgets.count(); ++i) {
        QVariantHash flashcard;
        flashcard["front"] = widgets[i].frontTextEdit->toPlainText();
        flashcard["back"] = widgets[i].backTextEdit->toPlainText();
        QJsonObject flashcardObj = QJsonObject::fromVariantHash(flashcard);
        newStack[QString::number(i)] = flashcardObj;
        ++count;
    }
    newStack["count"] = count;

    QJsonDocument doc(newStack);
    QFile stackFile(editStackPath);
    stackFile.open(QFile::WriteOnly);
    stackFile.write(doc.toJson(QJsonDocument::Indented));
    stackFile.close();

    QMessageBox::information(this, "Success", "Stack was saved successfully.");
}

void Editor::prepareEditor(Ui::Editor* ui) {
    QVBoxLayout* scrollAreaLayout = setupScrollArea(ui->scrollArea);

    for (int i = 0; i < countWidgets; ++i) {
        QWidget* currentFlashcardWidget = createFlashcardWidget(i);
        addFlashcardWidget(scrollAreaLayout, currentFlashcardWidget);
    }

    // Make connections
    this->connect(ui->addPushButton, &QPushButton::clicked, this, [this, scrollAreaLayout]() {
        QWidget* newFlashcardWidget = createFlashcardWidget(countWidgets++);
        addFlashcardWidget(scrollAreaLayout, newFlashcardWidget);
    });

    this->connect(ui->savePushButton, &QPushButton::clicked, this, [this]() {
        saveStack();
    });
}

Editor::Editor(QWidget *parent, QWidget* chooseStackWidget, QString stackPath) : QWidget(parent), ui(new Ui::Editor) {
    ui->setupUi(this);
    editStackPath = stackPath;

    readStack();
    prepareEditor(ui);
    mainWidget2 = chooseStackWidget;

    this->setWindowTitle(stackPath);
}

void Editor::closeEvent(QCloseEvent* event) {
    event->ignore();
    this->hide();
    mainWidget2->show();
    mainWidget2->activateWindow();
}

Editor::~Editor() {
    delete ui;
}
