#ifndef ZONE_H
#define ZONE_H

//My includes
#include "lightSignal.h"

//Qt includes
#include <QObject>
#include <QPoint>

#define TI_RECT1_X 0
#define TI_RECT1_Y 1
#define TI_RECT1_WIDTH 2
#define TI_RECT1_HEIGHT 3
#define TI_RECT2_X 4
#define TI_RECT2_Y 5
#define TI_RECT2_WIDTH 6
#define TI_RECT2_HEIGHT 7

class LightSignal;

class Zone : public QObject{
    Q_OBJECT
    public :
        explicit Zone(QString name, int* hints, QObject *parent = nullptr);
        ~Zone();

        //setters
        void setState(bool state);
        void setHasTrain(bool h);
        void toggleState(); //if on goes off and if off goes on
        void setPreviousZone(Zone* z);
        void setNextZone(Zone *z);
        void setProtectionSignals(LightSignal *s,LightSignal *sIPCS);

        //getters
        QString getName();
        LightSignal* getProtectionSignal();
        LightSignal* getProtectionSignalIPCS();
        bool isZoneEnabled();
        bool hasTrain();
        Zone* getPreviousZone();
        Zone* getNextZone();
        
        int* getHint();
        int GetOrigineX();
        int GetOrigineY();
        int GetModificationAreaWidth();
        int GetModificationAreaHeight();
        QPoint GetRelativeOrigineRect1();
        QPoint GetRelativeOrigineRect2();

        void emitUpdateZone();
    signals :
        void powerChanged();
    private :
        QString name;
        bool currentState;
        bool hasTrainOnIt;
        int *hint;

        Zone *nextZone = nullptr;
        Zone *previousZone = nullptr;
        
        LightSignal *protectionSignal = nullptr; //signal that protects the zone
        LightSignal *protectionSignalIPCS = nullptr;
    
};

#endif //ZONE_H