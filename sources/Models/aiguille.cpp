#include "aiguille.h"

Aiguille::Aiguille(int id, Direction dir,QObject *parent)
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
Aiguille *Aiguille::getConjAiguille(){
    return aiguilleConj;
}

LightSignal *Aiguille::getPreviousSignal(){
    return previousSignal;
}

void Aiguille::setDirection(Direction newDir){
    if(currentDirection!=newDir){
        qDebug()<< "Aiguille" << id << "is set to"<< (newDir==DROITE ? "DROITE" : "GAUCHE");
        currentDirection = newDir;
        emit positionChanged();
    } else {
        qWarning() << "Set direction not successfull ";
    }
}

void Aiguille::setConj(Aiguille *conj){
    aiguilleConj = conj;
}

void Aiguille::emitUpdateAig(){
    emit positionChanged();
}
