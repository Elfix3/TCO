#include "lightSignal.h"

static QStringList allTypes = {"SAVL","SAVLR","CSAVLRR","CSAVLRRR"};
static QStringList allAspects = {"IDLE","VL","A","S","C","R","RR"};


LightSignal::LightSignal(int id, SignalType type, QObject *parent)
 :QObject(parent), id(id), type(type), currentAspect(IDLE), isIPCS(false){
     if(id>15 || id == 14){
         isIPCS = true;
        }
    //qDebug() <<"Signal" << id << "created" << (isIPCS ? "in IPCS" :"");

}

LightSignal::~LightSignal(){
    delete this;
}

//infos for debug purposes
void LightSignal::info(){
    if(this!=nullptr){
        

        qDebug() << "\033[33m\n<-------------------------->";
        qDebug() << "<------Signals info :------>";
        qDebug() << "<-------------------------->\033[0m";
        qDebug() << "Signal id : " << id;
        qDebug() << "Signal type : " << allTypes[(type-3)/2].toStdString().c_str();
        qDebug() << "Signal aspect :" << allAspects[currentAspect].toStdString().c_str();
        if(previousSignal != nullptr){
            qDebug()<<"Previous signal id :"<<previousSignal->id;
        }
        if(nextSignal != nullptr){
            qDebug() << "Next signal id :"<<nextSignal->id;
        }
        qDebug() << "\033[33m<---------End info--------->\033[0m\n\n";
    } else {
        qWarning() << "Error : No info available, nullptr";
    }
}

//Getters
int LightSignal::getId(){
    return id;
}
SignalType LightSignal::getType(){
    return type;
}
Aspect LightSignal::getAspect(){
    return currentAspect;
}
LightSignal *LightSignal::getPrevious(){
    return previousSignal;
}
LightSignal *LightSignal::getNext(){
    return nextSignal;
}
bool LightSignal::getisIPCS(){
    return isIPCS;
}

//Setters
void LightSignal::setAspect(Aspect newAspect){
    if(newAspect != currentAspect && isValidAspect(newAspect)){
        qDebug() << "Signal" << id << "is set to"<<toString(newAspect).c_str();
        currentAspect = newAspect;
        emit aspectChanged();
    } else {
        qWarning() << "Set aspect not successfull";
    }

}
void LightSignal::setId(int newId){
    return; //pretty much useless
    id = newId;
}

void LightSignal::setPrevious(LightSignal *previous){
    previousSignal = previous;
}
void LightSignal::setNext(LightSignal *next){
    nextSignal = next;
}






//Miscellaneaous 
std::string LightSignal::toString(Aspect someAspect){
    return allAspects[someAspect].toStdString();
}
std::string LightSignal::toString(SignalType someType){
    return allTypes[(someType-3)/2].toStdString();
}
bool LightSignal::isValidAspect(Aspect aspect){
    if(aspect == IDLE){
        return true;
    }
    if((type == SAVL || type == CSAVLRR) && aspect == R  ){
        //try to update as ralentissement
        qWarning() << "Error : no" <<toString(aspect).c_str() << "on a"<<toString(type).c_str()<<"signal";
        return false;
    }
    if((type == SAVL || type == SAVLR) && (aspect == RR || aspect == C)){
        qWarning() << "Error : no" <<toString(aspect).c_str() << "on a"<<toString(type).c_str()<<"signal";
        return false;
    }
    //no warning if is valid
    return true;
}

void LightSignal::emitUpdateSig(){
    emit aspectChanged();
}
