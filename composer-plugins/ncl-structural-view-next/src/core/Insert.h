#ifndef QNSTINSERT_H
#define QNSTINSERT_H

#include "Command.h"

class Insert : public Command
{
public:
  Insert(QString _uid, QString _parent, QMap<QString, QString> _properties, QMap<QString, QString> _settings);
  virtual ~Insert();

  virtual void undo();
  virtual void redo();

private:
  QString _uid;
  QString _parent;
  QMap<QString, QString> _properties;

  QMap<QString, QString> _settings;
};


#endif // QNSTINSERT_H
