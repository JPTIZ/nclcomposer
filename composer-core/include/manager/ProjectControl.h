#ifndef PROJECTCONTROL_H
#define PROJECTCONTROL_H

#include <QtDebug>
#include <QReadWriteLock>
#include <QtDebug>

#include <QObject>

#include <QMap>
#include <QString>

#include "../util/Project.h"
#include "../util/DocumentParser.h"


namespace manager {

    class ProjectControl : public QObject{
        Q_OBJECT
        private:
            QReadWriteLock lockProjects;
            Project *activeProject;
            QMap<QString,Project*> projects;
            DocumentParser *documentParser;

        public:
            ProjectControl();
            ~ProjectControl();
            /** Abre uma base privada existente localizada pelo parâmetro
                location. Se a base privada não existir ou se o parâmetro
                location não for informado, uma nova base é criada com o
                identificador baseId. */
             bool openProject (QString projectId, QString location);
             bool activateProject (QString projectId);
             bool deactivateProject (QString projectId);
             bool deleteProject (QString projectId);
             Project* createProject(QString projectId, QString location);
             /** Salva todo o conteúdo da base privada em um dispositivo de
                armazenamento persistente */
             bool saveProject (QString projectId, QString location);
             /** Fecha a base privada e descarta todo o conteúdo da base
                privada */
             bool closeProject (QString projectId);
             bool addDocument (QString projectId, QString uri, QString  documentId);
             bool removeDocument (QString projectId, QString documentId);
             bool deleteDocument (QString projectId, QString documentId);
             bool saveDocument (QString projectId, QString documentId,
                                QString location);
             Project *getProject(QString projectId);
             inline Project *getCurrentProject() {return this->activeProject;}
         signals:
             void newActivateProject(Project *);
    };
}
#endif // PROJECTCONTROL_H

