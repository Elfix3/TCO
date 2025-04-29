#include "gui.h"
#include <iostream>



Gui::Gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::GUI) {
    //importe l'interface depuis le fichier .gui
    ui->setupUi(this);


    this->setWindowTitle("Tableau de commande optique"); //Title of gui
    

    
}

Gui::~Gui(){
    delete ui;
}



Ui::GUI *Gui::getUI(){
    return ui;
}

void Gui::loadMaquette(MaquetteHandler *handler){
    
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
            view->move(aiguillePositions[id].position);
        } else {
            qWarning() << "Error : no placement avaiable for aiguille " << id;
        }
    }
}




void Gui::resizeEvent(QResizeEvent *event) {
   //pretty much useless
}



void Gui::buttonPressed() {
    //pretty much useless
}



