#include "gui.h"
#include <iostream>



Gui::Gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::GUI) {
    //importe l'interface depuis le fichier .gui
    ui->setupUi(this);


    this->setWindowTitle("Tableau de contrôle optique"); //Title of gui
    

    
    connect(ui->Bouton,&QPushButton::clicked,this,&Gui::hideIPCS);
    
}

Gui::~Gui(){
    delete ui;
}



Ui::GUI *Gui::getUI(){
    return ui;
}

void Gui::loadMaquette(MaquetteHandler *handler){
    //pour tous les signaux crée les vues
    for(LightSignal* signal : handler->getAllSignals()){
        short id = signal->getId();
        if(signalPositions.contains(id)){
            SignalView *view = new SignalView(signal,this);
            view->setAngle(signalPositions[id].angle);
            view->move(signalPositions[id].position);
        } else {
            qWarning() << "Error : no placement available for signal " << id;
        }
    }
   
    //pour toutes les aiguilles crée les vues
    for(Aiguille *aiguille : handler->getAllAiguilles()){
        short id = aiguille->getId();
        if(aiguillePositions.contains(id)){
            AiguilleView *view = new AiguilleView(aiguille,this);
            if(aiguillePositions[id].angle == 180){
                view->flipAiguille();
            }
            view->move(aiguillePositions[id].position);
        } else {
            qWarning() << "Error : no placement avaiable for aiguille " << id;
        }
    }

    int start = 10;

    //you should place the zoneView widget correctly if it's not definitive solution
    for(Zone *zone : handler->getAllZones()){
        QString name = zone->getName();
        //verification if position is available ??
        ZoneView *view = new ZoneView(zone,this);
        view->move(QPoint(start+=15,10));
    }
}




void Gui::resizeEvent(QResizeEvent *event) {
   //pretty much useless
}



void Gui::buttonPressed() {
    //pretty much useless
}

void Gui::hideIPCS(){
    const auto signalViews = this->findChildren<SignalView*>();
    for(SignalView *sigVw : signalViews){
        if(sigVw->isIPCSView()){
            sigVw->hide();
        }
    }
}
