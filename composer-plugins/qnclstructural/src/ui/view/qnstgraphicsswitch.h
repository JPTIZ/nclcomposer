#ifndef QNSTGRAPHICSSWITCH_H
#define QNSTGRAPHICSSWITCH_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicshtml.h"
#include "qnstgraphicsncl.h"
#include "qnstgraphicsswitchport.h"

class QnstGraphicsSwitch : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsSwitch(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsSwitch();

public slots:
    void performHtml();

    void performNCL();

    void performSwitchPort();

    void performImage();

    void performAudio();

    void performText();

    void performVideo();

    void performScript();

    void performSettings();

    void performMedia();

    void performContext();

    void performSwitch();

    void performArea();

    void performProperty();

private:
    void createObjects();

    void createConnections();

    QString dropsrc;
};


#endif // QNSTGRAPHICSSWITCH_H
