#include "maquetteHandler.h"
#include <QMessageBox>

MaquetteHandler::MaquetteHandler(QObject *parent)
    : QObject(parent)
{
    SETUP_AIGUILLES();
    SETUP_SIGNALS();
    SET_ALL_VL();

    //very important, sets up all the connections for my maquette
    connectSetup();

    //SET_ALL_DIR_RIGHT();
}

QMap<int, LightSignal *> MaquetteHandler::getAllSignals(){
    return lightSignals;
}

QMap<int, Aiguille *> MaquetteHandler::getAllAiguilles(){
    return aiguilles;
}

bool MaquetteHandler::connectSignalsById(int previousId,int nextId){
    if(!lightSignals.contains(previousId)){
        qWarning() << "Error : signal with Id"<<previousId<<"not found";
        return false;
    }
    if(!lightSignals.contains(nextId)){
        qWarning() << "Error : signal with Id"<<nextId<<"not found";
        return false;
    }
    LightSignal* prev = lightSignals[previousId];
    LightSignal* next = lightSignals[nextId];
    prev->setNext(next);
    next->setPrevious(prev);
    return true;

}

bool MaquetteHandler::connectAiguilleConj(int aigId, int conjId)
{
    if(!aiguilles.contains(aigId)){
        qWarning() << "Error : aiguille with Id"<<aigId<<"not found";
        return false;
    }
    if(!aiguilles.contains(conjId)){
        qWarning() << "Error : aiguille with Id"<<conjId<<"not found";
        return false;
    }
    Aiguille *aig = aiguilles[aigId];
    Aiguille *conj = aiguilles[conjId];
    aig->setConj(conj);
    conj->setConj(aig);
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
        connectSignalsById(12,2) &&
    
        connectAiguilleConj(1,2);
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
    //réception de la donnée du capteur
    if(command == "/C1"){
        
        lightSignals[1]->setAspect(S);
        lightSignals[1]->getPrevious()->setAspect(A);
        lightSignals[1]->getPrevious()->getPrevious()->setAspect(VL);

        
       
        

    } else if(command == "/C2"){
        lightSignals[3]->setAspect(S);
        lightSignals[3]->getPrevious()->setAspect(A);
        lightSignals[3]->getPrevious()->getPrevious()->setAspect(VL);
        
        aiguilles[1]->setDirection(GAUCHE);
        aiguilles[1]->getConjAiguille()->setDirection(GAUCHE);
        
    } else if (command == "/C3"){
        lightSignals[5]->setAspect(S);
        lightSignals[5]->getPrevious()->setAspect(A);
        lightSignals[5]->getPrevious()->getPrevious()->setAspect(VL);

        aiguilles[1]->setDirection(DROITE);
        aiguilles[1]->getConjAiguille()->setDirection(DROITE);
        
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
    addSignalToMaquette(new LightSignal(1, SAVLR,this));
    addSignalToMaquette(new LightSignal(2, SAVLR,this));
    addSignalToMaquette(new LightSignal(3, CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(4, CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(5, CSAVLRR,this));
    addSignalToMaquette(new LightSignal(6, CSAVLRR,this));
    addSignalToMaquette(new LightSignal(7,SAVL,this));
    addSignalToMaquette(new LightSignal(8,SAVLR,this));
    addSignalToMaquette(new LightSignal(9,SAVLR,this));
    addSignalToMaquette(new LightSignal(10,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(11,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(12,CSAVLRR,this));
    addSignalToMaquette(new LightSignal(13,CSAVLRR,this));
    addSignalToMaquette(new LightSignal(15,SAVL,this));

    //Add IPCS
}

void MaquetteHandler::SETUP_AIGUILLES(){
    //VERY IMPORTANT, HERE IS THE INSTANCIATION OF ALL AIGUILLES
    addAiguilleToMaquette(new Aiguille(1,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(2,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(3,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(4,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(5,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(6,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(7,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(8,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(9,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(10,GAUCHE,this));

}

void MaquetteHandler::SET_ALL_VL(){
    for(LightSignal *sig : lightSignals){
        sig->setAspect(VL);
    }
}


void MaquetteHandler::SET_ALL_DIR(Direction dir){
    //usefull ??
    for(Aiguille *aig : aiguilles){
        aig->setDirection(dir);
    }
}
