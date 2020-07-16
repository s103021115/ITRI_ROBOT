#ifndef MAINGRAPHICSSCENE_H
#define MAINGRAPHICSSCENE_H

#include <QGraphicsScene>

class MainGraphicsScene : public QGraphicsScene
{
public:
  MainGraphicsScene(QObject* qObj);
public slots:
  void checkConnections();
};

#endif // MAINGRAPHICSSCENE_H
