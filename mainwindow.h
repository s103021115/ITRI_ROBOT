#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QPushButton>
#include <QDebug>
#include <typeinfo>
#include <memory>
#include "blockitem.h"

using std::vector;
using std::pair;
using std::unique_ptr;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  private slots:
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

public slots:
    void on_block_mouse_release(BlockItem*);
    void on_block_mouse_press(BlockItem*);
    void on_block_mouse_move(BlockItem*);

  private:
    Ui::MainWindow *ui;
    QString currentSelectedInstruction;
    QGraphicsScene *scene;
    vector<BlockItem*> blocks;
    vector<pair<BlockItem*, QRectF>> heatMap;
    void updateHeatMap(BlockItem*);
    void updateBlockChain(BlockItem*);
};

#endif // MAINWINDOW_H
