#ifndef CHOOSESTACK_H
#define CHOOSESTACK_H

#include <QWidget>
#include <QListWidgetItem>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class choosestack; }
QT_END_NAMESPACE

class choosestack : public QWidget {
    Q_OBJECT

public:
    void readConfig();
    void listStacks(QListWidget* stacksListWidget);
    void showViewer(QListWidgetItem* stackItem);
    void exportStack(QListWidgetItem* stackItem);
    void editStack(QListWidgetItem* stackItem);
    void settings();
    void makeConnections(Ui::choosestack* ui);
    choosestack(QWidget *parent = nullptr);
    ~choosestack();

private:
    Ui::choosestack *ui;
};
#endif // CHOOSESTACK_H
