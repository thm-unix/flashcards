#ifndef STACKVIEWER_H
#define STACKVIEWER_H

#include <QWidget>
#include <QString>

enum Side {
    Front,
    Back
};

namespace Ui {
class stackviewer;
}

class stackviewer : public QWidget {
    Q_OBJECT

public:
    void readStack(QString path, int prevNumber);
    void prepare(Ui::stackviewer* ui);
    void setCurrentProgress(Ui::stackviewer* ui);
    QString getFlashcard(Side side);
    void showFlashcard(Side side, Ui::stackviewer* ui);
    void flipFlashcard(Ui::stackviewer* ui);
    void makeConnections(Ui::stackviewer* ui, QString stackPath);
    explicit stackviewer(QWidget *parent = nullptr,
                         QWidget* chooseStackWidget = nullptr,
                         QString path = "",
                         QString viewerFontFamily = "");
    ~stackviewer();

    Ui::stackviewer *ui;

protected:
    void closeEvent(QCloseEvent* event) override;

};

#endif // STACKVIEWER_H
