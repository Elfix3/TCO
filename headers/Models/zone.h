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

class Zone : public QObject{
    Q_OBJECT
    public :
        explicit Zone(QString name, int* hints, QObject *parent = nullptr);
        ~Zone();

        //setters
        void setState(bool state);
        void toggleState(); //if on goes off and if off goes on
        void setPreviousZone(Zone* z);
        void setNextZone(Zone *z);
        void setNextSignal(LightSignal *s);

        //getters
        QString getName();
        int* getHint();
        int GetOrigineX();
        int GetOrigineY();
        int GetModificationAreaWidth();
        int GetModificationAreaHeight();
        QPoint GetRelativeOrigineRect1();
        QPoint GetRelativeOrigineRect2();
        bool isZoneEnabled();
        

        void emitUpdateZone();
    signals :
        void powerChanged();
    private :
        QString name;
        bool currentState;
        int *hint;

        Zone *nextZone;
        Zone *previousZone;
        LightSignal *nextSignal; //to allow enable of the zone on non semaphore or carre next signal

    
};

#endif //ZONE_H
