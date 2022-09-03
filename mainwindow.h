#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QShortcut *keyLeft, *keyRight, *keyDown, *keyUp;

private slots:
    void slotShortcutRight();
    void slotShortcutLeft();
    void slotShortcutDown();
    void slotShortcutUp();

protected:
    int koch_depth = 0;
    bool may_paint = true, reverse_koch = false;
    std::vector <QPoint> start_figure = {{100, 220}, {700, 220}, {400, (int)(220 + 300 * sqrt(3))}};

    void paintEvent(QPaintEvent *event);
    std::vector <QPoint>  Koch(const std::vector <QPoint> &coordinates);
};
#endif // MAINWINDOW_H
