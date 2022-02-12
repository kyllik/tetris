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
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QList>
#include <QTransform>
#include <QPointF>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 0; // x coordinate
    int top_margin = 0; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.

    // Timer connected to falling method making falling continous motion.
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::falling);

    scene_->setBackgroundBrush(Qt::black);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    // Moving tetris piece to right.
    if(event->key() == Qt::Key_D) {
        if(!is_allowed_to_move(STEP, 0)) {
            return;
        }
        move_by(STEP, 0);
    }
    // Moving tetris piece to left.
    if(event->key() == Qt::Key_A) {
        if(!is_allowed_to_move(-STEP, 0)) {
            return;
        }
        move_by(-STEP,0);
    }
    // Rotating tetris piece clockwise.
    if(event->key() == Qt::Key_W) {
        rotate_falling_piece(90);
    }
    // Rotating tetris piece counterclockwise.
    if(event->key() == Qt::Key_Q) {
        rotate_falling_piece(-90);
    }
    // Hard dropping tetris piece.
    if(event->key() == Qt::Key_X) {
        for(int i=0; i<23; i++)
            falling();
    }
    // Soft dropping tetris piece.dd
    if(event->key() == Qt::Key_S) {
        falling();
    }
}

void MainWindow::falling()
{

    if(!is_allowed_to_move(0, STEP)){
        // Not allowed to fall.
        timer_.stop();
        for (auto tile : falling_piece_tiles_) {
            fallen_tiles_.append(tile);
        }
        falling_piece_tiles_.clear();
        ui->spawn_button->setDisabled(false);
        return;
    }
    // Allowed to fall.
    move_by(0, STEP);
}

void MainWindow::spawn()
{
    // Randomly choosed tetromino piece number
    int tetromino = distr(randomEng);

    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    // Tiles for tetromino piece with chosen color.
    tile1_ = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen,
                             colors_.at(tetromino));
    tile2_ = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen,
                             colors_.at(tetromino));
    tile3_ = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen,
                             colors_.at(tetromino));
    tile4_ = scene_->addRect(0, 0, SQUARE_SIDE, SQUARE_SIDE, blackPen,
                             colors_.at(tetromino));

    // Positions for tiles.
    tile1_->setPos((tetrominos.at(tetromino)).at(0));
    tile2_->setPos((tetrominos.at(tetromino)).at(1));
    tile3_->setPos((tetrominos.at(tetromino)).at(2));
    tile4_->setPos((tetrominos.at(tetromino)).at(3));

    // Adding tiles to vector so those can be moved and rotated together.
    falling_piece_tiles_.append(tile1_);
    falling_piece_tiles_.append(tile2_);
    falling_piece_tiles_.append(tile3_);
    falling_piece_tiles_.append(tile4_);

    // If there's no room for new piece, game is over.
    if(!is_allowed_to_move(0,STEP)) {
        ui->textBrowser->setText("Game over!");
        ui->spawn_button->setEnabled(false);
        foreach(QGraphicsItem* tile, falling_piece_tiles_) {
            delete tile;
        }
        falling_piece_tiles_.clear();
    }
}

void MainWindow::move_by(int x, int y)
{
    // Moves every tile of falling piece.
    for(auto tile : falling_piece_tiles_) {
        tile->moveBy(x, y);
    }
}

bool MainWindow::is_allowed_to_move(int x, int y)
{
    QRectF rect = scene_->sceneRect();
    for(auto tile : falling_piece_tiles_) {
        if(!rect.contains(tile->x()+x, tile->y()+y))
            // Not allowed to move.
            return false;
        for(auto fallen : fallen_tiles_) {
            if (fallen->pos()==QPointF(tile->x()+x, tile->y()+y))
                // Not allowed to move.
                return false;
        }
    }
    // Allowed to move.
    return true;
}

void MainWindow::rotate_falling_piece(int deg)
{
    // First rotates every tile separetely.
    foreach(QGraphicsItem* tile, falling_piece_tiles_) {
        tile->setRotation(tile->rotation()-deg);
    }

    // Then rotates every tile as a group to opposite direction.
    QGraphicsItemGroup* group = scene_->createItemGroup(falling_piece_tiles_);
    QPointF offset = tile3_->sceneBoundingRect().center();
    QTransform transform;
    transform.translate(offset.x(), offset.y());
    transform.rotate(deg);
    transform.translate(-offset.x(), -offset.y());
    group->setTransform(transform);
    scene_->destroyItemGroup(group);
    scene_->update();

    // If location is not allowed, piece will be rotated back.
    if(!is_allowed_to_move(0,0)) {
        QGraphicsItemGroup* group = scene_->createItemGroup(falling_piece_tiles_);
        QPointF offset = tile3_->sceneBoundingRect().center();
        QTransform transform;
        transform.translate(offset.x(), offset.y());
        transform.rotate(-deg);
        transform.translate(-offset.x(), -offset.y());
        group->setTransform(transform);
        scene_->destroyItemGroup(group);
        scene_->update();
        foreach(QGraphicsItem* tile, falling_piece_tiles_) {
            tile->setRotation(tile->rotation()+deg);
        }

    }
}

void MainWindow::on_spawn_button_clicked()
{
    // New tetromino spawns and timer starts.
    spawn();
    ui->spawn_button->setDisabled(true);
    timer_.start(SPEED);

}

void MainWindow::on_restart_button_clicked()
{
    // Removes tiles from gaming area.
    ui->spawn_button->setEnabled(true);
    ui->textBrowser->clear();
    foreach(QGraphicsItem* tile, fallen_tiles_) {
        delete tile;
    }
    foreach(QGraphicsItem* tile, falling_piece_tiles_) {
        delete tile;
    }
    fallen_tiles_.clear();
    falling_piece_tiles_.clear();
    timer_.stop();
}
