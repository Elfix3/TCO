#include "MainController.h"
#include <iostream>

MainController::MainController(QObject *parent)
    : QObject(parent)
{
    std::cout<<"<----Controller created---->\n"<<std::endl;
    myGui = new Gui();
    myControl = new Control();
    myMaquetteHandler = new MaquetteHandler(this);
    mySerialHandler = new SerialHandler(this);

    //mySerialHandler->openSerial("COM5");
    myGui->loadMaquette(myMaquetteHandler);

   

    
    //Main controller has the purpose to link all the class between them with the slot signal system
    //All the connection explained below :

    //Connection of the data recieived signal with the zoneUpdateFromSensor from maquette handler
    connect(mySerialHandler, &SerialHandler::dataReceived, myMaquetteHandler, &MaquetteHandler::zoneUpdateFromSensor);

    //Connection of the 

    //
    connect(myMaquetteHandler, &MaquetteHandler::sendCommand,mySerialHandler, &SerialHandler::writeData);
        

        
    
}

MainController::~MainController(){
    delete myControl;
    delete myGui;
    delete myMaquetteHandler;
    delete mySerialHandler;
}

void MainController::manageButton(){
    //ON/OFF behavior for a button on arduino internal LED
    
}

void MainController::RESET(){

}

void MainController::showGui(){
    myGui->show();
    QMessageBox::StandardButton reply;
    /* reply = QMessageBox::question(myGui, "Confirmation", "Voulez-vous continuer ?",
        QMessageBox::Yes | QMessageBox::No); */
}

void MainController::showControl(){
    myControl->show();
}

void MainController::dumbTestSetup(){
}




