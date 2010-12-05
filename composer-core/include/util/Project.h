#ifndef PROJECT_H
#define PROJECT_H

#include <QReadWriteLock>
#include <QString>
#include <QMap>
#include <QDir>
#include <QDebug>

#include <model/ncm/NclDocument.h>
using namespace ncm;
#include <model/functional/Document.h>
using namespace ncm::functional;

namespace composer {
namespace core {
namespace util {

class Project {
    private:
        QReadWriteLock lockString;
        QReadWriteLock lockDocuments;
        QString location; /*top dir of the project */
        QString projectId;
        QMap<QString, NclDocument*> nclDocuments;
        Document *documentFacade;
    public:
        Project(QString, QString);
        ~Project();
        inline QString getLocation() {
            QReadLocker locker(&lockString);
            return this->location;
        }

        inline QString getProjectId() {
            QReadLocker locker(&lockString);
            return this->projectId;
        }
        inline NclDocument* getNclDocument(QString documentId) {
                QReadLocker locker(&lockDocuments);
                return nclDocuments.contains(documentId) ?
                       nclDocuments.value(documentId) : NULL;
        }
        inline QMap<QString, NclDocument*> getNclDocuments() {
                QReadLocker locker(&lockDocuments);
                return nclDocuments;
        }
        bool addDocument(QString documentId, NclDocument*);
        bool removeDocument(QString documentId);
        bool deleteDocument(QString documentId);

};
}
}
}

#endif // PROJECT_H
