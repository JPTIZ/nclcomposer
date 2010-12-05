#include "../../include/modules/ProjectControl.h"

namespace composer {
namespace core {
namespace module {

    ProjectControl *ProjectControl::instance = NULL;

    ProjectControl::ProjectControl() {
        this->activeProject = NULL;
        this->documentParser = new DocumentParser();
    }

    ProjectControl::~ProjectControl() {
        QWriteLocker locker(&lockProjects);
        QMapIterator<QString,Project*> it(projects);
        while(it.hasNext()){
            it.next();
            Project *p = it.value();
            projects.remove(it.key());
            delete p;
            p = NULL;
        }
        projects.clear();
        delete documentParser;
        documentParser = NULL;
    }

    IModule* ProjectControl::getInstance(){
        QMutexLocker locker(&instMutex);
        if (!instance) {
            instance = new ProjectControl();
        }
        return instance;
    }

    void     ProjectControl::releaseInstance() {
        QMutexLocker locker(&instMutex);
        if (instance != NULL) {
            delete instance;
            instance = NULL;
        }
    }

    bool ProjectControl::openProject (QString projectId, QString location) {
        //TODO - fazer parser do arquivo de projeto
    }

    Project* ProjectControl::createProject(QString projectId, QString location) {
        lockProjects.lockForRead();
        if (!projects.contains(projectId)) {
            lockProjects.unlock();
            Project *p = new Project(projectId,location);
            lockProjects.lockForWrite();
            projects[projectId] = p;
            lockProjects.unlock();
            return p;
        }
        lockProjects.unlock();
        return NULL;
    }

    bool ProjectControl::activateProject (QString projectId){
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            lockProjects.unlock();
            lockProjects.lockForWrite();
            this->activeProject = projects[projectId];
            lockProjects.unlock();
            return true;
        }
        lockProjects.unlock();
        return false;
    }
    bool ProjectControl::deactivateProject (QString projectId){
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            lockProjects.unlock();
            lockProjects.lockForWrite();
            this->activeProject = NULL;
            lockProjects.unlock();
            return true;
        }
        lockProjects.unlock();
        return false;
    }

    Project *ProjectControl::getProject(QString projectId) {
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            Project *ret = projects[projectId];
            lockProjects.unlock();
            return ret;
        }
        lockProjects.unlock();
        return NULL;
    }

    bool ProjectControl::saveProject (QString projectId, QString location) {
        //TODO - salvar arquivo de projeto e chamar save para cada NCLDocument
    }

    bool ProjectControl::saveAllProjects() {
        QWriteLocker locker(&lockProjects);
        QMapIterator<QString,Project*> it(projects);
    #ifdef Q_WS_MAC
        QSettings settings("telemidia.pucrio.br", "composer");
     #else
        QSettings settings("telemidia", "composer");
     #endif
        settings.beginGroup("projects");
        while(it.hasNext()){
            it.next();
            Project *p = it.value();
            settings.setValue(p->getProjectId(),p->getLocation());
            qDebug() << "SAVEALL projectId: " << p->getProjectId() <<
                        "location: " << p->getLocation();
            //TODO salvar alterações no NCL
        }
        settings.endGroup();
        settings.sync();
    }

    bool ProjectControl::closeProject (QString projectId) {
        //TODO - salvar arquivo de projeto e chamar save para cada NCLDocument
    }
    bool ProjectControl::deleteProject (QString projectId) {
        //TODO - deletar todos os NCLDocument e deletar o arquivo de projeto
    }

    bool ProjectControl::addDocument (QString projectId, QString uri,
                                      QString  documentId, bool copy) {
        Project *p = getProject(projectId);
        QString projectLocation = p->getLocation();
        if (copy) {
            QString destUri = projectLocation+documentId;
            if (documentId.endsWith(".ncl")) destUri += ".ncl";

            if (!QFile::copy(uri,destUri)) {
               qDebug() << tr("addDocument fails on copy the document");
               return false;
           }
           uri = destUri;
        }
        if (documentParser->setUpParser(uri)) {
            if (documentParser->parseDocument()) {
              //TODO - verify the copy parameter to addDocument in Project
              if (p->addDocument(documentId,
                                 documentParser->getNclDocument())) {
                return true;
              } else {
                qDebug() << tr("addDocument fails to addDocument to project");
                return false;
              }
            } else {
                qDebug() << tr("addDocument fails in parseDocument");
                return false;
              }
            } else {
                qDebug() << tr("addDocument fails in setUpParser");
                return false;
            }
        return false;
    }

    bool ProjectControl::removeDocument (QString projectId,
                                         QString documentId) {
        //TODO - deletar o NclDocument do projeto mas não o arquivo
    }
    bool ProjectControl::deleteDocument (QString projectId, QString documentId) {
        //TODO - deletar o NclDocument do Projeto e deletar o arquivo do sistema
    }

    bool ProjectControl::saveDocument (QString projectId, QString documentId,
                                       QString location) {
        //TODO - salvar o documento Ncl
    }

}//fim
}//fim
}//fim namespace
