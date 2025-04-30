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

   

    //dumbTestSetup();

    
    
    bool connection = true;
    if(connection){
        /* connect(myGui->getUI()->myButton, &QPushButton::clicked,  // Supposons que getButton() retourne le QPushButton
            this, &MainController::manageButton); */

        //myMaquetteHandler->getAllSignals().value(1)->info();
        connect(mySerialHandler, &SerialHandler::dataReceived, myMaquetteHandler, &MaquetteHandler::processCommand);

        //Connect
        connect(myMaquetteHandler, &MaquetteHandler::sendCommand,mySerialHandler, &SerialHandler::writeData);
        

        //connect(mySerialHandler, &SerialHandler::dataReceived, 
        //myControl, &Control::updateComboBox);
    }
    
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
    reply = QMessageBox::question(myGui, "Confirmation", "Voulez-vous continuer ?",
        QMessageBox::Yes | QMessageBox::No);
}

void MainController::showControl(){
    myControl->show();
}

void MainController::dumbTestSetup(){
}




