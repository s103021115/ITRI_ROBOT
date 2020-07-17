#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockitem.h"

#include <fstream>
#include <iostream>


using namespace std;

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
void MainWindow::loadBlockChain() {

    for(int i=0;i<blocks.size();i++)
    {
        if(i>0)
            blocks[i]->prev = blocks[i-1];
        if(i<blocks.size()-1)
            blocks[i]->next = blocks[i+1];
    }
    updateBlockChain(blocks[0]);

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    this->currentSelectedInstruction = arg1;
}

//void MainWindow::splitString(string str, vector<string> &list)
//{
//    int current = 0;
//    int next;
//    while(current<str.size())
//    {
//        next = str.find_first_of(" ",current);
//        if(next != current)
//        {
//            string tmp = str.substr(current,next-current);
//            if(tmp.size() != 0)
//            {
//                list.push_back(tmp);
//            }
//        }
//        if(next == string::npos)
//            break;
//        current = next+1;
//    }
//}

void MainWindow::on_Save_clicked()
{

//    fstream file, heap;
//    string filename = "../ITRI_ROBOT-master/save/blocks.irb";
//    string heatname = "../ITRI_ROBOT-master/save/heat.irb";
//    file.open(filename,ios::out);
//    heap.open(heatname,ios::out);
//    for(int i=0;i<blocks.size();i++)
//    {
//        file<<blocks[i]->instruction.toStdString()<<" "<<blocks[i]->var1->text().toStdString()<<" "<<blocks[i]->var2->text().toStdString()<<"\n\n";
//    }
//    BlockItem *tmp = heatMap[0].first;
//    while(tmp != nullptr)
//    {
//        heap<<tmp->instruction.toStdString()<<" "<<tmp->var1->text().toStdString()<<" "<<tmp->var2->text().toStdString()<<"\n\n";
//        tmp = tmp->next;
//    }
//    cout<<"write file finish.\n";

//    file.close();

    QString fileName = QFileDialog::getSaveFileName(NULL, QObject::tr("Text file"),
        qApp->applicationDirPath(), QObject::tr("Files (*.irb)"));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for writing:" << qPrintable(file.errorString());
        return;
    }
    else
    {
        qDebug() << "Open file for writing:" << qPrintable(file.errorString());
    }

    QTextStream out(&file);
    BlockItem *tmp = heatMap[0].first;
    while(tmp != nullptr)
    {
        out<<tmp->instruction<<" "<<tmp->var1->text()<<" "<<tmp->var2->text()<<"\n\n";
        tmp = tmp->next;
    }

    file.close();

}

void MainWindow::on_Load_clicked()
{
    blocks.clear();
    heatMap.clear();
    scene->clear();

    QString fileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Text file"),
        qApp->applicationDirPath(), QObject::tr("Files (*.irb)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for reading:" << qPrintable(file.errorString());
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString fileLine = in.readLine();
        QString instruction;

        if(fileLine.size() == 0)
            continue;

        instruction = fileLine.split(" ").at(0);


        BlockItem *b = new BlockItem(blocks.size(), instruction);
      //  QRectF heat = QRectF(b->pos().x(), b->pos().y()+b->height-20, 100, 40);
        QObject::connect(b, &BlockItem::on_block_mouse_release, this, &MainWindow::on_block_mouse_release);
        QObject::connect(b, &BlockItem::on_block_mouse_press, this, &MainWindow::on_block_mouse_press);
        QObject::connect(b, &BlockItem::on_block_mouse_move, this, &MainWindow::on_block_mouse_move);
        blocks.push_back(b);
        b->var1->setText(fileLine.split(" ").at(1));
        b->var2->setText(fileLine.split(" ").at(2));
        updateHeatMap(b);
      //  heatMap.push_back(std::make_pair(b, heat));
        scene->addItem(b);
    }
    loadBlockChain();
    file.close();


}
