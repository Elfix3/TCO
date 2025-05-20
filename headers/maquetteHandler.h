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

        void updateAll(); //sends the update signal for anyObject on the maquette

    public slots:

        void zoneUpdateFromSensor(const QString &command); //updates the internal state  of the maquette
        void handleObjectUpdate(); //sends the correct signals for any object change 

        void updateSignalFromCombo(int id, Aspect newAspect); //probably useless
        void updateZoneFromRadioButton(QString name, bool state);
        
        
        void disableBAL();
        void enableBAL();

        signals:
        
            void signalChanged(int id, Aspect newAspect); //if isFromUser, wont try to change the Control
            void aiguilleChanged(int id, Direction newDirection);
            void zoneChanged(QString name, bool state);
            void initBALstatus(bool isEnabled); //note BAL stands for Bloc Automatique Lumineux. It's responsible of enabling signalisation or not


            void sendCommand(const QString &command); //should we keep this
        
        
        private:
            //all my objects
            QMap <int,LightSignal*> lightSignals; //Qmap with all my lightSignals
            QMap <int,Aiguille*> aiguilles; //Qmap with all my aiguilles
            QMap <QString,Zone*> zones; //Qmap with all my zones

            bool IsBalActive = true; //enables or disables the BAL

            //ADD objects functions
            void addSignalToMaquette(LightSignal *mySignal);
            void addAiguilleToMaquette(Aiguille *myAiguille);
            void addZoneToMaquette(Zone* zone);
            

            //CREATE objects with the add
            void SETUP_SIGNALS();
            void SETUP_AIGUILLES();
            void SETUP_ZONES();


            //CONNECT OBJECTS
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