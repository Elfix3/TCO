#include "zone.h"


Zone::Zone(QString name, int* ahint, QObject *parent)
    :QObject(parent), name(name), currentState(false), hasTrainOnIt(false){
        //qDebug() << "Creation of  zone " << "Z -" << name << (currentState ? "POWERED " : "NOT POWERED");
    this->hint = ahint;
}

Zone::~Zone(){
}

void Zone::setState(bool state){
    if(currentState!=state){
        if(debugInfos)qDebug() << "Zone"<<name<<"is now"<<(state==true ? "powered" : "unpowered");
        currentState = state;
        emit powerChanged();
        //bizarre
    } else {
        if(debugInfos)qDebug() << "Zone"<<name<<"is now"<<(state==true ? "powered" : "unpowered");
        currentState = state;
        emit powerChanged();
    }
    //logique à implémenter !!!!!!!!!

    //désactivation du relais précédent TOUJOURS
    //mise à jour des signaux (tout le bal donc) et TOUJOURS
    //si feu suivant différent de sémaphore ou carré, activer le relais
    //ET IPCS ????????
}

void Zone::setHasTrain(bool h){
    this->hasTrainOnIt = h;
}

void Zone::toggleState(){
    currentState = !currentState;
    emit powerChanged();
}

void Zone::setPreviousZone(Zone* z){
    previousZone = z;
}

void Zone::setNextZone(Zone* z){
    nextZone = z;
}

void Zone::setProtectionSignals(LightSignal *s, LightSignal *sIPCS){
    protectionSignal = s;
    protectionSignalIPCS = sIPCS;
}



QString Zone::getName(){
    return name;
}

LightSignal* Zone::getProtectionSignal(){
    return protectionSignal;
}

LightSignal *Zone::getProtectionSignalIPCS(){
    return protectionSignalIPCS;
}


int* Zone::getHint(){
    return hint;
}

int Zone::GetOrigineX(){
    if (hint[TI_RECT2_X] == 0)
        return hint[TI_RECT1_X];
    // ce second cas ne devrait pas se produire.
    if (hint[TI_RECT1_X] == 0)
        return hint[TI_RECT2_X];
    return std::min(hint[TI_RECT1_X], hint[TI_RECT2_X]);
}

int Zone::GetOrigineY(){
    if (hint[TI_RECT2_Y] == 0)
        return hint[TI_RECT1_Y];
    // ce second cas ne devrait pas se produire.
    if (hint[TI_RECT1_Y] == 0)
        return hint[TI_RECT2_Y];
    return std::min(hint[TI_RECT1_Y], hint[TI_RECT2_Y]);
}

int Zone::GetModificationAreaWidth()
{
    return std::max(hint[TI_RECT1_WIDTH] + hint[TI_RECT1_X], hint[TI_RECT2_WIDTH]+ hint[TI_RECT2_X]) - GetOrigineX();
}

int Zone::GetModificationAreaHeight()
{
    return std::max(hint[TI_RECT1_HEIGHT] + hint[TI_RECT1_Y], hint[TI_RECT2_HEIGHT]+ hint[TI_RECT2_Y]) - GetOrigineY();
}

QPoint Zone::GetRelativeOrigineRect1()
{
    return QPoint(hint[TI_RECT1_X] - GetOrigineX(), hint[TI_RECT1_Y] - GetOrigineY());
}

QPoint Zone::GetRelativeOrigineRect2()
{
    return QPoint(hint[TI_RECT2_X] - GetOrigineX(), hint[TI_RECT2_Y] - GetOrigineY());
}


bool Zone::isZoneEnabled(){
    return currentState;
}

bool Zone::hasTrain(){
    return hasTrainOnIt;
}



Zone* Zone::getPreviousZone(){
    return previousZone;
}

Zone* Zone::getNextZone(){
    return nextZone;
}

void Zone::emitUpdateZone(){
 emit powerChanged();
}
