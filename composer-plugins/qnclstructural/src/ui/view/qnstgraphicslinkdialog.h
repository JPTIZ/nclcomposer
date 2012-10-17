#ifndef QNSTGRAPHICSLINKDIALOG_H
#define QNSTGRAPHICSLINKDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QShowEvent>

#include <QStringListModel>
#include <QSortFilterProxyModel>

#include  "qnstconnector.h"

#include "ui_QnstGraphicsLinkForm.h"

#include <QtGui/QLineEdit>
#include <QStringList>

class QListView;
class QStringListModel;
class QModelIndex;

class CompleteLineEdit : public QLineEdit {
    Q_OBJECT
public:
    CompleteLineEdit(QStringList words, QWidget *parent = 0);
    void setStringList(const QStringList &words);

public slots:
    void setCompleter(const QString &text);
    void completeText(const QModelIndex &index);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void hideEvent ( QHideEvent * event );
    // virtual void focusOutEvent(QFocusEvent *e);
    virtual bool eventFilter(QObject *object, QEvent *event);

private:
    QStringList words;
    QListView *listView;
    QStringListModel *model;
};

class QnstGraphicsLinkDialog : public QDialog
{
    Q_OBJECT

public:
    QnstGraphicsLinkDialog(QWidget* parent = 0);

    ~QnstGraphicsLinkDialog();

    void init(QMap<QString, QnstConnector*> connectors);

protected slots:
    void adjustBinds(QString conn);

protected:
    virtual void showEvent(QShowEvent *evt);

public:
    //TODO: This shouldn't be public
    Ui::QnstGraphicsLinkForm form;

    QMap<QString, QnstConnector*> connectors;
    QString getCurrentConnector();

private:
    bool firstTime, changeModel;
    CompleteLineEdit *connLineEdit;
};

#endif // QNSTGRAPHICSLINKDIALOG_H
