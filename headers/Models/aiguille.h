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
            Direction getInitialDir();
            Aiguille* getConjAiguille();
            QVector<LightSignal*> getProtectionSignal();
            
            //setters
            void setDirection(Direction dir);
            void setConj(Aiguille *conj);
            void addProtectionSig(LightSignal *sig);

            void emitUpdateAig();
        signals :
            void positionChanged();

        private :
            int id;
            Direction currentDirection;
            Direction initialDir;

            Aiguille* aiguilleConj = nullptr; //aiguille conjuguée
            
            QVector<LightSignal*> protectionSignals;
            
            
            LightSignal* protectionSignal = nullptr; //on protège l'aiguille  avec un signal avant
            LightSignal* protectionSignal2 = nullptr; //pretty much useless

            bool debugInfos = false;
};

#endif //AIGUILLE_H