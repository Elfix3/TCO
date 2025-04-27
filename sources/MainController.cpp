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

    mySerialHandler->openSerial("COM3");
    //dumbTestSetup();
    /* LightSignal *sig1 = myMaquetteHandler->getAllSignals().value(1); 
    SignalView *sig1v = new SignalView(sig1,myGui);
    sig1v->move(600,600);
    sig1v->setAngle(45); */

    //Creation of all the views
    

    /* for(auto it = myMaquetteHandler->getAllSignals().begin(); it != myMaquetteHandler->getAllSignals().end();it++){
        myGui->set
    }
 */
    


    // Connectez le bouton de myGui au slot manageButton
    
    
    bool connection = true;
    if(connection){
        /* connect(myGui->getUI()->myButton, &QPushButton::clicked,  // Supposons que getButton() retourne le QPushButton
            this, &MainController::manageButton); */

        //myMaquetteHandler->getAllSignals().value(1)->info();
        connect(mySerialHandler, &SerialHandler::dataReceived, myMaquetteHandler, &MaquetteHandler::processCommand);

        connect(myMaquetteHandler, &MaquetteHandler::sendCommand,
        mySerialHandler, &SerialHandler::writeData);


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
    buttonState = !buttonState;
    if(buttonState){
        mySerialHandler->writeData("ON\n");
    } else {
        mySerialHandler->writeData("OFF\n");
    }
}

void MainController::showGui(){
    myGui->show();
}

void MainController::showControl(){
    myControl->show();
}

void MainController::dumbTestSetup(){
    LightSignal *signalTest1 = new LightSignal(99,SAVL);
    SignalView *signalViewTest1 = new SignalView(signalTest1,myGui);
    signalViewTest1->move(200,200);

    LightSignal *signalTest2 = new LightSignal(999,SAVLR);
    SignalView *signalViewTest2 = new SignalView(signalTest2,myGui);
    signalViewTest2->move(300,300);

    LightSignal *signalTest3 = new LightSignal(9999,CSAVLRR);
    SignalView *signalViewTest3 = new SignalView(signalTest3,myGui);
    signalViewTest3->move(400,400);

    LightSignal *signalTest4 = new LightSignal(99999,CSAVLRRR);
    SignalView *signalViewTest4 = new SignalView(signalTest4,myGui);
    signalViewTest4->move(500,500);
    signalViewTest4->setAngle(120);
    signalTest1->setAspect(RR);
    signalTest2->setAspect(RR);
    signalTest3->setAspect(RR);
    signalTest4->setAspect(RR);
}

void MainController::initSerial(){

}


