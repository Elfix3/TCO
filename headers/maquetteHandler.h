#ifndef MAQUETTE_HANDLER_H
#define MAQUETTE_HANDLER_H



//file includes
#include "lightSignal.h"
#include "aiguille.h"
#include "zone.h"

//Qt includes
#include <QObject>
#include <QMap>
#include <QRegularExpression>

class MaquetteHandler : public QObject {
    Q_OBJECT

    public:
        explicit MaquetteHandler(QObject *parent = nullptr);
        ~MaquetteHandler();

        
        void INIT();


        //Getters
        QMap<int,LightSignal*> getAllSignals();
        QMap<int,Aiguille*> getAllAiguilles();
        QMap<QString,Zone*> getAllZones();

    public slots:

        void zoneUpdateFromSensor(const QString &command); //updates the internal state  of the maquette
        void handleObjectUpdate(); //sends the correct signals for any object change


        void updateSignalFromCombo(int id, Aspect newAspect); //probably useless
        
        signals:
        
            void signalChanged(int id, Aspect newAspect);
            void aiguilleChanged(int id, Direction newDirection);
            void zoneChanged(QString name, bool state);
            
            void sendCommand(const QString &command); //should we keep this
        
        
        private:
            QMap <int,LightSignal*> lightSignals; //Qmap with all my lightSignals
            QMap <int,Aiguille*> aiguilles; //Qmap with all my aiguilles
            QMap <QString,Zone*> zones; //Qmap with all my zones

            //Setup  functions
            void SETUP_SIGNALS();
            void SETUP_AIGUILLES();
            void SETUP_ZONES();

            //all adding functions for my maquette objets (connects with the handle update slot)
            void addSignalToMaquette(LightSignal *mySignal);
            void addAiguilleToMaquette(Aiguille *myAiguille);
            void addZoneToMaquette(Zone* zone);
            
            bool connectSignalsById(int next, int previous);
            bool connectAiguilleConj(int aig, int conj);
            bool connectSetup(int setup=1);


            
            

            void SET_ALL_VL();
            void SET_ALL_DIR(Direction dir);
            



            //debug parameters
            bool debugSignal = 0;
            bool debugAiguille = 0;
            bool debugZone = 0;
};



#endif //MAQUETTE_HANDLER_H