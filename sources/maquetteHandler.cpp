#include "maquetteHandler.h"


MaquetteHandler::MaquetteHandler(QObject *parent)
    : QObject(parent)
{

    //VERY IMPORTANT, HERE IS THE INSTANCIATION OF ALL SIGNALS
    addSignalToMaquette(new LightSignal(1, SAVLR));
    addSignalToMaquette(new LightSignal(2, SAVLR));
    addSignalToMaquette(new LightSignal(3, CSAVLRRR));
    addSignalToMaquette(new LightSignal(4, CSAVLRRR));
    addSignalToMaquette(new LightSignal(5, CSAVLRR));
    addSignalToMaquette(new LightSignal(6, CSAVLRR));
    addSignalToMaquette(new LightSignal(7,SAVL));
    addSignalToMaquette(new LightSignal(8,SAVLR));
    addSignalToMaquette(new LightSignal(9,SAVLR));
    addSignalToMaquette(new LightSignal(10,CSAVLRRR));
    addSignalToMaquette(new LightSignal(11,CSAVLRRR));
    addSignalToMaquette(new LightSignal(12,CSAVLRR));
    addSignalToMaquette(new LightSignal(13,CSAVLRR));
    addSignalToMaquette(new LightSignal(15,SAVL));

    qDebug() << "\nEnd of signal adding\n";

    //connect all signals :
    if(connectSetup()){
        qDebug() << "All connections made";
    } else {
        qWarning() << "Error : invalid connexion";
    }

    INIT_VL();


    //connectSignalsById(1,2);
    //connectSignalsById(2,3);

    //lightSignals[1]->info();
    //lightSignals[2]->info();

    //VERY IMPORTANT, HERE IS THE INSTANCIATION OF ALL SWITCHES = AIGUILLES

}

QMap<int, LightSignal *> MaquetteHandler::getAllSignals(){
    return lightSignals;
}

bool MaquetteHandler::connectSignalsById(int previousId,int nextId){
    if(!lightSignals.contains(previousId)){
        qWarning() << "Error : Id"<<previousId<<"not found";
        return false;
    }
    if(!lightSignals.contains(nextId)){
        qWarning() << "Error : Id"<<nextId<<"not found";
        return false;
    }

    

    LightSignal* prev = lightSignals[previousId];
    LightSignal* next = lightSignals[nextId];
    prev->setNext(next);
    next->setPrevious(prev);
    return true;

}

bool MaquetteHandler::connectSetup(int setup){
    if(setup==1){
        qDebug() << "Setting up of all the connections :";
        
        //Voie 1
        return connectSignalsById(1,3) &&
        connectSignalsById(3,5) &&
        connectSignalsById(5,7) &&
        connectSignalsById(7,9) &&
        connectSignalsById(9,11) &&
        connectSignalsById(11,13) &&
        connectSignalsById(13,15) &&
        connectSignalsById(15,1) &&

        //Voie 2
        connectSignalsById(2,4) &&
        connectSignalsById(4,6) &&
        connectSignalsById(6,8) &&
        connectSignalsById(8,10) &&
        connectSignalsById(10,12) &&
        connectSignalsById(12,2);
    }
    if(setup == -1) {
        qDebug() << "TestSetup";
        return false;
    }
    return false;
}

void MaquetteHandler::INIT_VL(){
    foreach(LightSignal *signal, lightSignals){
        signal->setAspect(VL);
    }
}

void MaquetteHandler::processCommand(QString command){
    
    if(command == "/C1"){
        qDebug("VL");
        lightSignals[1]->setAspect(VL);
        emit sendCommand("VL\n");
    } else if(command == "/C2"){
        qDebug("A");
        emit sendCommand("S\n");
        lightSignals[1]->setAspect(S);
    } else if (command == "/C3"){
        qDebug("S");
        emit sendCommand("A\n");
        lightSignals[1]->setAspect(A);
        
    }
}

/* void MaquetteHandler::setUpOrder(){
    //here we set the different relationships beetween all the signals
} */

void MaquetteHandler::addSignalToMaquette(LightSignal *mySignal){
    //This method is to add the LightSignal instances to the  maquette
    short sigId = mySignal->getId();

    if(sigId<1){
        qWarning() << "Error : Incorrect Signal ID must be greater than 0";
    } else if(lightSignals.contains(sigId)){
        qWarning() << "Error : Signal with ID" << sigId << "Already exits";
    } else {
        lightSignals[mySignal->getId()] = mySignal;
    }
}
