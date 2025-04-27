#ifndef SIGNAL_H
#define SIGNAL_H


//file includes
//#include "signalView.h"

//Qt includes
#include <QObject>
#include <QDebug>
#include <QStringList>





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

        //setters
        void setAspect(Aspect newAspect);
        void setId(int id); //useless
        void setPrevious(LightSignal* previous);
        void setNext(LightSignal* next);

        //miscellaneaous
        std::string toString(Aspect someAspect);
        std::string toString(SignalType someType);
        bool isValidAspect(Aspect aspect);
        
    signals:
        void signalUpdated();

    protected :

    private:
        int id;
        const SignalType type; //SIGNAL DOESNT CHANGE ITS TYPE
        Aspect currentAspect;
        
        LightSignal* previousSignal = nullptr;
        LightSignal* nextSignal = nullptr; 



        //used for dispalay purposes
        QStringList allTypes = {"SAVL","SAVLR","CSAVLRR","CSAVLRRR"};
        QStringList allAspects = {"IDLE","VL","A","S","C","R","RR"};


};



#endif //SIGNAL_H