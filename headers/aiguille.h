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
            explicit Aiguille(int id, Direction dir=DROITE, QObject *parent = nullptr);
            ~Aiguille();

            //getters
            int getId();
            Direction getDirection();
            const Aiguille* getConjAiguille();
            const  LightSignal* getPreviousSignal();
            

        signals :
            void aiguilleUpdate();

        private :
            int id;
            Direction currentDirection;

            Aiguille* aiguilleConj = nullptr; //aiguille conjuguée
            LightSignal* previousSignal = nullptr; //on protège l'aiguille  avec un signal avant

};

#endif //AIGUILLE_H