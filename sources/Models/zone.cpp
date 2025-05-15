#include "zone.h"


Zone::Zone(QString name, QObject *parent)
    :QObject(parent), name(name), currentState(false){
        //qDebug() << "Creation of  zone " << "Z -" << name << (currentState ? "POWERED " : "NOT POWERED");
}

Zone::~Zone(){
}

void Zone::state(bool state){
    currentState = state;
    previousZone->currentState = state;
    //logique à implémenter !!!!!!!!!

    //désactivation du relais précédent TOUJOURS
    //mise à jour des signaux (tout le bal donc) et TOUJOURS
    //si feu suivant différent de sémaphore ou carré, activer le relais
    //ET IPCS ????????
}

void Zone::toggleState(){
    currentState = !currentState;
}

void Zone::setPreviousZone(Zone* z){
    previousZone = z;
}

void Zone::setNextZone(Zone* z){
    nextZone = z;
}

void Zone::setNextSignal(LightSignal* s){
    nextSignal = s;
}


QString Zone::getName(){
    return name;
}

bool Zone::isZoneEnabled(){
    return currentState;
}
