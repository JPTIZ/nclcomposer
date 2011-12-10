/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "tst_ModuleInit.h"
#include "tst_ModuleLanguage.h"
#include "tst_ModuleProject.h"
#include "tst_ModulePlugin.h"

int run_ModuleInit()
{
    tst_ModuleInit testModuleInit;
    testModuleInit.setInterations(10240);
    testModuleInit.setBenchmark(true);
    QStringList args;
    args.append("-v2");
    return QTest::qExec(&testModuleInit,args);
}

int run_ModuleLanguage()
{
    tst_ModuleLanguage testModuleLanguage;
    testModuleLanguage.setBenchmark(true);
    QStringList args;
    args.append("-v2");
    return QTest::qExec(&testModuleLanguage, args);
}

int run_ModuleProject()
{
    //run_ModuleInit();
    run_ModuleLanguage();
}

void myMessageOutput(QtMsgType type, const char *msg)
 {
     /*switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }*/
 }


int main(int argc, char *argv[])
{
  qInstallMsgHandler(myMessageOutput);
  QApplication app(argc, argv);
  run_ModuleProject();
}

// QTEST_MAIN(tst_ModuleProject)
//QTEST_MAIN(tst_ModulePlugin)
