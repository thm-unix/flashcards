#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>

#include "export.h"

QString exportFontFamily;
QJsonObject stackToExport;
int countFlashcards;
int lastYPixel = 100;

void Export::readStack(QString stackPath) {
    QFile file;
    QString data;

    file.setFileName(stackPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    stackToExport = QJsonDocument::fromJson(data.toUtf8()).object();
    countFlashcards = stackToExport["count"].toInt();
    lastYPixel = 100;
}

int Export::getTextHeight(QString text, QFont font) {
    QFontMetrics fm(font);
    return text.split('\n').count() * ((float)fm.capHeight() * 2);
}

int Export::getMaximumFontSize(QString flashcard, int width, int height) {
    int maxSize = 64;
    QFont font;
    font.setFamily(exportFontFamily);
    font.setPointSize(64);
    QFontMetrics fm(font);

    // find max line
    QString maxLine;
    foreach (QString line, flashcard.split('\n')) {
        if (fm.horizontalAdvance(line) > fm.horizontalAdvance(maxLine)) {
            maxLine = line;
        }
    }

    while ((fm.horizontalAdvance(maxLine) >= width) ||
          (getTextHeight(flashcard, font) >= height)) {
        --maxSize;
        font.setPointSize(maxSize);
        fm = QFontMetrics(font);
    }

    return maxSize;
}

QString Export::getFlashcard(int i, FlashcardSide side) {
    if (side == FrontSide) {
        return stackToExport[QString::number(i)].toObject()["front"].toString();
    }
    else {
        return stackToExport[QString::number(i)].toObject()["back"].toString();
    }
}

void Export::makePDF(QString outputPath) {
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputPath);

    // A6 pixel size: 1748x1240
    printer.setPageSize(QPageSize(QSizeF(1311, 930), QPageSize::Point));

    QPainter painter(&printer);

    for (int i = 0; i < countFlashcards; ++i) {
        QString frontText = getFlashcard(i, FrontSide);
        int ptSize = getMaximumFontSize(frontText, 1668, 1040);
        painter.setFont(QFont(exportFontFamily, ptSize));

        QRect flashcardRect(40, 80, 1748 - 40 - 40, 1240 - 80 - 100);
        painter.setPen(Qt::white);
        painter.drawRect(flashcardRect);
        painter.setPen(Qt::black);

        painter.drawText(flashcardRect, Qt::AlignLeft | Qt::AlignVCenter, frontText);

        printer.newPage();

        QString backText = getFlashcard(i, BackSide);
        int ptSize2 = getMaximumFontSize(backText, 1668, 1040);
        painter.setFont(QFont(exportFontFamily, ptSize2));

        QRect flashcardRect2(40, 80, 1748 - 40 - 40, 1240 - 80 - 100);
        painter.setPen(Qt::white);
        painter.drawRect(flashcardRect2);
        painter.setPen(Qt::black);

        painter.drawText(flashcardRect2, Qt::AlignLeft | Qt::AlignVCenter, backText);
        printer.newPage();
    }

    painter.end();
}

Export::Export(QString stackPath, QString fontFamily) {
    exportFontFamily = fontFamily;
    readStack(stackPath);

    QString outputPath = QFileDialog::getSaveFileName(this, "Save PDF file", getenv("HOME"), "PDF (*.pdf)");
    if (!outputPath.isEmpty()) {
        if (!outputPath.endsWith(".pdf")) {
            outputPath += ".pdf";
        }
        makePDF(outputPath);
    }
}
