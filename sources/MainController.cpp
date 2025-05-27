#include "MainController.h"
#include "zoneView.h"


MainController::MainController(QObject *parent)
    : QObject(parent)
{
    qInfo() << "\n"
           ".-------------------------------------------.\n"
           "|                  APP START                |\n"
           "'-------------------------------------------'\n";
    
    //creation of all my elements
    mySerialHandler = new SerialHandler(this);
    myMaquetteHandler = new MaquetteHandler(this);
    myControl = new Control();
    myGui = new Gui();

    qInfo() << "############################################\n";
    if(!mySerialHandler->INIT()) qFatal("\033[1;31m\nError: Arduino missing end of the program\033[0m");
    qInfo() << "############################################\n";
    
    myMaquetteHandler->INIT();
    
    
    //QMainWindows
    
    //mySerialHandler->INIT();
    
    //Handlers
    
    //Info()<< "\n\n<----Maquette Handler---->\n";


    //myGui->loadMaquette(myMaquetteHandler);
    //myControl->loadMaquette(myMaquetteHandler);

    
    //Main controller has the purpose to link all the class between them with the slot signal system
    //All the connection explained below :

    //Connection of the data recieived signal with the zoneUpdateFromSensor from maquette handler
    //connect(mySerialHandler, &SerialHandler::dataReceived, myMaquetteHandler, &MaquetteHandler::zoneUpdateFromSensor);


    //connects the sensor command reception with the zone update
    //connect(mySerialHandler,&SerialHandler::commandReady,myMaquetteHandler,&MaquetteHandler::updateTrainPosition);



    
    //connects the object update with the command sending
    connect(myMaquetteHandler,&MaquetteHandler::aiguilleChanged,mySerialHandler,&SerialHandler::sendCommandAiguille); //not necessary in my opinion
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
    connect(myControl,&Control::aiguilleChangedFromControl,myMaquetteHandler,&MaquetteHandler::updateAiguilleFromRadioButton);
    connect(myControl,&Control::sendAiguilleImpulse,mySerialHandler,&SerialHandler::sendCommandAiguille);


    
    //myMaquetteHandler->emitAllStates(); //alows the sync between Control, MaquetteHandler and the actual maquette
}


MainController::~MainController(){
    delete myControl;
    delete myGui;
    delete myMaquetteHandler;
    mySerialHandler->closeSerial();
    delete mySerialHandler;

}



void MainController::showGui(){
    myGui->show();
}

void MainController::showControl(){
    myControl->show();
}




