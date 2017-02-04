/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QMetaObject>
#include <QMetaMethod>

#include <QDialogButtonBox>
#include <QListWidgetItem>

namespace composer {
namespace gui {

PreferencesDialog::PreferencesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferencesDialog)
{
  ui->setupUi(this);
  this->setModal(true);

  loadPreferencesPages();

  connect( ui->listWidget,
          SIGNAL(itemSelectionChanged()),
          this,
          SLOT(changeActivePage()) );

  connect( ui->buttonBox_2,
           SIGNAL(clicked(QAbstractButton*)),
           this,
           SLOT(buttonClicked(QAbstractButton*)) );


  connect( this,
           SIGNAL(accepted()),
           this,
           SLOT(applyCurrentValues()) );

  currentItem = nullptr;
}

PreferencesDialog::~PreferencesDialog()
{
  delete ui;
}

void PreferencesDialog::addPreferencePage(IPluginFactory *pF)
{
  if (pF->getPreferencePageWidget() == nullptr)
      return;

  addPreferencePage( pF->icon(),
                     pF->metadata().value("name").toString(),
                     (IPreferencesPage*) pF->getPreferencePageWidget());
}

void PreferencesDialog::addPreferencePage(QIcon icon, QString name,
                                          IPreferencesPage *page)
{
  new QListWidgetItem(icon, name, ui->listWidget, 0);

  pages[name] = page;
  page->hide();
  ui->scrollAreaVerticalLayout->addWidget(page);
}

void PreferencesDialog::addPreferencePage(IPreferencesPage *page)
{
  new QListWidgetItem(page->getIcon(), page->getName(), ui->listWidget, 0);

  pages[page->getName()] = page;
  page->hide();
  ui->scrollAreaVerticalLayout->addWidget(page);
}

void PreferencesDialog::loadPreferencesPages()
{
  QList<IPluginFactory*> plugins = PluginControl::getInstance()->
      getLoadedPlugins();
  QList<IPluginFactory*>::iterator it;

  for (it = plugins.begin(); it != plugins.end(); it++)
  {
    IPluginFactory *pF = *it;
    if (pF->getPreferencePageWidget() == nullptr)
        continue;

    new QListWidgetItem( pF->icon(),
                         pF->metadata().value("name").toString(),
                         ui->listWidget, 0 );

    IPreferencesPage *page = (IPreferencesPage*) pF->getPreferencePageWidget();
    pages[pF->metadata().value("name").toString()] = page;
    page->hide();
    ui->scrollAreaVerticalLayout->addWidget(page);
  }
}

void PreferencesDialog::changeActivePage()
{
  if (currentItem != nullptr)
  {
    if(pages.contains(currentItem->text()))
    {
      pages[currentItem->text()]->hide();
    }
  }

  currentItem = ui->listWidget->currentItem();
  if(currentItem != nullptr)
  {
    if(pages.contains(currentItem->text()))
    {
      pages[currentItem->text()]->show();
      currentPage = pages[currentItem->text()];
      ui->pageTitle->setText(currentItem->text());
    }
  }
}

void PreferencesDialog::buttonClicked(QAbstractButton* button)
{
  QDialogButtonBox::StandardButton std = ui->buttonBox_2->standardButton(button);

  if(std == QDialogButtonBox::Apply)
    applyCurrentValues();
}

void PreferencesDialog::applyCurrentValues()
{
  currentPage->applyValues();
}

/* void PreferencesDialog::show()
{
  QDialog::show();
  selectFirst();
}

void PreferencesDialog::selectFirst()
{
  // \todo And if we do not have any child?
  ui->listWidget->item(0)->setSelected(true);
} */

}} //end namespace
