#ifndef RULESVIEWPLUGIN_H
#define RULESVIEWPLUGIN_H

#include <IPlugin.h>
#include <QMap>

#include "rulesview.h"
#include "rulestreewidget.h"

using namespace composer::extension;

/*!
 * \brief Rules View is a simple plugin allows users to graphically
 *        define rules for content adapt
 */
class RulesViewPlugin : public IPlugin
{
  Q_OBJECT
public:
  explicit RulesViewPlugin();
  ~RulesViewPlugin ();

  QWidget* getWidget() {return _rulesTable; }


  void addRule (Entity *);

public slots:
  void init();

  void onEntityAdded(QString, Entity *);
  void onEntityChanged(QString entityID, Entity *entity);
  void onEntityRemoved(QString, QString entityID);

private slots:
  void sendRemoveEntitySignal (QTreeWidgetItem *);
  void updateValue(QTreeWidgetItem*);
  void sendSelectionChangedSignal();

private:
  void findAllRules (Entity *);
  void releaseItemChildren (QTreeWidgetItem *);

  Project * _currentProject;

  QString * _selectedUId;

  RulesTreeWidget * _rulesTable;
  Entity * _ruleBaseEntity;
  QMap <QTreeWidgetItem *, QString> _items;
};

#endif // RULESVIEWPLUGIN_H
