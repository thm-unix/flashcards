#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextEdit>

struct FlashcardWidget {
    QWidget* flashcardWidget;
    QTextEdit* frontTextEdit;
    QTextEdit* backTextEdit;
};

namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    QWidget* createFlashcardWidget(int i);
    void prepareEditor(Ui::Editor* ui);
    void readStack();
    void saveStack();
    explicit Editor(QWidget *parent = nullptr,
                    QWidget* chooseStackWidget = nullptr,
                    QString stackPath = "");
    ~Editor();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::Editor *ui;
};

#endif // EDITOR_H
