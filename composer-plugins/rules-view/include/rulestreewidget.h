#ifndef RULESTREEWIDGET_H
#define RULESTREEWIDGET_H

#include <QTreeWidget>

static const int RULEBASE_TYPE = QTreeWidgetItem::UserType;
static const int RULE_TYPE = QTreeWidgetItem::UserType + 1;
static const int COMPOSITE_TYPE = QTreeWidgetItem::UserType + 2;


class RulesTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit RulesTreeWidget(QWidget *parent = 0);

signals:
  void removeEntityRequested (QTreeWidgetItem *);

private slots:
  void onCustomContextMenuRequested(const QPoint &pos);
  void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos);
  void editItem (QTreeWidgetItem *item, int column);
};

#endif // RULESTREEWIDGET_H
