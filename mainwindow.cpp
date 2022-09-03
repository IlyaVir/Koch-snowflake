#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <cmath>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    keyLeft = new QShortcut(this);
    keyLeft -> setKey(Qt::Key_Left);
    connect(keyLeft, SIGNAL(activated()), this, SLOT(slotShortcutLeft()));

    keyRight = new QShortcut(this);
    keyRight -> setKey(Qt::Key_Right);
    connect(keyRight, SIGNAL(activated()), this, SLOT(slotShortcutRight()));

    keyDown = new QShortcut(this);
    keyDown -> setKey(Qt::Key_Down);
    connect(keyDown, SIGNAL(activated()), this, SLOT(slotShortcutDown()));

    keyUp = new QShortcut(this);
    keyUp -> setKey(Qt::Key_Up);
    connect(keyUp, SIGNAL(activated()), this, SLOT(slotShortcutUp()));
}

int sqr_len(const QPoint &a, const QPoint &b) {
    QPoint c = b - a;
    return c.x() * c.x() + c.y() + c.y();
}

double sqrt_len(const QPoint &a, const QPoint &b) {
    return sqrt(sqr_len(a, b));
}

std::vector <QPoint> MainWindow::Koch(const std::vector <QPoint> &coordinates) {
    std::vector <QPoint> new_coordinates;
    new_coordinates.reserve(coordinates.size() * 4);

    for (size_t i = 0; i < coordinates.size(); ++i) {
        auto left_coordinate = coordinates[i], right_coordinate = coordinates[(i + 1) % coordinates.size()];
        new_coordinates.push_back(left_coordinate);
        auto left_p = left_coordinate + (right_coordinate - left_coordinate) / 3,
             right_p = left_coordinate + (2 * (right_coordinate - left_coordinate)) / 3,
             middle_p = (left_p + right_p) / 2 - QPoint((left_coordinate.y() - right_coordinate.y()) / (2 * sqrt(3)), (right_coordinate.x() - left_coordinate.x()) / (2 * sqrt(3)));
        new_coordinates.push_back(left_p);
        new_coordinates.push_back(middle_p);
        new_coordinates.push_back(right_p);
        new_coordinates.push_back(right_coordinate);
    }

    return new_coordinates;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    may_paint = false;

    QPainter qp(this);
    QPen pen(Qt::black, 2, Qt::SolidLine);
    qp.setPen(pen);
    qp.setRenderHint(QPainter::Antialiasing);
    std::vector <QPoint> begin_triangle = start_figure;
    //std::vector <QPoint> begin_triangle = {{100, 220}, {700, 220}, {400, (int)(220 + 300 * sqrt(3))}};
    //std::vector <QPoint> begin_triangle = {{200, 600}, {200, 200}, {600, 200}, {600, 600}};

    if (reverse_koch) {
        std::reverse(begin_triangle.begin(), begin_triangle.end());
    }
    for (int i = 0; i < koch_depth; ++i) {
        begin_triangle = Koch(begin_triangle);
    }
    for (size_t i = 0; i < begin_triangle.size(); ++i) {
        qp.drawLine(begin_triangle[i], begin_triangle[(i + 1) % begin_triangle.size()]);
    }

    may_paint = true;
    //qp.drawLine(20, 20, 50, 50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotShortcutRight()
{
    if (may_paint) {
        if (koch_depth < 7) { // 7 is best
            ++koch_depth;
            repaint();
        } else {
            //QMessageBox::warning(this, "Лежачий полицейский", "Вы слишком далеко лезите. Я дальше не пущу!");
        }
        //QMessageBox::information(this, "xxx", QString::fromStdString(std::to_string(koch_depth)));
    }

}

void MainWindow::slotShortcutLeft()
{
    if (may_paint) {
        if (koch_depth > 0) {
            --koch_depth;
            repaint();
        } else {
            //QMessageBox::warning(this, "Лежачий полицейский", "Куда уж меньше?");
        }
        //QMessageBox::information(this, "xxx", QString::fromStdString(std::to_string(koch_depth)));
    }
}

void MainWindow::slotShortcutDown()
{
    if (start_figure.size() == 3) {
        start_figure = {{200, 600}, {200, 200}, {600, 200}, {600, 600}};
    } else {
        start_figure = {{100, 220}, {700, 220}, {400, (int)(220 + 300 * sqrt(3))}};
    }
    if (may_paint) {
        may_paint = false;
        repaint();
    }
}

void MainWindow::slotShortcutUp()
{
    reverse_koch = !reverse_koch;
    if (may_paint) {
        may_paint = false;
        repaint();
    }
}

