#include "maquetteHandler.h"
#include <QMessageBox>

MaquetteHandler::MaquetteHandler(QObject *parent)
    : QObject(parent){
}

MaquetteHandler::~MaquetteHandler(){
    qDeleteAll(lightSignals);
    qDeleteAll(aiguilles);
    qDeleteAll(zones);
}

void MaquetteHandler::INIT(){
    //create all the objets
    SETUP_SIGNALS();
    SETUP_AIGUILLES();
    SETUP_ZONES();
    
    //connects them up();
    if(!connectSetup()){
        qFatal("Error in the connect setup");
    } else {
        qDebug("Sucessfull connection setup");
    }


    //sets them in their initial aspect (to define)

}

QMap<int, LightSignal *> MaquetteHandler::getAllSignals(){
    return lightSignals;
}

QMap<int, Aiguille *> MaquetteHandler::getAllAiguilles(){
    return aiguilles;
}

QMap<QString, Zone *> MaquetteHandler::getAllZones(){
    return zones;
}

void MaquetteHandler::handleObjectUpdate(){
    QObject* obj = sender();
    qDebug() << "object update";
    if(!obj)return; //nullptr

    if(LightSignal *sig = qobject_cast<LightSignal*>(obj)){ //tries to cast the objet to a signal
        emit signalChanged(sig->getId(),sig->getAspect());
    } else if(Aiguille *aig = qobject_cast<Aiguille*>(obj)){ //tries to cast the object to an aiguille
        emit aiguilleChanged(aig->getId(), aig->getDirection());
    } else if(Zone *z = qobject_cast<Zone*>(obj)){ //tries to cast the object to a zone
        emit zoneChanged(z->getName(), z->isZoneEnabled());
    } else {
        qWarning("Error : unrecognized object type");
    }

}

bool MaquetteHandler::connectSignalsById(int previousId,int nextId){
    if(nextId == previousId){
        qWarning("Error : cannot connect a signal to itself");
        return false;
    }
    
    if(!lightSignals.contains(previousId)){
        qWarning() << "Error : signal with Id"<<previousId<<"not found";
        qWarning("here");
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

bool MaquetteHandler::connectAiguilleConj(int aigId, int conjId){
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

void MaquetteHandler::zoneUpdateFromSensor(const QString &command){
    //COMMAND to process : type /Z-12B END
    //In this case, looks for the zone 12B and updates it
    

    if(command.startsWith("/C_")){
        Zone *zoneToUpdate = zones[command.mid(3)];
        if(zoneToUpdate!=nullptr){
            zoneToUpdate->setState(true);
        }
    }
    /* if(command.startsWith("/Z-")&&command.endsWith(" END")){
        qDebug() << "Processing of the command : ";
        QString zone = command.mid(1,command.length()-5);
        ///////Extraction du nom de la zone à mettre à jour

    } else {
        qWarning() << "Error : incorrect command from arduino, no relais update";
        return;
    } */

}

void MaquetteHandler::updateSignalFromCombo(int id, Aspect newAspect){
    lightSignals[id]->setAspect(newAspect);
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
        qWarning() << "Error : Signal with ID" << sigId << "already exits";
    } else {
        
        //qDebug() << "Signal"<<sigId<< "with type"<<mySignal->toString(mySignal->getType()).c_str()<<"added to the maquette" << "\t\t"<< (mySignal->getisIPCS() ? "[IPCS]":"[Normal sens]");
        
        if(debugSignal){ QString typeStr = QString::fromStdString(mySignal->toString(mySignal->getType()));
        QString sensStr = mySignal->getisIPCS() ? "[IPCS]" : "[Normal sens]";
        qDebug() << "Signal" << sigId
            << "with type" << typeStr
            << "added to the maquette"
            << "\t\t" << qPrintable(sensStr.leftJustified(13, ' '));
        }
        //adds the signal to the maquetteHandler
        lightSignals[mySignal->getId()] = mySignal;
        //connects the update Signal to the handleUpdate slot
        connect(mySignal,&LightSignal::aspectChanged,this,&MaquetteHandler::handleObjectUpdate);
    }
    
}

void MaquetteHandler::addAiguilleToMaquette(Aiguille *myAiguille){
    short aigId = myAiguille->getId();
    //Direction dir = myAiguille->getDirection();
    if(aigId<1){
        qWarning() << "Error : Incorrect aiguille ID must be greater than 0";
    } else if(aiguilles.contains(aigId)){
        qWarning() << "Error : Aiguille with ID" << aigId << "  already exits";
    } else{
        if(debugAiguille) qDebug() << "Aiguille" << aigId << "added to the maquette";
        
        aiguilles[myAiguille->getId()] = myAiguille;
        connect(myAiguille,&Aiguille::positionChanged,this,&MaquetteHandler::handleObjectUpdate);
    }

}

void MaquetteHandler::addZoneToMaquette(Zone *zone){
    QString name = zone->getName();

    QRegularExpression regex("^\\d+[AB]$");
    QRegularExpressionMatch match = regex.match(name);
    if(!match.hasMatch()){
        qWarning() << "Error : " << name << "is not a valid name";
    } else if(zones.contains(name)){
        qWarning() << "Error : Zone with name : " << name << "already exists";
    } else {
        if(debugZone) qDebug() << "Zone with name" << name << "added to the maquette";
        
        zones[zone->getName()] = zone; 
        connect(zone,&Zone::powerChanged,this,&MaquetteHandler::handleObjectUpdate);
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

void MaquetteHandler::SETUP_ZONES(){
    addZoneToMaquette(new Zone("1A")); //voie 1 selon le sens de circulation
    addZoneToMaquette(new Zone("1B"));
    addZoneToMaquette(new Zone("3A"));
    addZoneToMaquette(new Zone("3B"));
    addZoneToMaquette(new Zone("5A"));
    addZoneToMaquette(new Zone("5B"));
    addZoneToMaquette(new Zone("7A"));
    addZoneToMaquette(new Zone("7B"));
    addZoneToMaquette(new Zone("9A"));
    addZoneToMaquette(new Zone("9B"));
    addZoneToMaquette(new Zone("11A"));
    addZoneToMaquette(new Zone("11B"));
    addZoneToMaquette(new Zone("13A"));
    addZoneToMaquette(new Zone("13B"));
    addZoneToMaquette(new Zone("15A"));
    addZoneToMaquette(new Zone("15B"));

    addZoneToMaquette(new Zone("2A")); //voie 2 selon le sens de circulation
    addZoneToMaquette(new Zone("2B"));
    addZoneToMaquette(new Zone("4A"));
    addZoneToMaquette(new Zone("4B"));
    addZoneToMaquette(new Zone("6A"));
    addZoneToMaquette(new Zone("6B"));
    addZoneToMaquette(new Zone("8A"));
    addZoneToMaquette(new Zone("8B"));
    addZoneToMaquette(new Zone("10A"));
    addZoneToMaquette(new Zone("10B"));
    addZoneToMaquette(new Zone("12A"));
    addZoneToMaquette(new Zone("12B"));

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
