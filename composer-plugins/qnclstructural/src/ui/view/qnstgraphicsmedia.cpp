#include "qnstgraphicsmedia.h"

#include <QDrag>

#include "qnstutil.h"

QnstGraphicsMedia::QnstGraphicsMedia(QnstGraphicsEntity* parent)
  : QnstGraphicsNode(parent), enableDrag(false)
{
  setnstType(Qnst::Media);

  setResizable(false);

  createObjects();
  createConnections();

  setnstId("");

  /* Default media position */
  if(parent)
  {
    setTop(parent->getHeight()/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(parent->getWidth()/2 - DEFAULT_MEDIA_WIDTH/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(DEFAULT_MEDIA_WIDTH);
  setHeight(DEFAULT_MEDIA_HEIGHT);
}

QnstGraphicsMedia::~QnstGraphicsMedia()
{

}

QString QnstGraphicsMedia::getIcon() const
{
  return icon;
}

QString QnstGraphicsMedia::getSource() const
{
  return source;
}

void QnstGraphicsMedia::setSource(QString source)
{
  this->source = source;
}

void QnstGraphicsMedia::setnstType(QnstType type)
{
  QnstGraphicsNode::setnstType(type);

  this->icon = QnstUtil::iconFromMediaType(type);
}

void QnstGraphicsMedia::updateToolTip()
{
  QString tip = "";
  QString name = (getnstId() != "" ? getnstId() : "?");

  switch(getnstType())
  {
    case Qnst::Image:
      tip += "Image ("+name+")";
      break;

    case Qnst::Audio:
      tip += "Audio ("+name+")";
      break;

    case Qnst::Text:
      tip += "Text ("+name+")";
      break;

    case Qnst::Video:
      tip += "Video ("+name+")";
      break;

    case Qnst::NCLua:
      tip += "NCLua ("+name+")";
      break;

    case Qnst::Html:
      tip += "HTML ("+name+")";
      break;

    case Qnst::NCL:
      tip += "NCL ("+name+")";
      break;

    case Qnst::Settings:
      tip += "Settings ("+name+")";
      break;

    default:
      tip += "Media ("+name+")";
      break;
  }

  if (hasError)
    tip += " - Error: " + erroMsg;
  else if (getSource().isEmpty())
    tip += " - Alert: Missing 'src' attribute";

  setToolTip(tip);
}

QString QnstGraphicsMedia::getRefer() const
{
  return refer;
}

void QnstGraphicsMedia::setRefer(QString refer)
{
  this->refer = refer;
}

QString QnstGraphicsMedia::getReferUID() const
{
  return referUID;
}

void QnstGraphicsMedia::setReferUID(QString refetUID)
{
  this->referUID = refetUID;
}

QString QnstGraphicsMedia::getInstance() const
{
  return instance;
}

void QnstGraphicsMedia::setInstance(QString instance)
{
  this->instance = instance;
}

void QnstGraphicsMedia::createObjects()
{
  menu = new QnstMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);
  menu->actionAddArea->setEnabled(true);
  menu->actionAddProperty->setEnabled(true);
}

void QnstGraphicsMedia::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

  connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
  connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
  connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

  connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  connect(menu, SIGNAL(menuAddEntityTriggered(Qnst::EntityType)),
          SLOT(createEntity(Qnst::EntityType)));

  connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
  connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
  connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
  connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));
}

void QnstGraphicsMedia::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(icon));

  drawMouseHoverHighlight(painter); // This should not be HERE!!

  painter->setPen(QPen(QBrush(Qt::black),0));

  if(hasError)
  {
    painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/delete"));
  }
  else if (getSource().isEmpty())
  {
    painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/alert"));
  }

  if(!getReferUID().isEmpty() && !getReferUID().isEmpty())
  {
    painter->drawPixmap(8, getHeight()-40, 32, 32, QPixmap(":/icon/alias"));
  }

  QString localid = (getnstId() != "" ? getnstId() : "?");

  if (localid.length() > 5)
  {
    localid = getnstId().replace(3,getnstId().length()-3,"...");
  }

  // draw a formated text with underline when there is error
  if(hasError)
  {
    int N_STEPS = 20;
    int begin_w = 8;
    int end_w = getWidth();
    double current_x = begin_w;
    double step = (double) ( end_w - begin_w ) / N_STEPS;

    QPolygonF polygon;
    painter->setPen(QPen(QBrush(Qt::red), 0)); // 0px = cosmetic border
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < N_STEPS; i++)
    {
      current_x = begin_w + (double) i * step;

      if( i % 2)
        polygon << QPointF( current_x, getHeight() - 3 );
      else
        polygon << QPointF( current_x, getHeight() );
    }

    painter->drawPolyline(polygon);
  }

  // draw the text
  painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, localid);

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
  }
}

void QnstGraphicsMedia::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}


void QnstGraphicsMedia::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  qDebug() << "QnstGraphicsMedia::mousePressEvent" << isDraggable();
  if(isDraggable())
  {
    QMimeData *data = new QMimeData;
    data->setColorData(Qt::green);
    data->setData("nclcomposer/mediaid", getnstId().toAscii());
    data->setData("nclcomposer/qnstuid", getnstUid().toAscii());

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->start();
  }
  else
    QnstGraphicsNode::mousePressEvent(event);
}

void QnstGraphicsMedia::setProperties(const QMap<QString, QString> &properties)
{
  QnstGraphicsNode::setProperties(properties);

  setSource(properties["src"]);
  setRefer(properties["refer"]);
  setReferUID(properties["referUID"]);
  setInstance(properties["instance"]);
}

void QnstGraphicsMedia::getProperties(QMap<QString, QString> &properties)
{
  QnstGraphicsNode::getProperties(properties);

  properties["src"] = getSource();
  properties["refer"] = getRefer();
  properties["referUID"] = getReferUID();
  properties["instance"] = getInstance();
}
