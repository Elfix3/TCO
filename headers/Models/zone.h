#ifndef ZONE_H
#define ZONE_H

//My includes
#include "lightSignal.h"

//Qt includes
#include <QObject>

class Zone : public QObject{
    Q_OBJECT
    public :
        explicit Zone(QString name, QObject *parent = nullptr);
        ~Zone();

        //setters
        void setState(bool state);
        void toggleState(); //if on goes off and if off goes on
        void setPreviousZone(Zone* z);
        void setNextZone(Zone *z);
        void setNextSignal(LightSignal *s);

        //getters
        QString getName();
        bool isZoneEnabled();
        
    signals :
        void powerChanged();
    private :
        QString name;
        bool currentState;

        Zone *nextZone;
        Zone *previousZone;
        LightSignal *nextSignal; //to allow enable of the zone on non semaphore or carre next signal

    
};

#endif //ZONE_H