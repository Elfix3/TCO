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

void MaquetteHandler::zoneUpdateFromSensor(const QString &command){
    //COMMAND to process : type /Z-12B END
    //In this case, looks for the zone 12B and updates it
    
    if(command.startsWith("/Z-")&&command.endsWith(" END")){
        qDebug() << "Processing of the command : ";
        QString zone = command.mid(1,command.length()-5);
        ///////Extraction du nom de la zone à mettre à jour

    } else {
        qWarning() << "Error : incorrect command from arduino, no relais update";
        return;
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

        

        //qDebug() << "Signal"<<sigId<< "with type"<<mySignal->toString(mySignal->getType()).c_str()<<"added to the maquette" << "\t\t"<< (mySignal->getisIPCS() ? "[IPCS]":"[Normal sens]");
        
    QString typeStr = QString::fromStdString(mySignal->toString(mySignal->getType()));
    QString sensStr = mySignal->getisIPCS() ? "[IPCS]" : "[Normal sens]";
    qDebug() << "Signal" << sigId
         << "with type" << typeStr
         << "added to the maquette"
         << "\t\t" << qPrintable(sensStr.leftJustified(13, ' '));
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
    addSignalToMaquette(new LightSignal(1, SAVLR,this)); //voie 1 selon le sens de circulation
    addSignalToMaquette(new LightSignal(3, CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(5, CSAVLRR,this));
    addSignalToMaquette(new LightSignal(7,SAVL,this));
    addSignalToMaquette(new LightSignal(9,SAVLR,this));
    addSignalToMaquette(new LightSignal(11,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(13,CSAVLRR,this));
    addSignalToMaquette(new LightSignal(15,SAVL,this));
    
    addSignalToMaquette(new LightSignal(2, SAVLR,this)); //voie 2 selon le sens de circulation
    addSignalToMaquette(new LightSignal(4, CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(8,SAVLR,this));
    addSignalToMaquette(new LightSignal(6, CSAVLRR,this));
    addSignalToMaquette(new LightSignal(10,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(12,CSAVLRR,this));

    //Add IPCS
    addSignalToMaquette(new LightSignal(17,SAVL,this)); //voie 1 selon le contre sens
    addSignalToMaquette(new LightSignal(19,SAVLR,this));
    addSignalToMaquette(new LightSignal(21,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(23,CSAVLRR,this));
    addSignalToMaquette(new LightSignal(25,SAVL,this));
    addSignalToMaquette(new LightSignal(27,SAVLR,this));
    addSignalToMaquette(new LightSignal(29,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(31,CSAVLRR,this));

    addSignalToMaquette(new LightSignal(14,SAVLR,this)); // voie 2 selon le contre sens
    addSignalToMaquette(new LightSignal(16,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(18,CSAVLRR,this));
    addSignalToMaquette(new LightSignal(20,SAVLR,this));
    addSignalToMaquette(new LightSignal(22,CSAVLRRR,this));
    addSignalToMaquette(new LightSignal(24,CSAVLRR,this));
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
