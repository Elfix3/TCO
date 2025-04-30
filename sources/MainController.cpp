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

    mySerialHandler->openSerial("COM5");
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
}

void MainController::showControl(){
    myControl->show();
}

void MainController::dumbTestSetup(){
    LightSignal *signalTest1 = new LightSignal(1,SAVL);
    SignalView *signalViewTest1 = new SignalView(signalTest1,myGui);
    signalViewTest1->setAngle(270);
    signalViewTest1->move(350,135);

    LightSignal *signalTest2 = new LightSignal(2,SAVLR);
    SignalView *signalViewTest2 = new SignalView(signalTest2,myGui);
    signalViewTest2->setAngle(48);
    signalViewTest2->move(55,55);

    LightSignal *signalTest3 = new LightSignal(3,CSAVLRR);
    SignalView *signalViewTest3 = new SignalView(signalTest3,myGui);
    signalViewTest3->setAngle(90);
    signalViewTest3->move(800,0);


    LightSignal *signalTest4 = new LightSignal(4,CSAVLRRR);
    SignalView *signalViewTest4 = new SignalView(signalTest4,myGui);
    signalViewTest4->setAngle(90);
    signalViewTest4->move(320,0);

    //signalTest2->setAspect(VL);
    //signalTest3->setAspect(C);
    //signalTest4->setAspect(S);

    LightSignal *signalTest1IPCS = new LightSignal(111,SAVL);
    //SignalView *signalViewTest1IPCS = new SignalView(signalTest1IPCS,myGui);
    //signalViewTest1IPCS->move(0,0);

    LightSignal *signalTest2IPCS = new LightSignal(222,SAVLR);
    SignalView *signalViewTest2IPCS = new SignalView(signalTest2IPCS,myGui);
    signalViewTest2IPCS->setAngle(90);
    signalViewTest2IPCS->move(320,150);

    LightSignal *signalTest3IPCS = new LightSignal(333,CSAVLRR);
    SignalView *signalViewTest3IPCS = new SignalView(signalTest3IPCS,myGui);
    signalViewTest3IPCS->setAngle(270);
    signalViewTest3IPCS->move(280,0);

    LightSignal *signalTest4IPCS = new LightSignal(444,CSAVLRRR);
    SignalView *signalViewTest4IPCS = new SignalView(signalTest4IPCS,myGui);
    signalViewTest4IPCS->setAngle(270);
    signalViewTest4IPCS->move(820,0);


    signalTest2->setAspect(VL);
    signalTest4->setAspect(A);
    signalTest3->setAspect(S);

    signalTest4IPCS->setAspect(C);
    signalTest3IPCS->setAspect(C);

    signalTest1->setAspect(VL);
    signalTest2IPCS->setAspect(S);

    //signalViewTest2->hide();
}




