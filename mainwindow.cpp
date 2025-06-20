#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minesweeper.h"
#include "minesweeper_lib.h"
#include <algorithm>
#include <QMessageBox>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->beginButton, SIGNAL(clicked()), this, SLOT(beginButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::beginButton()
{
    xdim = (size_t)ui->spinBox_Col->value();
    ydim = (size_t)ui->spinBox_Row->value();
    unsigned int mine = (unsigned int)ui->spinBox_Mine->value();

    if (std::min(xdim, ydim) <= 0 || mine <= 0 || mine > xdim * ydim || std::max(xdim, ydim) > 25) {
        QMessageBox::information(this, "Error Box", "Please enter valid value(s)");
        return;
    }

    if (board) cleanBoard(board);

    startGame(board, xdim, ydim, mine);
    ui->stackedWidget->setCurrentWidget(ui->gamePage);

    for (int row = {}; row < ydim; ++row) {
        for (int col {}; col < xdim; ++col) {
            QPushButton* btn = new QPushButton();
            btn->setFixedSize(30, 30);
            ui->gridLayout_4->addWidget(btn, row, col);
            buttonGrid[col][row] = btn;

            btn->setProperty("col", col);
            btn->setProperty("row", row);
            btn->installEventFilter(this);
            cellReveal(col, row);
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPushButton* btn = qobject_cast<QPushButton*>(obj);

        if (btn) {
            int x = btn->property("col").toInt();
            int y = btn->property("row").toInt();

            if (mouseEvent->button() == Qt::LeftButton) {
                cellFlag(x, y);
            } else if (mouseEvent->button() == Qt::RightButton) {
                cellReveal(x, y);
            }

            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::cellFlag(int x, int y)
{
    QPushButton* btn = buttonGrid[x][y];

    QString current = btn->text();
    if (current == "🚩") btn->setText("");
    else {
        btn->setText("🚩");
        btn->setStyleSheet("background-color: grey;");
    }

    mark(board, xdim, ydim, x, y);

}

void MainWindow::cellReveal(int x, int y)
{
    QPushButton* btn = buttonGrid[x][y];
    int result = reveal(board, xdim, ydim, x, y);
    int value = board[y * xdim + x] & valueMask();

    // if (result == 1 || result == 2) return;


    QString colour = "";

    switch (value) {
        case 0:
            btn->setText("");
            colour = "302151";
            break;
        case 1:
            btn->setText("1");
            colour = "5C2247";
            break;
        case 2:
            btn->setText("2");
            colour = "89233E";
            break;
        case 3:
            btn->setText("3");
            colour = "B52434";
            break;
        case 4:
            btn->setText("4");
            colour = "B52434";
            break;
        case 5:
            btn->setText("5");
            colour = "CC2530";
            break;
        case 6:
            btn->setText("6");
            colour = "E2252B";
            break;
        case 7:
            btn->setText("7");
            colour = "F82626";
            break;
        case 8:
            btn->setText("8");
            colour = "E80409";
            break;
        default:
            btn->setText("💣");
            colour = "FF0000";
            QMessageBox::information(this, "You lost!", "You clicked a mine!");
            break;
        }

        btn->setStyleSheet("background-color: #" + colour + "; color: black; font-weight: bold;");

    if (isGameWon(board, xdim, ydim)) {
        QMessageBox::information(this, "Victory", "You revealed all safe cells!");
    }
}
