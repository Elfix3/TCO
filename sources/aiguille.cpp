#include "aiguille.h"

Aiguille::Aiguille(int id, Direction dir ,QObject *parent)
 :QObject(parent), id(id), currentDirection(dir){
    //qDebug() << "Aiguille" << id << "created" << (dir==DROITE ? "right direction" : "left direction");
}

Aiguille::~Aiguille(){
}
//infos

int Aiguille::getId(){
    return id;
}
Direction Aiguille::getDirection(){
    return currentDirection;
}
const Aiguille *Aiguille::getConjAiguille(){
    return aiguilleConj;
}

const LightSignal *Aiguille::getPreviousSignal(){
    return previousSignal;
}
