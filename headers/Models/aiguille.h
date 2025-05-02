#ifndef AIGUILLE_H
#define AIGUILLE_H

//my Includes
#include "lightSignal.h"

//Qt includes
#include <QObject>
#include <QDebug>
#include <QStringList>


enum Direction {DROITE,GAUCHE};

    class Aiguille : public QObject{
        Q_OBJECT
        public :
            explicit Aiguille(int id, Direction dir=GAUCHE, QObject *parent = nullptr);
            ~Aiguille();

            //getters
            int getId();
            Direction getDirection();
            Aiguille* getConjAiguille();
            LightSignal* getPreviousSignal();
            
            //setters
            void setDirection(Direction dir);
            void setConj(Aiguille *conj);
        signals :
            void aiguilleUpdate();

        private :
            int id;
            Direction currentDirection;

            Aiguille* aiguilleConj = nullptr; //aiguille conjuguée
            LightSignal* previousSignal = nullptr; //on protège l'aiguille  avec un signal avant

};

#endif //AIGUILLE_H