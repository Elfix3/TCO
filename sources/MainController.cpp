#include "MainController.h"
#include <iostream>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
    std::cout<<"<----Controller created---->\n"<<std::endl;
    
    
    //QMainWindows
    //mySerialHandler = new SerialHandler(this);
    std::cout<<"<----Serial Handler---->\n"<<std::endl;
    mySerialHandler = new SerialHandler();
    mySerialHandler->INIT();
    
    myGui = new Gui();
    //Handlers
    
    std::cout<<"\n\n<----Maquette Handler---->\n"<<std::endl;
    myMaquetteHandler = new MaquetteHandler(this);
    myMaquetteHandler->INIT();


    
    
    //myMaquetteHandler->getAllAiguilles()[3]->setDirection(DROITE);
    //myGui = new Gui(); 
    //load la GUI ici 
    myGui->loadMaquette(myMaquetteHandler);
    //myGui->loadMaquette(myMaquetteHandler);
    //myControl->loadMaquette(myMaquetteHandler);

    
    //Main controller has the purpose to link all the class between them with the slot signal system
    //All the connection explained below :

    //Connection of the data recieived signal with the zoneUpdateFromSensor from maquette handler
    //connect(mySerialHandler, &SerialHandler::dataReceived, myMaquetteHandler, &MaquetteHandler::zoneUpdateFromSensor);


    //connects the sensor command reception with the zone update
    connect(mySerialHandler,&SerialHandler::commandReady,myMaquetteHandler,&MaquetteHandler::zoneUpdateFromSensor);
    
    //connects the object update with the command sending
    connect(myMaquetteHandler,&MaquetteHandler::aiguilleChanged,mySerialHandler,&SerialHandler::sendCommandAiguille);
    connect(myMaquetteHandler,&MaquetteHandler::signalChanged,mySerialHandler, &SerialHandler::sendCommandSignal);
    connect(myMaquetteHandler,&MaquetteHandler::zoneChanged, mySerialHandler,&SerialHandler::sendCommandZone);


    //myMaquetteHandler->getAllSignals()[1]->setAspect(VL);
    //myMaquetteHandler->getAllSignals()[2]->setAspect(S);
    
}


MainController::~MainController(){
    delete myControl;
    delete myGui;
    delete myMaquetteHandler;
    delete mySerialHandler;
}



void MainController::showGui(){
    myGui->show();
}

void MainController::showControl(){
    myControl->show();
}




