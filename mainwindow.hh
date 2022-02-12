/* tetris
 *
 * Class: Mainwindow
 *
 * Program author
 * Name: Emma Sjöholm
 * Student number: 291683
 * UserID: kgemsj
 * E-Mail: emma.sjoholm@tuni.fi
 *
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QGraphicsRectItem>
#include <QTimer>
#include <vector>
#include <QList>
#include <QPointF>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void on_spawn_button_clicked();

    void on_restart_button_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    // Tiles of a falling piece.
    QGraphicsRectItem* tile1_;
    QGraphicsRectItem* tile2_;
    QGraphicsRectItem* tile3_;
    QGraphicsRectItem* tile4_;

    // List of tiles of falling piece.
    QList<QGraphicsItem*> falling_piece_tiles_;
    // List of all the fallen tiles.
    QList<QGraphicsItem*> fallen_tiles_;
    // Falling speed.
    int SPEED = 1000;
    // Colors of the pieces.
    std::vector<QBrush> colors_ = {Qt::red, Qt::blue, Qt::yellow, Qt::green,
                                   Qt::magenta, Qt::cyan, QColor("#FF9100")};
    QTimer timer_;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int STEP = 20;
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Coordinates to spawning point.
    const int A = 0;
    const int B = STEP;
    const int C = 2*STEP;
    const int D = 3*STEP;

    const std::vector<QList<QPointF>> tetrominos = { {QPointF(A,A), QPointF(B,A), QPointF(C,A), QPoint(D,A)},
                                                     {QPointF(A,A), QPointF(A,B), QPointF(B,B), QPoint(C,B)},
                                                     {QPointF(C,A), QPointF(C,B), QPointF(B,B), QPoint(A,B)},
                                                     {QPointF(A,A), QPointF(A,B), QPointF(B,A), QPoint(B,B)},
                                                     {QPointF(A,B), QPointF(B,A), QPointF(B,B), QPoint(C,A)},
                                                     {QPointF(A,B), QPointF(B,A), QPointF(B,B), QPoint(C,B)},
                                                     {QPointF(A,A), QPointF(B,A), QPointF(B,B), QPoint(C,B)}};

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // More constants, attibutes, and methods

    // Drops piece one step every time it is called.
    void falling();
    // Spawns piece.
    void spawn();
    // Moves piece by given coordinates.
    void move_by(int x, int y);
    // Tells, whether piece is allowed to move given coordinates or not.
    bool is_allowed_to_move(int x, int y);
    // Rotates falling piece by given degrees.
    void rotate_falling_piece(int deg);
};

#endif // MAINWINDOW_HH
