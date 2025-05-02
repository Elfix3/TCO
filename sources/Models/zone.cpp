#include "zone.h"

Zone::Zone()
{
}

Zone::~Zone()
{
}

void Zone::state(bool state){
    currentState = state;
    previousZone->currentState = state;
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
