#include "../../include/util/CoreManager.h"


namespace composer {
namespace core {
namespace util {

    CoreManager::CoreManager(QObject *parent) :
        QObject(parent) {

        connect(ProjectControl::getInstance(),
                SIGNAL(documentCreatedAndParsed(QString,QString)),
                SIGNAL(documentAdded(QString,QString)));
        connect(ProjectControl::getInstance(),
                SIGNAL(errorNotify(QString)),
                SIGNAL(onError(QString)));
    }

    CoreManager::~CoreManager() {

    }

    void CoreManager::addProject(QString projectId,QString location) {
        ProjectControl *projectControl = ProjectControl::getInstance();

        qDebug() << "CoreManager::addProject(" << projectId << ", "
                 << location << ")";

        QDir dir(location + QDir::separator() + projectId);

        if (dir.exists()) {
            projectControl->createProject(projectId,location);
            emit projectCreated(projectId,location);
            //TODO - fazer o filtro certinho
            //QStringList filters;
            //filters << "*.ncl";
            //dir.setNameFilters(filters);
            dir.setFilter(QDir::Files | QDir::AllDirs
                          | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            dir.setSorting(QDir::DirsFirst | QDir::Name);
            addFilesInDirectory(dir,projectId);
        } else { //dir dont exists
            emit onError(tr("Project %1 could not be open,"
                            "maybe the path (%2) has been changed")
                         .arg(projectId).arg(location));
        }
    }

    void CoreManager::addFilesInDirectory(QDir dir, QString projectId) {
        QFileInfoList list = dir.entryInfoList();

        qDebug() << "CoreManager::addFilesInDirectory(" <<
                    dir << ", " << projectId;

        for (int i = 0; i < list.size(); ++i) {
           QFileInfo fileInfo = list.at(i);
           qDebug() << "CoreManager::addFilesInDirectory parsing file: " <<
                        fileInfo.fileName();
           if (fileInfo.isFile()) {
               QString fileName = fileInfo.fileName();
               QString filePath = fileInfo.absoluteFilePath();
               if (fileName.endsWith(".ncl")) {
                   qDebug() << "CoreManager::addFilesInDirectory " <<
                                "Adding NCL File: " << fileName <<
                                "in: " << projectId;
                   addDocument(fileName,filePath,projectId,false);
               }
           } else if (fileInfo.isDir()) {
               //TODO adicionar diretorio no projectTree
               //TODO testar emitindo sinal de projetoCriado
               addFilesInDirectory(fileInfo.dir(),projectId);
           }

         }
    }

    void CoreManager::createProject(QString name, QString location){
        ProjectControl *projectControl = ProjectControl::getInstance();

        qDebug() << "CoreManager::createProject(" << name << ", "
                 << location << ")";

        QDir dir(location);
        QDir dirEx(location + QDir::separator() + name);

        if (!dir.exists()) {
            if ( QMessageBox::question((QWidget*)this->parent(),
                                   tr("The directory does not exists"),
                                   tr("Do you want to create ?"))
                == QMessageBox::No) {
                emit onError(
                     tr("Directory for this project could not be created"));
                return;
            }
        }

        if(dirEx.exists()) {
            //TODO - asks to create the project anyway
            emit onError(tr("A directory with this name already exists"));
            return;
        }
        if(dir.mkdir(name)) {
            projectControl->createProject(name,location);
            emit projectCreated(name,location);
        } else {
            emit onError(tr("Directory for this project could not be created"));
        }
    }

    void CoreManager::addDocument(QString name,QString location,
                                  QString projectId, bool copy) {

        ProjectControl *projectControl = ProjectControl::getInstance();

        if(!projectControl->addDocument(projectId,location,name,copy)) {
            emit onError(tr("Could not add the choosen NCL document"));
        }
    }


    bool CoreManager::saveSettings() {
        ProjectControl *projectControl = ProjectControl::getInstance();

        if (!projectControl->saveAllProjects()) {
            emit onError(tr("Could not save the current projects"));
            return false;
        }
        return true;
    }

}
}
}
