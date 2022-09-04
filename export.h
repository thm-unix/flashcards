#ifndef EXPORT_H
#define EXPORT_H

#include <QWidget>
#include <QPrinter>
#include <QPainter>

enum FlashcardSide {
    FrontSide,
    BackSide
};

class Export : QWidget {
public:
    void readStack(QString stackPath);
    int getTextHeight(QString text, QFont font);
    int getMaximumFontSize(QString flashcard, int width, int height);
    QString getFlashcard(int i, FlashcardSide side);
    void makePDF(QString outputPath);
    Export(QString stackPath = "", QString fontFamily = "");
};

#endif // EXPORT_H
