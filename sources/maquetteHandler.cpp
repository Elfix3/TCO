#include "maquetteHandler.h"
#include <QMessageBox>

MaquetteHandler::MaquetteHandler(QObject *parent)
    : QObject(parent){
    qInfo()<<"<---------Maquette Handler created--------->\n";
}

MaquetteHandler::~MaquetteHandler(){
    qDeleteAll(lightSignals);
    qDeleteAll(aiguilles);
    qDeleteAll(zones);
}

void MaquetteHandler::INIT(){ //to rework ?
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

    //
    SET_ALL_VL();
    
}

const QMap<int, LightSignal*>& MaquetteHandler::getAllSignals(){
    return lightSignals;
}

const QMap<int, Aiguille *>& MaquetteHandler::getAllAiguilles(){
    return aiguilles;
}

const QMap<QString, Zone *>& MaquetteHandler::getAllZones(){
    return zones;
}

void MaquetteHandler::emitAllStates(){
    //sends all the signals to update the maquette
    for(LightSignal *sig : lightSignals){
        sig->emitUpdateSig();
    }
    for(Aiguille *aig : aiguilles){
        aig->emitUpdateAig();
    }
    for(Zone *z : zones){
        z->emitUpdateZone();
    }
    emit initBALstatus(IsBalActive); // is supposed to be on by default :)
}

void MaquetteHandler::updateTrainPosition(const QString &command){
    //recieves the command from the sensor to update signals S->A->VL
    //command structure is : "/C-Z4A"
    
    /*train position is a zone occupied, used to determine whether
    user changed on signalisation should be allowed or not*/
    if(IsBalActive){
        if(command.startsWith("/C_Z")){
            Zone *zone = zones[command.mid(4)]; //gets the corresponding zone
            if(zone!=nullptr){
                zoneTrain1 = zone;
                LightSignal *protectionSig = zoneTrain1->getProtectionSignal();

                if(protectionSig!=nullptr){
                    protectionSig->setAspect(S);
                    protectionSig->getPrevious()->setAspect(A);
                    protectionSig->getPrevious()->getPrevious()->setAspect(VL);
                }
                

            } else {
                qWarning() << "Error : Zone "<<command.mid(4) << "does not exist on the maquette";
            }
        } else {
            qWarning() << "Error : command" << command<< "is not valid";
        }
    } 


}


void MaquetteHandler::handleObjectUpdate(){
    //messy or clear enough ?
    QObject* obj = sender();
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


// rework these baddies please, these bitches not doing their work correctly :(

void MaquetteHandler::updateSignalFromCombo(int id, Aspect newAspect){
    lightSignals[id]->setAspect(newAspect);
}

void MaquetteHandler::updateZoneFromRadioButton(QString name, bool state){
    zones[name]->setState(state);
}

void MaquetteHandler::updateAiguilleFromRadioButton(int id, Direction newDir){
    qDebug() << "fn call";
    aiguilles[id]->setDirection(newDir);
}

void MaquetteHandler::disableBAL(){
    if(IsBalActive){
        qDebug() << "BAL disabled";
        IsBalActive = false;
    } else {
        //qWarning() << "BAL is already inactive";
    }
}

void MaquetteHandler::enableBAL(){
    if(!IsBalActive){
        qDebug() << "BAL enabled";
        IsBalActive = true;
    } else {
        //qWarning() << "BAL is already active";
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
        qWarning() << "Error : Signal with ID" << sigId << "already exits";
    } else {
        
        //qDebug() << "Signal"<<sigId<< "with type"<<mySignal->toString(mySignal->getType()).c_str()<<"added to the maquette" << "\t\t"<< (mySignal->getisIPCS() ? "[IPCS]":"[Normal sens]");
        
        if(debug.signal){ QString typeStr = QString::fromStdString(mySignal->toString(mySignal->getType()));
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
        if(debug.aiguille) qDebug() << "Aiguille" << aigId << "added to the maquette";
        
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
        if(debug.zone) qDebug() << "Zone with name" << name << "added to the maquette";
        
        zones[zone->getName()] = zone; 
        connect(zone,&Zone::powerChanged,this,&MaquetteHandler::handleObjectUpdate);
    }
}


//##########################################//
//#####Instanciation of my models here#####//
//#########################################//

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
    addAiguilleToMaquette(new Aiguille(5,DROITE,this));
    addAiguilleToMaquette(new Aiguille(6,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(7,GAUCHE,this));
    addAiguilleToMaquette(new Aiguille(8,DROITE,this));
    addAiguilleToMaquette(new Aiguille(9,DROITE,this));
    addAiguilleToMaquette(new Aiguille(11,DROITE,this));

    //10 is missing, not an error :)

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


bool MaquetteHandler::connectSignalsById(int previousId,int nextId){
    qDebug() << previousId << " : " << nextId;
    if(nextId == previousId){
        qWarning() << "Error : cannot connect a signal to itself";
        return false;
    }
    
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

bool MaquetteHandler::connectZonesByNames(QString previousName, QString nextName){
    qDebug() << previousName << " : " << nextName;
    if(previousName == nextName){
        qWarning() << "Error : cannot connect a zone to it self";
    }

    if(!zones.contains(previousName)){
        qWarning() << "Error : zone with name" << previousName << "not found";
    }
    if(!zones.contains(nextName)){
        qWarning() << "Error : zone with name" << nextName << "not found";
    }
    Zone* prev = zones[previousName];
    Zone* next = zones[nextName];
    prev->setNextZone(next);
    next->setPreviousZone(prev);
    return true;
}

bool MaquetteHandler::connectSignalsWithZone(int idSig, QString zoneName){
    qDebug() << idSig << "covers" << zoneName;
    if(!zones.contains(zoneName)){
        qWarning() << "Error : zone with name" << zoneName << "not found";
        return false;
    }
    if(!lightSignals.contains(idSig)){
        qWarning() << "Error : signal with Id" << idSig << "not found";
        return false;
    }
    Zone *z = zones[zoneName];
    LightSignal *s = lightSignals[idSig];
    z->setProtectionSignal(s);
    s->setprotectedZone(z);
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
    
 
    qDebug() << "Setting up of all the connections :";


    //### SIGNAUX ###//

    //Voie 1 sens
    for(int i = 1; i<16; i+=2){
        if(!connectSignalsById(i,(i+2)%16)){ //connects all the odd signals (1,3) (3,5) [...] (13,15) (15,1)
            return false;
        }
    }
    //Voie 2 sens
    for(int i = 2; i <= 12;i+=2){
        if(!connectSignalsById(i, ((i==12) ? 2 : i + 2))){ //connects all the even signals (2,4) (4,6) [...] (12,2)
            return false;
        } 
    }

    //### ZONES avec ZONES ###///

    //voie 1
    for(int i = 1; i <= 15; i += 2) {
        QString currentA = QString("%1A").arg(i);
        QString currentB = QString("%1B").arg(i);
        QString nextA = QString("%1A").arg((i + 2) % 16);  // Bouclage 15→1

        if(!connectZonesByNames(currentA, currentB) || !connectZonesByNames(currentB, nextA)){
            return false;
        }
    }
    //voie 2
    for(int i = 2; i<=12;i+=2){
        QString currentA = QString("%1A").arg(i);
        QString currentB = QString("%1B").arg(i);
        QString nextA = QString("%1A").arg(((i==12) ? 2 : i + 2));
        if(!connectZonesByNames(currentA, currentB) || !connectZonesByNames(currentB, nextA)){
            return false;
        }
    }

    //### SIGNAUX avec ZONES ###//
    for(int i = 1 ; i <=15;i++){
        //ADD IPCS AS WELL !!!!!!!
        if(i==14)continue; //14 or any incorrect signal value;

        if(!connectSignalsWithZone(i,QString("%1A").arg(i))){
            return false;
        }
        //here I should also connect IPCS to their B protected zone
    }

    return true;


}




void MaquetteHandler::SET_ALL_VL(){ //necessary ?
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
