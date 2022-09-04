#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QPushButton>
#include <QMessageBox>
#include <QFontMetrics>
#include <QCloseEvent>

#include "stackviewer.h"
#include "singleeditor.h"
#include "ui_stackviewer.h"

QString viewerFontFamily;
QWidget* mainWidget1;
QJsonObject stack;
Side currentSide;
int currentNumber;
int count;

void stackviewer::readStack(QString stackPath, int prevNumber) {
    QFile file;
    QString data;

    file.setFileName(stackPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    stack = QJsonDocument::fromJson(data.toUtf8()).object();
    count = stack["count"].toInt();
    currentNumber = prevNumber;
}

void stackviewer::prepare(Ui::stackviewer* ui) {
    ui->progressLabel->setText("0/" + QString::number(count));
}

void stackviewer::setCurrentProgress(Ui::stackviewer* ui) {
    ui->progressLabel->setText(QString::number(currentNumber+1) + "/" + QString::number(count));
    ui->progressBar->setValue((int)((float)(currentNumber+1) / (float)count * 100));
}

int getTextHeight(QString text, QFont font) {
    QFontMetrics fm(font);
    return text.split('\n').count() * ((float)fm.capHeight() * 2);
}

int getMaximumFontSize(QLabel* label, QString flashcard) {
    int maxSize = 48;
    QFont font;
    font.setFamily(viewerFontFamily);
    font.setPointSize(48);
    QFontMetrics fm(font);

    // find max line
    QString maxLine;
    foreach (QString line, flashcard.split('\n')) {
        if (fm.horizontalAdvance(line) > fm.horizontalAdvance(maxLine)) {
            maxLine = line;
        }
    }

    qDebug() << label->size().height();

    while ((fm.horizontalAdvance(maxLine) >= label->size().width()) ||
          (getTextHeight(flashcard, font) >= label->size().height())) {
        --maxSize;
        font.setPointSize(maxSize);
        fm = QFontMetrics(font);
    }

    return maxSize;
}

QString stackviewer::getFlashcard(Side side) {
    if (side == Front) {
        return stack[QString::number(currentNumber)].toObject()["front"].toString();
    }
    else {
        return stack[QString::number(currentNumber)].toObject()["back"].toString();
    }
}

void stackviewer::showFlashcard(Side side, Ui::stackviewer* ui) {
    if (currentNumber < count) {
        QString flashcard = getFlashcard(side);
        currentSide = side;

        int ptSize = getMaximumFontSize(ui->flashcardLabel, flashcard);

        ui->flashcardLabel->setText(flashcard);
        ui->flashcardLabel->setFont(QFont(viewerFontFamily, ptSize));
        setCurrentProgress(ui);
    }
}

void stackviewer::flipFlashcard(Ui::stackviewer* ui) {
    Side side = (currentSide == Front) ? Back : Front;
    showFlashcard(side, ui);
}

void stackviewer::makeConnections(Ui::stackviewer* ui, QString stackPath) {
    this->connect(ui->prevPushButton, &QPushButton::clicked, this, [this, ui]() {
        if (currentNumber > 0) {
            --currentNumber;
            showFlashcard(Front, ui);
        }
        else {
            currentNumber = count - 1;
            showFlashcard(Front, ui);
        }
    });

    this->connect(ui->flipPushButton, &QPushButton::clicked, this, [this, ui]() {
        flipFlashcard(ui);
    });

    this->connect(ui->editPushButton, &QPushButton::clicked, this, [this, ui, stackPath]() {
        SingleEditor* se = new SingleEditor(nullptr, this, stack, stackPath, currentNumber);
        se->show();
        se->activateWindow();
    });

    this->connect(ui->nextPushButton, &QPushButton::clicked, this, [this, ui]() {
        if (currentNumber < (count-1)) {
            ++currentNumber;
            showFlashcard(Front, ui);
        }
        else {
            QMessageBox::information(this, "Stack completion", "You have finished this stack.");

            // TODO: Ask about forgotten flashcards
            this->hide();
            mainWidget1->show();
            mainWidget1->activateWindow();
        }
    });
}

stackviewer::stackviewer(QWidget *parent,
                         QWidget* chooseStackWidget,
                         QString path,
                         QString fontFamily) : QWidget(parent),
                                         ui(new Ui::stackviewer) {
    mainWidget1 = chooseStackWidget;
    viewerFontFamily = fontFamily;

    this->setWindowTitle(path);

    ui->setupUi(this);
    readStack(path, 0);
    prepare(ui);
    makeConnections(ui, path);
    showFlashcard(Front, ui);
}

void stackviewer::closeEvent(QCloseEvent* event) {
    event->ignore();
    this->hide();
    mainWidget1->show();
    mainWidget1->activateWindow();
    delete this;
}

stackviewer::~stackviewer() {
    delete ui;
}
