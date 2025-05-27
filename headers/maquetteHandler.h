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
            const QMap<int,LightSignal*>& getAllSignals(); //utile ?
            const QMap<int,Aiguille*>& getAllAiguilles();
            const QMap<QString,Zone*>& getAllZones();

            void emitAllStates();

    public slots:
            //UPDATES TRAINS POSITION !!!!!
            void updateTrainPosition(const QString &command);//updates the train position



            void handleObjectUpdate(); //sends the correct signals for any object change 
                
            void updateSignalFromCombo(int id, Aspect newAspect); //probably useless
            void updateZoneFromRadioButton(QString name, bool state);
            void updateAiguilleFromRadioButton(int id, Direction newDir);
                
            void disableBAL();
            void enableBAL();
        
         //sends the update signal for anyObject on the maquette
    signals:

            void signalChanged(int id, Aspect newAspect); //if isFromUser, wont try to change the Control
            void aiguilleChanged(int id, Direction newDirection);
            void zoneChanged(QString name, bool state);
            void initBALstatus(bool isEnabled); //note BAL stands for Bloc Automatique Lumineux. It's responsible of enabling signalisation or not


            void sendCommand(const QString &command); //should we keep this
        
        
    private:
            //##########################################//
            //#####  All objects on my maquette  #######//
            //##########################################//

            QMap <int,LightSignal*> lightSignals; //Qmap with all my lightSignals
            QMap <int,Aiguille*> aiguilles; //Qmap with all my aiguilles
            QMap <QString,Zone*> zones; //Qmap with all my zones

            Zone* zoneTrain1 = nullptr; //stores the position of the first train
            Zone* zoneTrain2 = nullptr; //stores the position of the second train (not yet implemented)
            

            bool IsBalActive = true; //enables or disables the BAL restrictions


            
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
            bool connectZonesByNames(QString previous, QString next);

            bool connectSignalsWithZone(int idSig, QString zoneName);

            bool connectAiguilleConj(int aig, int conj);
            bool connectSetup(int setup=1);

            
            

            void SET_ALL_VL();
            void SET_ALL_DIR(Direction dir);
            


            //to show some debug informations
            struct DebugFlags{
                bool signal = false;
                bool aiguille = false;
                bool zone = false;
            };
            DebugFlags debug;
           
};



#endif //MAQUETTE_HANDLER_H