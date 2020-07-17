#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <QWidget>
#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QString>

class BlockItem : public QObject, public QGraphicsItem {
  Q_OBJECT
  public:
    int id;
    int height;
    int width;
    QString instruction;

    QString instruction_var;
    QLineEdit *var1, *var2;

    BlockItem* next;
    BlockItem* prev;
    BlockItem(int, QString);
    QRectF boundingRect() const;

    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);


  protected:
    QBrush        m_qBrush;
    QPainterPath  m_qPath;
    QGraphicsProxyWidget* proxyWidget;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void handleCollision();

  signals:
    void on_block_mouse_release(BlockItem*);
    void on_block_mouse_press(BlockItem*);
    void on_block_mouse_move(BlockItem*);

};

#endif // BLOCKITEM_H
