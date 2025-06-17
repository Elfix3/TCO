#include "MainController.h"
#include "zoneView.h"
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
    myControl = new Control();
    //Handlers
    
    std::cout<<"\n\n<----Maquette Handler---->\n"<<std::endl;
    myMaquetteHandler = new MaquetteHandler(this);
    myMaquetteHandler->INIT();


    myGui->loadMaquette(myMaquetteHandler);
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

    //connects the object update with the control elements
    connect(myMaquetteHandler,&MaquetteHandler::aiguilleChanged,myControl,&Control::updateAiguilleOnControl);
    connect(myMaquetteHandler,&MaquetteHandler::signalChanged,myControl,&Control::updateSignalOnControl);
    connect(myMaquetteHandler,&MaquetteHandler::zoneChanged,myControl,&Control::updateZoneOnControl);
    
    connect(myControl,&Control::BALisDisabled,myMaquetteHandler,&MaquetteHandler::disableBAL);
    connect(myControl,&Control::BALisEnabled,myMaquetteHandler,&MaquetteHandler::enableBAL);
    connect(myMaquetteHandler,&MaquetteHandler::initBALstatus,myControl,&Control::setUpBALstatus);
    //detects when the change comes from the user
   

    connect(myControl,&Control::signalChangedFromControl,myMaquetteHandler,&MaquetteHandler::updateSignalFromCombo); //normal names ???
    connect(myControl,&Control::zoneChangedFromControl,myMaquetteHandler,&MaquetteHandler::updateZoneFromRadioButton);
    //aiguilles with popup?
    /* myMaquetteHandler->getAllSignals()[1]->setAspect(A);
    myMaquetteHandler->getAllSignals()[2]->setAspect(S);
    myMaquetteHandler->getAllZones()["5A"]->setState(true);
    myMaquetteHandler->getAllZones()["8B"]->setState(true);

    myMaquetteHandler->getAllZones()["8B"]->setState(false); */
    
    myMaquetteHandler->updateAll();
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




