#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockitem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    QList<QString> instructions{
      "MOVA",
      "MOVC",
      "WAIT",
      "BLEND",
      "HOLD"
    };
    foreach(const QString i, instructions) {
      ui->comboBox->addItem(i);
    }
    this->currentSelectedInstruction = instructions.first();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
  BlockItem *b = new BlockItem(blocks.size(), this->currentSelectedInstruction);
//  QRectF heat = QRectF(b->pos().x(), b->pos().y()+b->height-20, 100, 40);
  QObject::connect(b, &BlockItem::on_block_mouse_release, this, &MainWindow::on_block_mouse_release);
  QObject::connect(b, &BlockItem::on_block_mouse_press, this, &MainWindow::on_block_mouse_press);
  QObject::connect(b, &BlockItem::on_block_mouse_move, this, &MainWindow::on_block_mouse_move);
  blocks.push_back(b);
  updateHeatMap(b);
//  heatMap.push_back(std::make_pair(b, heat));
  scene->addItem(b);
}

void MainWindow::on_block_mouse_release(BlockItem* block) {
  QRectF blockIndent = QRectF(block->pos().x()+10, block->pos().y(), 10, 5);
  bool needUpdate = false;
  BlockItem *aboveBlock = nullptr, *belowBlock = nullptr, *temp = nullptr;

  // Update block chain
  for(unsigned i=0; i<heatMap.size(); i++) {
    if(heatMap[i].second.contains(blockIndent)) {
      needUpdate = true;
      aboveBlock = heatMap[i].first;
      belowBlock = heatMap[i].first->next;
      if(belowBlock == block) {
        break;
      }
      if(aboveBlock == block) {
        break;
      }


      // Unset original prev to this link
      if(block->prev != nullptr) {
        block->prev->next = nullptr;
      }

      // Connect first block of currently dragging block chain
      aboveBlock->next = block;
      block->prev = aboveBlock;

      // Update currently dragging block chain
      if(block->next != nullptr) {
        temp = block->next;
        while(temp->next != nullptr) {
          temp = temp->next;
        }
      } else {
        temp = block;
      }
      // temp is pointing to the last block of currently dragging block chain
      temp->next = belowBlock;
      if(belowBlock != nullptr) {
        belowBlock->prev = temp;
      }
      break;
    }
  }

  // Update block positions

  if(needUpdate) {
    updateBlockChain(aboveBlock);
  } else {
    if(block->prev != nullptr) {
      block->prev->next = nullptr;
    }
    block->prev = nullptr;
    if(block->next != nullptr) {
      updateBlockChain(block);
    }
  }


  updateHeatMap(block);
  return;
}

void MainWindow::on_block_mouse_press(BlockItem* block) {
  foreach(BlockItem* b, blocks) {
    b->setZValue(0);
  }
  block->setZValue(1);
}

void MainWindow::on_block_mouse_move(BlockItem* block) {
  if(block->next != nullptr) {
    updateBlockChain(block);
  }
}

void MainWindow::updateHeatMap(BlockItem* b) {
  int id = b->id;
  int rmIdx = -1;
  for(unsigned i=0; i<heatMap.size(); i++) {
    if(heatMap[i].first->id == id) {
      rmIdx = i;
      QRectF heat = QRectF(b->pos().x(), b->pos().y()+b->height-20, 100, 40);
      heatMap.push_back(std::make_pair(b, heat));
      break;
    }
  }
  if(rmIdx != -1) {
    heatMap.erase(heatMap.begin()+rmIdx);
  } else {
    QRectF heat = QRectF(b->pos().x(), b->pos().y()+b->height-20, 100, 40);
    heatMap.push_back(std::make_pair(b, heat));
  }
}

void MainWindow::updateBlockChain(BlockItem *head) {
  while(head != nullptr) {
    if(head->next == nullptr){
      break;
    }
    head->next->setPos(QPointF(head->pos().x(), head->pos().y()+head->height));
    updateHeatMap(head->next);
    head = head->next;
  }
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    this->currentSelectedInstruction = arg1;
}
