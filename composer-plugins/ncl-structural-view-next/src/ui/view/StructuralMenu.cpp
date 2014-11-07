#include "StructuralMenu.h"

StructuralMenu::StructuralMenu(QWidget* parent)
  : QMenu(parent)
{
  createActions();
  createMenus();
  createConnections();
}

StructuralMenu::~StructuralMenu()
{

}

void StructuralMenu::createActions()
{
  // help action
  helpAction = new QAction(this);
  helpAction->setEnabled(false);
  helpAction->setText(tr("Help"));
  helpAction->setShortcut(QKeySequence("F1"));

  // undo action
  undoAction = new QAction(this);
  undoAction->setEnabled(false);
  undoAction->setText(tr("Undo"));
  undoAction->setShortcut(QKeySequence("Ctrl+Z"));

  // redo action
  redoAction = new QAction(this);
  redoAction->setEnabled(false);
  redoAction->setText(tr("Redo"));
  redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

  // cut action
  cutAction = new QAction(this);
  cutAction->setEnabled(false);
  cutAction->setText(tr("Cut"));
  cutAction->setShortcut(QKeySequence("Ctrl+X"));

  // copy action
  copyAction = new QAction(this);
  copyAction->setEnabled(false);
  copyAction->setText(tr("Copy"));
  copyAction->setShortcut(QKeySequence("Ctrl+C"));

  // paste action
  pasteAction = new QAction(this);
  pasteAction->setEnabled(false);
  pasteAction->setText(tr("Paste"));
  pasteAction->setShortcut(QKeySequence("Ctrl+V"));

  // delete action
  deleteAction = new QAction(this);
  deleteAction->setEnabled(false);
  deleteAction->setText(tr("Delete"));
  deleteAction->setShortcut(QKeySequence("Del"));

  // export action
  snapshotAction = new QAction(this);
  snapshotAction->setEnabled(false);
  snapshotAction->setText(tr("Snapshot..."));

  // media action
  mediaAction = new QAction(this);
  mediaAction->setEnabled(false);
  mediaAction->setText(tr("Media"));
  mediaAction->setIcon(QIcon(":/images/icon/media-insert"));

  // context action
  contextAction = new QAction(this);
  contextAction->setEnabled(false);
  contextAction->setText(tr("Context"));
  contextAction->setIcon(QIcon(":/images/icon/context-insert"));

  // switch action
  switchAction = new QAction(this);
  switchAction->setEnabled(false);
  switchAction->setText(tr("Switch"));
  switchAction->setIcon(QIcon(":/images/icon/switch-insert"));

  // body action
  bodyAction = new QAction(this);
  bodyAction->setEnabled(false);
  bodyAction->setText(tr("Body"));
  bodyAction->setIcon(QIcon(":/images/icon/body-insert"));

  // port action
  portAction = new QAction(this);
  portAction->setEnabled(false);
  portAction->setText(tr("Port"));
  portAction->setIcon(QIcon(":/images/icon/port-insert"));

  // switchport action
  switchPortAction = new QAction(this);
  switchPortAction->setEnabled(false);
  switchPortAction->setText(tr("Switch Port"));
  switchPortAction->setIcon(QIcon(":/images/icon/switchport-insert"));

  // area action
  areaAction = new QAction(this);
  areaAction->setEnabled(false);
  areaAction->setText(tr("Area"));
  areaAction->setIcon(QIcon(":/images/icon/area-insert"));

  // property action
  propertyAction = new QAction(this);
  propertyAction->setEnabled(false);
  propertyAction->setText(tr("Property"));
  propertyAction->setIcon(QIcon(":/images/icon/property-insert"));
}

void StructuralMenu::createMenus()
{
  // insert menu
  insertMenu = new QMenu();
  insertMenu->setTitle(tr("Insert"));

  insertMenu->addAction(mediaAction);
  insertMenu->addSeparator();
  insertMenu->addAction(contextAction);
  insertMenu->addAction(switchAction);
  insertMenu->addAction(bodyAction);
  insertMenu->addSeparator();
  insertMenu->addAction(areaAction);
  insertMenu->addAction(propertyAction);
  insertMenu->addAction(portAction);
  insertMenu->addAction(switchPortAction);
  insertMenu->setEnabled(false);

  addAction(helpAction);
  addSeparator();
  addAction(undoAction);
  addAction(redoAction);
  addSeparator();
  addAction(cutAction);
  addAction(copyAction);
  addAction(pasteAction);
  addSeparator();
  addAction(deleteAction);
  addSeparator();
  addAction(snapshotAction);
  addSeparator();
  addMenu(insertMenu);
}

void StructuralMenu::createConnections()
{
  connect(mediaAction,SIGNAL(triggered()),SLOT(performMedia()));
  connect(bodyAction,SIGNAL(triggered()),SLOT(performBody()));
  connect(contextAction,SIGNAL(triggered()),SLOT(performContext()));
  connect(switchAction,SIGNAL(triggered()),SLOT(performSwitch()));
  connect(portAction,SIGNAL(triggered()),SLOT(performPort()));
  connect(areaAction,SIGNAL(triggered()),SLOT(performArea()));
  connect(switchPortAction,SIGNAL(triggered()),SLOT(performSwitchPort()));
  connect(propertyAction,SIGNAL(triggered()),SLOT(performProperty()));
}

void StructuralMenu::performBody()
{
  insert(Structural::Body);
}

void StructuralMenu::performContext()
{
  insert(Structural::Context);
}

void StructuralMenu::performSwitch()
{
  insert(Structural::Switch);
}

void StructuralMenu::performMedia()
{
  insert(Structural::Media);
}

void StructuralMenu::performPort()
{
  insert(Structural::Port);
}

void StructuralMenu::performArea()
{
  insert(Structural::Area);
}

void StructuralMenu::performSwitchPort()
{
  insert(Structural::SwitchPort);
}

void StructuralMenu::performProperty()
{
  insert(Structural::Property);
}
