#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minesweeper.h"
#include "minesweeper_lib.h"
#include <algorithm>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>

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

    startGame(board, xdim, ydim, mine);
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    ui->label_7->setText("💣 = " + QString::number(mine));
    ui->gridLayout_4->setSpacing(0);
    ui->gridLayout_4->setContentsMargins(0, 0, 0, 0);

    for (int row = {}; row < ydim; ++row) {
        for (int col {}; col < xdim; ++col) {
            QPushButton* btn = new QPushButton();
            btn->setFixedSize(30, 30);
            ui->gridLayout_4->addWidget(btn, row, col);
            buttonGrid[col][row] = btn;

            btn->setProperty("col", col);
            btn->setProperty("row", row);
            btn->installEventFilter(this);
            //cellReveal(col, row);
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

            if (mouseEvent->button() == Qt::RightButton) cellFlag(x, y);
            if (mouseEvent->button() == Qt::LeftButton) cellReveal(x, y);
            printBoard(board, xdim, ydim);
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::cellFlag(int x, int y)
{
    QPushButton* btn = buttonGrid[x][y];
    switch (mark(board, xdim, ydim, x, y)) {
        case 2:
            break;
        case 1:
            btn->setText("");
            btn->setStyleSheet("background-color: black;");
            break;
        default:
            btn->setText("🚩");
            btn->setStyleSheet("background-color: black;");
    }
}

void MainWindow::cellReveal(int x, int y)
{
    QPushButton* btn = buttonGrid[x][y];
    if (reveal(board, xdim, ydim, x, y) == 1) return;

    QString colour = "";

    for (size_t i{}; i < (xdim * ydim); i ++) {
        if ((board[i] & hiddenBit()) == 0) {
            btn = buttonGrid[i % xdim][i / xdim];
            switch (board[i] & valueMask()) {
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
                    colour = "FFFFFF";
                    btn->setStyleSheet("background-color: #" + colour + "; font-weight: bold;");
                    QMessageBox::information(this, "You lost!", "You clicked a mine!");
                    break;
            }
            btn->setStyleSheet("background-color: #" + colour + "; color: white; font-weight: bold;");
        }
    }





    if (isGameWon(board, xdim, ydim)) QMessageBox::information(this, "Victory", "You revealed all safe cells!");

}
