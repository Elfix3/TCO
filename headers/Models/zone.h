#ifndef ZONE_H
#define ZONE_H

//My includes
#include "lightSignal.h"

//Qt includes
#include <QObject>

class LightSignal;

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
        void setProtectionSignal(LightSignal *s);

        //getters
        QString getName();
        LightSignal* getProtectionSignal();
        bool isZoneEnabled();
        

        void emitUpdateZone();
    signals :
        void powerChanged();
    private :
        QString name;
        bool currentState;

        Zone *nextZone = nullptr;
        Zone *previousZone = nullptr;
        
        LightSignal *protectionSignal = nullptr; //signal that protects the zone

    
};

#endif //ZONE_H