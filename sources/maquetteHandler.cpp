#include "maquetteHandler.h"


MaquetteHandler::MaquetteHandler(QObject *parent)
    : QObject(parent)
{
    SETUP_AIGUILLES();
    SETUP_SIGNALS();
    
    aiguilles[1]->setDirection(GAUCHE);
    aiguilles[2]->setDirection(GAUCHE);
}

QMap<int, LightSignal *> MaquetteHandler::getAllSignals(){
    return lightSignals;
}

QMap<int, Aiguille *> MaquetteHandler::getAllAiguilles(){
    return aiguilles;
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

/* void MaquetteHandler::INIT_VL(){
    foreach(LightSignal *signal, lightSignals){
        signal->setAspect(VL);
    }
} */

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

        qDebug() << "Signal"<<sigId<< "with type"<<mySignal->toString(mySignal->getType()).c_str()<<"added to the maquette";
        lightSignals[mySignal->getId()] = mySignal;
    }
}

void MaquetteHandler::addAiguilleToMaquette(Aiguille *myAiguille){
    short aigId = myAiguille->getId();
    //Direction dir = myAiguille->getDirection();
    if(aigId<1){
        qWarning() << "Error : Incorrect aiguille ID must be greater than 0";
    } else if(aiguilles.contains(aigId)){
        qWarning() << "Error : Aiguille with ID" << aigId << "Already exits";
    } else {
        qDebug() << "Aiguille" << aigId << "added to the maquette";
        aiguilles[myAiguille->getId()] = myAiguille;
    }

}

void MaquetteHandler::SETUP_SIGNALS(){
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
}

void MaquetteHandler::SETUP_AIGUILLES(){
    //VERY IMPORTANT, HERE IS THE INSTANCIATION OF ALL AIGUILLES
    addAiguilleToMaquette(new Aiguille(1));
    addAiguilleToMaquette(new Aiguille(2));
    addAiguilleToMaquette(new Aiguille(3));
    addAiguilleToMaquette(new Aiguille(4));
    addAiguilleToMaquette(new Aiguille(5));
    addAiguilleToMaquette(new Aiguille(6));

}