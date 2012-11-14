#ifndef QNSTGRAPHICSCOMPOSITION_H
#define QNSTGRAPHICSCOMPOSITION_H

#include "qnst.h"
#include "qnstgraphicsnode.h"

#include <cmath>

#include <QGraphicsSceneMouseEvent>

// Spring Algoritms Params
#define SPRING_LENGTH 125
#define SPRING_CONSTANT 0.1
#define SPRING_DAMPING 0.5

#define SPRING_INTERATION 50

class QnstGraphicsComposition : public QnstGraphicsNode
{
  Q_OBJECT

public:
  QnstGraphicsComposition(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsComposition();

  QString getColor() const;

  void setColor(QString color);

  void setCollapsed(bool collapsed);

  qreal getLastW();

  qreal getLastH();

  void setLastW(qreal lastW);

  void setLastH(qreal lastH);

  bool isCollapsed();

  void collapse();

  virtual void updateTooltip();

  virtual void setProperties(const QMap <QString, QString> &properties);

  virtual void getProperties(QMap<QString, QString> &properties);

protected slots:
  void adjustWithSpring();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

  virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
  bool collapsed;

  QString color;

  qreal lastw;

  qreal lasth;

  QnstGraphicsNode* tmp;
};

#endif // QNSTGRAPHICSCOMPOSITION_H
