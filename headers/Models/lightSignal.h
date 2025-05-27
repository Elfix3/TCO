#ifndef SIGNAL_H
#define SIGNAL_H


//file includes
#include "zone.h"

//Qt includes
#include <QObject>
#include <QDebug>
#include <QStringList>

class Zone;

enum SignalType {
    SAVL = 3, 
    SAVLR = 5,
    CSAVLRR = 7,
    CSAVLRRR = 9
};
enum Aspect{
    IDLE, //inactif
    VL, //Voie libre
    A, //Avertissement
    S, //Sémaphore
    C, //Carré
    R, //Ralentissement
    RR // Rappel ralentissement
};


class LightSignal : public QObject{
    Q_OBJECT
    public :
        explicit LightSignal(int id ,SignalType type, QObject *parent = nullptr);
        ~LightSignal();

        void info();



        //getters
        int getId();
        SignalType getType();
        Aspect getAspect();
        LightSignal* getPrevious();
        LightSignal* getNext();
        bool getisIPCS();



        //setters
        void setAspect(Aspect newAspect);
        void setId(int id); //useless mais
        void setPrevious(LightSignal* previous);
        void setNext(LightSignal* next);
        void setprotectedZone(Zone *zoneToProtect);

        //miscellaneaous
        static std::string toString(Aspect someAspect);
        

        std::string toString(SignalType someType);
        
        bool isValidAspect(Aspect aspect);
        

        void emitUpdateSig();
    signals:
        void aspectChanged();

    protected :

    private:
        int id;
        const SignalType type; //SIGNAL DOESNT CHANGE ITS TYPE
        Aspect currentAspect;
        bool isIPCS;

        //pointers to other objetcs
        Zone* protectedZone;
        LightSignal* previousSignal = nullptr;
        LightSignal* nextSignal = nullptr; 



        //used for dispalay purposes
        


};



#endif //SIGNAL_H