#include "blockitem.h"

BlockItem::BlockItem(int id, QString instruction) {
  this->id = id;
  this->instruction = instruction;
  m_qBrush.setColor( QColor::fromRgb( 255, 0, 255 ) );
  m_qBrush.setStyle( Qt::SolidPattern );
  height = 50;
  width = 400;
  next = nullptr;
  prev = nullptr;
  QVector<QPointF> v;
  {
    v.push_back(QPointF(0,0));
    v.push_back(QPointF(10,0));
    v.push_back(QPointF(15,5));
    v.push_back(QPointF(20,0));
    v.push_back(QPointF(width,0));
    v.push_back(QPointF(width,height));
    v.push_back(QPointF(20,height));
    v.push_back(QPointF(15,height+5));
    v.push_back(QPointF(10,height));
    v.push_back(QPointF(0,height));
  }
  m_qPath.addPolygon( QPolygonF( v ) );
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  this->setFlag(QGraphicsItem::ItemIsMovable, true);
  this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

  QLabel *instructionLabel = new QLabel(this->instruction);
  this->var1 = new QLineEdit;
  this->var2 = new QLineEdit;
  this->var1->setStyleSheet("border: 3px solid black;");
  this->var2->setStyleSheet("border: 3px solid black;");
//  QLineEdit *pt1 = new QLineEdit;
//  pt1->setStyleSheet("border: 3px solid black;");
//  QLineEdit *pt2 = new QLineEdit;
//  pt2->setStyleSheet("border: 3px solid black;");
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(instructionLabel);
//  layout->addWidget(pt1);
//  layout->addWidget(pt2);
  layout->addWidget(this->var1);
  layout->addWidget(this->var2);
  proxyWidget = new QGraphicsProxyWidget(this);
  auto widget = new QWidget();
  widget->setStyleSheet("background-color:transparent;");
  widget->setLayout(layout);
  proxyWidget->setWidget(widget);
}

//QList<BlockItem*> BlockItem::collidingItems(Qt::ItemSelectionMode mode) {
//  QList<QGraphicsItem*> c = this->collidingItems(mode);
//}


QRectF BlockItem::boundingRect() const {
  return m_qPath.boundingRect();
}

//QPainterPath BlockItem::shape() const {
//  return m_qPath;
//}

void BlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->fillPath( m_qPath, m_qBrush );
}

void BlockItem::handleCollision() {
  return;
}

void BlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseReleaseEvent(event);
  handleCollision();
  emit on_block_mouse_release(this);
}

void BlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mousePressEvent(event);
  emit on_block_mouse_press(this);
}


void BlockItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QGraphicsItem::mouseMoveEvent(event);
  emit on_block_mouse_move(this);
}
