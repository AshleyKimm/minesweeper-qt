#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void beginButton();

private:
    Ui::MainWindow *ui;
    size_t xdim{0};
    size_t ydim{0};
    char *board {NULL};

    void cellReveal(int x, int y);
    void cellFlag(int x, int y);
    QPushButton* buttonGrid[25][25];

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};
#endif // MAINWINDOW_H
