/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "modules/ProjectControl.h"
#include "modules/ProjectReader.h"

namespace composer {
    namespace core {

INIT_SINGLETON (ProjectControl)

ProjectControl::ProjectControl()
{
}

ProjectControl::~ProjectControl()
{
    QMap<QString,Project*>::iterator it;
    PluginControl *pg = PluginControl::getInstance();
    for (it = openProjects.begin(); it != openProjects.end(); it++)
    {
        Project *project = it.value();
        if (pg->releasePlugins(project))
        {
            delete project;
            project = NULL;
        } else {
            qWarning() << "Error: Failed to releasePlugins ";
        }
    }
}

bool ProjectControl::closeProject(QString location)
{
    if (!openProjects.contains(location)) return false;

    Project *project = openProjects[location];
    if (PluginControl::getInstance()->releasePlugins(project))
    {
        delete project;
        project = NULL;
        openProjects.remove(location);
    }
    else
    {
        qWarning() << "Error: Failed to close the project :" << location;
        return false;
    }
    return true;
}

void ProjectControl::launchProject(QString location)
{
    if (openProjects.contains(location))
    {
        emit projectAlreadyOpen(location);
        return;
    }

    QString ext = location;
    ext = ext.remove(0, ext.lastIndexOf(".") + 1);
    LanguageType type = Utilities::getLanguageTypeByExtension( ext );

    if(type == NONE)
    {
        //\todo TEST ON WINDOWS
//        QProcess spaw;
//        QStringList args;
//        args.append(location);
//#ifdef Q_WS_MAC
//        spaw.startDetached("/usr/bin/open", args);
//#else
//        spaw.startDetached("/usr/bin/gnome-open", args);
//#endif
        return;
    }

    /* Requests the LanguageProfile associated with this DocumentType */
    ILanguageProfile *profile = LanguageControl::getInstance()->
                                    getProfileFromType(type);

    if (!profile)
    {
        emit notifyError(tr("No Language Profile Extension "
                            "found for (%1)").
                         arg(ext.toUpper()));
        return;
    }

    emit startOpenProject(location);
    QMap<QString,QString> atts;
    QString documentId = location;
    documentId.remove(0, location.lastIndexOf(QDir::separator())+1);
    atts["id"] = documentId;

    ProjectReader pr;
    Project *project = pr.readFile(location);
    project->setAtrributes(atts);

    if(project != NULL)
    {
        // The project was readed without a problem.
        project->setLocation(location);
        project->setProjectType(type);

        PluginControl::getInstance()->launchProject(project);
        openProjects[location] = project;

        connect ( project, SIGNAL(dirtyProject(bool)),
                  this, SLOT(projectIsDirty(bool)));
    }
    else
        qWarning() << tr("Project could not be open!");

    project->setDirty(false);

    emit endOpenProject(location);
}

void ProjectControl::importFromDocument( QString docLocation,
                                         QString projLocation)
{
    if (openProjects.contains(projLocation))
    {
        emit projectAlreadyOpen(projLocation);
        return;
    }

    QString ext = docLocation;
    ext = ext.remove(0, ext.lastIndexOf(".") + 1);
    LanguageType type = Utilities::getLanguageTypeByExtension( ext );

    if(type == NONE)
    {
        qWarning() << "File format not recognized.";
        return;
    }

    /* Requests the LanguageProfile associated with this DocumentType */
    ILanguageProfile *profile = LanguageControl::getInstance()->
                                    getProfileFromType(type);

    if (!profile)
    {
        emit notifyError(tr("No Language Profile Extension "
                            "found for (%1)").
                         arg(ext.toUpper()));
        return;
    }

    emit startOpenProject(projLocation);

    QMap<QString,QString> atts;
    QString projectId = projLocation;
    projectId.remove(0, projLocation.lastIndexOf(QDir::separator())+1);
    atts["id"] = projectId;

    ProjectReader pr;
    Project *project = pr.readFile(projLocation);
    project->setAtrributes(atts);

    if(project != NULL)
    {
        // The project was read without any problem.
        project->setLocation(projLocation);
        project->setProjectType(type);

        PluginControl::getInstance()->launchProject(project);
        project->setLocation(projLocation);

        openProjects[projLocation] = project;

        IDocumentParser *parser;
        parser = profile->createParser(project);
        PluginControl::getInstance()->
                connectParser(parser, PluginControl::getInstance()->
                                               getMessageControl(projLocation));

        QFile input(docLocation);
        if(input.open(QIODevice::ReadOnly))
            parser->parseContent(input.readAll());
        else
            qWarning() << tr("File could not be open!");

        input.close();
    }
    else
        qWarning() << tr("Project could not be open!");

    project->setDirty(false);
    emit endOpenProject(projLocation);
}

void ProjectControl::saveProject(QString location)
{
    Project *project = openProjects.value(location);
    QFile fout(location);

    qDebug() << "Trying to save: " << location;
    if(!fout.exists())
    {
        qDebug() << "The file (" << location << ") doesn't exists. It will be\
                    created.";
    }

    if( !fout.open( QIODevice::WriteOnly ) )
    {
       // It could not open
       qWarning() << "Failed to open file (" <<  location << ") for writing.";
       return;
    }

    QString content = project->toString();
    fout.write(qCompress(content.toAscii(), content.size()));
    fout.close();
    project->setDirty(false);
}

Project *ProjectControl::getOpenProject(QString location)
{
    if(openProjects.contains(location))
        return openProjects.value(location);

    return NULL;
}

void ProjectControl::projectIsDirty(bool isDirty)
{
    Project *project = qobject_cast<Project *> (QObject::sender());
    if(project != NULL)
    {
        emit dirtyProject(project->getLocation(), isDirty);
    }
    else qWarning() << "Received a dirtyProject message for a NULL project";

}

} }//end namespace

