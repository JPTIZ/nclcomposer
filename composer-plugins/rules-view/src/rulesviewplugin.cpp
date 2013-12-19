#include "rulesviewplugin.h"

#include <QDebug>
#include <QAction>
#include <QList>

#include "util.h"
#include "ruleitem.h"
#include "compositeruleitem.h"

const char* RULEBASE_LABEL = "ruleBase";
const char* RULE_LABEL = "rule";
const char* COMPOSITERULE_LABEL = "compositeRule";

const char* ID_ATTR = "id";
const char* VAR_ATTR = "var";
const char* OPERATOR_ATTR = "operator";
const char* COMPARATOR_ATTR = "comparator";
const char* VALUE_ATTR = "value";

const int ELEMENT_COLUMN = 0;
const int ID_COLUMN = 1;
const int VAR_COLUMN = 2;
const int COMP_COLUMN = 3;
const int VALUE_COLUMN = 4;



RulesViewPlugin::RulesViewPlugin() :
  IPlugin ()
{
  _selectedUId = 0;

  _rulesTable = new RulesTreeWidget;
  _rulesTable->setColumnCount(5);

  connect(_rulesTable, SIGNAL(itemSelectionChanged()),
          this, SLOT(sendSelectionChangedSignal()));

  connect(_rulesTable, SIGNAL(removeEntityRequested(QTreeWidgetItem*)),
          this, SLOT(sendRemoveEntitySignal(QTreeWidgetItem *)));


  QStringList headers;
  headers << "Element" << "Id" << "Variable" << "Comparator" << "Value";

  _rulesTable->setHeaderLabels(headers);

  _ruleBaseEntity = 0;

  QStringList listenFilter;
  listenFilter << RULEBASE_LABEL << RULE_LABEL << COMPOSITERULE_LABEL
               << "media";


  emit setListenFilter(listenFilter);
}

void RulesViewPlugin::onEntityAdded(QString, Entity *entity)
{
  if (!entity) return;

  disconnect(_rulesTable, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
             this, SLOT(updateValue(QTreeWidgetItem*)));

  QString entityType = entity->getType();

  if (entityType == RULEBASE_LABEL ||entityType == RULE_LABEL ||
      entityType == COMPOSITERULE_LABEL)
    addRule(entity);

  connect(_rulesTable, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
          this, SLOT(updateValue(QTreeWidgetItem*)));
}

void RulesViewPlugin::onEntityChanged(QString entityID, Entity *entity)
{
  qDebug () << "RuleViewPlugin::onEntityChanged";
}

void RulesViewPlugin::onEntityRemoved(QString, QString entityID)
{
  QList<QTreeWidgetItem *> treeItems = _items.keys();
  foreach (QTreeWidgetItem *item, treeItems)
    if (_items.value(item) == entityID)
    {
      QTreeWidgetItem *parent = item->parent();
      releaseItemChildren(item);
      delete item;
      if (!parent) //Removing ruleBase element
      {
        _ruleBaseEntity = 0;
      }
    }
}

void RulesViewPlugin::releaseItemChildren(QTreeWidgetItem *item)
{
  _items.remove(item);
  for (int i = 0; i < item->childCount(); i++)
  {
    QTreeWidgetItem *child = item->child(i);
    releaseItemChildren(child);
  }
}

void RulesViewPlugin::init()
{
  _currentProject = getProject();
  if (!_currentProject) return;

  QList <Entity *> ruleBaseList =
      _currentProject->getEntitiesbyType(RULEBASE_LABEL);

  if (ruleBaseList.count() > 0)
  {
    _ruleBaseEntity = ruleBaseList.at(0);
    addRule(_ruleBaseEntity);
    findAllRules(_ruleBaseEntity);
  }

  connect (_rulesTable, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this,
           SLOT(updateValue(QTreeWidgetItem*)));
}

void RulesViewPlugin::findAllRules(Entity *root)
{
  if (!root) return;

  foreach (Entity *entity, root->getChildren())
  {
    QString type = entity->getType();
    if (type != RULE_LABEL && type != COMPOSITERULE_LABEL) continue;

    addRule (entity);

    if (type == COMPOSITERULE_LABEL)
      findAllRules(entity);
  }
}


void RulesViewPlugin::addRule(Entity *entity)
{
  if (!entity) return;

  QStringList itemLabels;
  QString entityType = entity->getType();

  QTreeWidgetItem *item = 0;
  QTreeWidgetItem *parent = 0;

  if (entityType == RULEBASE_LABEL)
  {
    QString label = entityType;
    QString id = entity->getAttribute(ID_ATTR);

    itemLabels << label << id;
    item = new QTreeWidgetItem(_rulesTable, itemLabels, RULEBASE_TYPE);

    item->setFlags(item->flags() | Qt::ItemIsEditable);

    _ruleBaseEntity = entity;
  }

  QList<QTreeWidgetItem *> treeItems = _items.keys();
  foreach (QTreeWidgetItem *treeItem, treeItems)
  {
    if (_items.value(treeItem) == entity->getParent()->getUniqueId())
    {
      parent = treeItem;
      break;
    }
  }

  if (parent)
  {
    if (entityType == RULE_LABEL )
    {
      item = new RuleItem ( parent,
                            entity->getAttribute(ID_ATTR),
                            entity->getAttribute(VAR_ATTR),
                            entity->getAttribute(COMPARATOR_ATTR),
                            entity->getAttribute(VALUE_ATTR),
                            RULE_TYPE, _rulesTable);

    }

    else if(entityType == COMPOSITERULE_LABEL)
    {
      item = new CompositeRuleItem (parent,
                                    entity->getAttribute(ID_ATTR),
                                    entity->getAttribute(OPERATOR_ATTR),
                                    COMPOSITE_TYPE, _rulesTable);
    }
  }

  if (item)
  {
    item->setExpanded(true);
    _items.insert(item, entity->getUniqueId());
  }
}


void RulesViewPlugin::updateValue(QTreeWidgetItem* item)
{
  if (!item) return;

  Entity *entity = _currentProject->getEntityById(_items.value(item));
  if (!entity) return;

  QString oldId = entity->getAttribute(ID_ATTR);

  QMap <QString, QString> attr;

  switch (item->type())
  {
    case RULEBASE_TYPE:
      {
        QString newId = item->text(ID_COLUMN);
        if (newId != oldId)
          attr.insert("id", newId);
        break;
      }

    case RULE_TYPE:
      {
        RuleItem *ruleItem = dynamic_cast <RuleItem *> (item);
        if (!ruleItem) return;

        QString oldVar = entity->getAttribute(VALUE_ATTR);
        QString oldComparator = entity->getAttribute(COMPARATOR_ATTR);
        QString oldValue = entity->getAttribute(VALUE_ATTR);

        QString newId = ruleItem->id();
        QString newVar = ruleItem->var();
        QString newComparator = ruleItem->comparator();
        QString newValue = ruleItem->value();

        if (newId != oldId || newVar != oldVar ||
            newComparator != oldComparator || newValue != oldValue)
        {
          attr.insert(ID_ATTR, ruleItem->id());
          attr.insert(VAR_ATTR, ruleItem->var());
          attr.insert(COMPARATOR_ATTR, ruleItem->comparator());
          attr.insert(VALUE_ATTR, ruleItem->value());
        }
        break;
      }

    case COMPOSITE_TYPE:
      {
        CompositeRuleItem *compositeRuleItem =
            dynamic_cast <CompositeRuleItem *> (item);

        if (!compositeRuleItem) return;

        QString oldOperator = entity->getAttribute(OPERATOR_ATTR);

        QString newId = compositeRuleItem->id();
        QString newOperator = compositeRuleItem->getOperator();

        if (newId != oldId || newOperator != oldOperator )
        {
          attr.insert("id", compositeRuleItem->id());
          attr.insert("operator", compositeRuleItem->getOperator());
        }
        break;
      }
  }

  if (!attr.isEmpty())
    emit setAttributes(entity, attr, true);

}

void RulesViewPlugin::sendSelectionChangedSignal()
{
  QTreeWidgetItem * currentItem = _rulesTable->currentItem();
  if (currentItem)
  {
    if (_selectedUId)
      delete _selectedUId;

    _selectedUId = new QString (_items.value(currentItem));

    emit sendBroadcastMessage("changeSelectedEntity", _selectedUId);
  }
}

void RulesViewPlugin::sendRemoveEntitySignal(QTreeWidgetItem *item)
{
  if (! item) return;

  QString uId = _items.value(item);

  Entity *entity = _currentProject->getEntityById(uId);
  if (entity)
    emit removeEntity(entity, true);
}

RulesViewPlugin::~RulesViewPlugin()
{

  delete _rulesTable;
}
