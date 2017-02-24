/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QDialogButtonBox>

namespace composer {
namespace gui {

AboutDialog::AboutDialog(QWidget *parent):
  QDialog(parent, Qt::Dialog),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);
  ui->label_ProgramName->setText( QString("NCL Composer v.") +
                                  qApp->applicationVersion() );

  ui->label_buildDate->setText(QString (BUILD_DATE));

  connect(ui->button_Close, SIGNAL(pressed()), this, SLOT(close()));

  connect(ui->button_aboutQt, SIGNAL(pressed()), qApp, SLOT(aboutQt()));
}

AboutDialog::~AboutDialog()
{
  delete ui;
}

void AboutDialog::showLicense()
{
  QMessageBox box(this);
  QFile file(":/LICENSE.LGPL");
  box.setInformativeText(file.readAll());
  box.setTextFormat(Qt::RichText);
  box.exec();
}

AboutPluginsDialog::AboutPluginsDialog(QWidget *parent)
{
  setWindowTitle(tr("Installed Plugins"));

  _treeWidgetPlugins = new QTreeWidget(this);
  _treeWidgetPlugins->setAlternatingRowColors(true);

  connect(_treeWidgetPlugins, SIGNAL(itemSelectionChanged()),
          this, SLOT(selectedAboutCurrentPluginFactory()));

  QStringList header;
  header << tr("Name") << tr("Load") << tr("Version") << tr("Vendor");
  _treeWidgetPlugins->setHeaderLabels(header);

  QDialogButtonBox *bOk = new QDialogButtonBox(QDialogButtonBox::Ok |
                                               QDialogButtonBox::Close,
                                               Qt::Horizontal,
                                               this);

  _detailsButton = bOk->button(QDialogButtonBox::Ok);
  _detailsButton->setText(tr("Details"));
  _detailsButton->setIcon(QIcon());
  _detailsButton->setEnabled(false);

  connect( bOk, SIGNAL(rejected()), this, SLOT(close()) );

  connect( _detailsButton, SIGNAL(pressed()), this, SLOT(showPluginDetails()) );

  QGridLayout *gLayout = new QGridLayout(this);
  gLayout->addWidget(new QLabel(tr("<b>NCL Composer</b> is an IDE for"
                                   " Declarative Multimedia languages."),
                                this));

  gLayout->addWidget(new QLabel(tr("<b>Installed Plug-ins</b>")));
  gLayout->addWidget(_treeWidgetPlugins);
  gLayout->addWidget(bOk);
  setLayout(gLayout);

  setModal(true);

  connect( this, SIGNAL(finished(int)),
           this, SLOT(saveLoadPluginData(int)) );

  _pluginDetailsDialog = new PluginDetailsDialog(this);
}

void AboutPluginsDialog::loadPlugins()
{
  QList<ILanguageProfile*> langList = LanguageControl::getInstance()->
      getLoadedProfiles();
  QList<IPluginFactory*> pList = PluginControl::getInstance()->
      getLoadedPlugins();


  //search for categories
  QTreeWidgetItem *treeWidgetItem;
  QMap <QString, QTreeWidgetItem*> categories;

  QString category = "Language profile";
  for (const ILanguageProfile *langProfile: langList)
  {
    if(!categories.contains(category))
    {
      treeWidgetItem = new QTreeWidgetItem(_treeWidgetPlugins);
      categories.insert(category, treeWidgetItem);
      treeWidgetItem->setText(0, category);
      treeWidgetItem->setTextColor(0, QColor("#0000FF"));
    }
  }

  for (IPluginFactory *pF: pList)
  {
    QString category = pF->metadata().value("category").toString();

    if(!categories.contains(category))
    {
      treeWidgetItem = new QTreeWidgetItem(_treeWidgetPlugins);
      categories.insert(category, treeWidgetItem);
      treeWidgetItem->setText(0, category);
      treeWidgetItem->setTextColor(0, QColor("#0000FF"));
    }
  }

  for (ILanguageProfile *langProfile: langList)
  {
    QString category = "Language profile";
    treeWidgetItem = new QTreeWidgetItem ( categories.value(category) );
    treeWidgetItem->setText(0, langProfile->getProfileName());

    treeWidgetItem->setCheckState(1, Qt::Checked);
    treeWidgetItem->setDisabled(true);
  }

  for (IPluginFactory *pF: pList)
  {
    QString category = pF->metadata().value("category").toString();
    treeWidgetItem = new QTreeWidgetItem ( categories.value(category) );
    treeWidgetItem->setText(0, pF->metadata().value("name").toString());
    treeWidgetItem->setData(0, Qt::UserRole, qVariantFromValue(pF));

    // Set checked (or not) based on the settings
    GlobalSettings settings;
    settings.beginGroup("loadPlugins");
    if(!settings.contains(pF->id()) || settings.value(pF->id()).toBool())
      treeWidgetItem->setCheckState(1, Qt::Checked);
    else
      treeWidgetItem->setCheckState(1, Qt::Unchecked);

    settings.endGroup();
    treeWidgetItem->setText(2, pF->metadata().value("version").toString());
    treeWidgetItem->setText(3, pF->metadata().value("vendor").toString());
  }

  _treeWidgetPlugins->expandAll();

  _treeWidgetPlugins->setColumnWidth(0, 150);
  _treeWidgetPlugins->resizeColumnToContents(1);
  _treeWidgetPlugins->resizeColumnToContents(2);
  _treeWidgetPlugins->resizeColumnToContents(3);

  _detailsButton->setEnabled(false);
}

void AboutPluginsDialog::selectedAboutCurrentPluginFactory()
{
  QList<QTreeWidgetItem*> selectedPlugins = _treeWidgetPlugins->selectedItems();
  if(selectedPlugins.size())
  {
    QTreeWidgetItem *item = selectedPlugins.at(0);
    QVariant itemVariant = item->data(0, Qt::UserRole);

    IPluginFactory *pluginFactory = itemVariant.value<IPluginFactory*>();
    if(pluginFactory)
    {
      _pluginDetailsDialog->setCurrentPlugin(pluginFactory);
      _detailsButton->setEnabled(true);
    }
  }
  else
    _detailsButton->setEnabled(false);
}

void AboutPluginsDialog::saveLoadPluginData(int)
{
  GlobalSettings settings;
  settings.beginGroup("loadPlugins");

  for (int i = 0; i < _treeWidgetPlugins->topLevelItemCount(); ++i)
  {
    QTreeWidgetItem *cat = _treeWidgetPlugins->topLevelItem(i);
    for (int j = 0; j < cat->childCount(); ++j)
    {
      QTreeWidgetItem *item = cat->child(j);
      QVariant itemVariant = item->data(0, Qt::UserRole);
      IPluginFactory *pluginFactory = itemVariant.value<IPluginFactory*>();
      if(pluginFactory)
      {
        if(item->checkState(1))
        {
          settings.setValue(pluginFactory->id(), true);
        }
        else
        {
          settings.setValue(pluginFactory->id(), false);
        }
      }
    }
  }

  settings.endGroup();
}

/*!
 * \brief Shows the details of the current selected plugins.
 */
void AboutPluginsDialog::showPluginDetails()
{
  _pluginDetailsDialog->show();
}

} } // end namespace

void composer::gui::AboutDialog::on_button_Homepage_pressed()
{
  QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br"));
}
