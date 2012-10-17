#ifndef QNSTGRAPHICSCONDITION_H
#define QNSTGRAPHICSCONDITION_H

#include "qnstgraphicsedge.h"

#include "qnstgraphicsbinddialog.h"

typedef QString QnstConditionIcon;
typedef Qnst::ConditionType QnstCondition;

class QnstGraphicsCondition : public QnstGraphicsEdge
{
    Q_OBJECT

public:
    QnstGraphicsCondition(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsCondition();

    QnstCondition getCondition();

    void setCondition(QnstCondition condition);

    QnstConditionIcon getConditionIcon();

    void setConditionIcon(QnstConditionIcon conditionIcon);

    void adjust();

    void aux_adjust(QPointF pointa, QPointF pointb);

    void setConn(QnstConnector* conn);

    void setNameUids(QMap<QString, QString> nameUids);

    void setParams(QMap<QString, QString> params);

    void addParam(QString uid, QString name, QString value);

    void setParam(QString name, QString value);

    void removeParam(QString name);

    void removeUId(QString uid);

signals:
    void bindParamAdded(QString uid, QString parent, QMap<QString, QString> properties);

    void bindParamUpdated(QString bindUid, QMap<QString, QString> params,  QMap<QString, QString> name_uids);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QnstCondition condition;

    QnstConditionIcon conditionIcon;

    QnstConnector* conn;

    QMap<QString, QString> name_uid;

    QMap<QString, QString> params;

    QnstGraphicsBindDialog* dialog;
};

#endif // QNSTGRAPHICSCONDITION_H
