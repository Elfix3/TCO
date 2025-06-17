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
#include <QRect>

#include <map>
#include <string>

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

            void processDirection(Zone *newZone);
            void BALSignalActivation(Zone *z,LightSignal *s); //used to manage S->A->VL and zone poweroff
            void emitAllStates();

    public slots:
            //UPDATES TRAINS POSITION !!!!!
            void handleCommand(const QString &command);//updates the train position
        


            void handleObjectUpdate(); //sends the correct signals for any object change 
                
            void updateSignalFromCombo(int id, Aspect newAspect); //probably useless
            void updateZoneFromRadioButton(QString name, bool state);
            void updateAiguilleFromRadioButton(int id, Direction newDir);
        
            void protectAiguille(int id);

            void maquetteReset();

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


            const std::map <std::string, std::array<int, 8>> tracksIlluminations = {
                //voie 1 selon le sens de circulation
                //Rect X Start, Rect Y start, Rect Width, Rect Height, LinePointX, LinePointY
                // 1440x810 ; image originelle : 3840x2160  => 8/3
                { "1A", { 344, 568, 152, 232, 496, 440, 192, 160 } },
                { "1B", { 680, 360, 368, 96, 0, 0, 0, 0 } },
                { "3A", { 1048, 360, 696, 120, 0, 0, 0, 0 } },
                { "3B", { 1747, 360, 308, 24, 0, 0, 0, 0 } },
                { "5A", { 2048, 360, 840, 120, 0, 0, 0, 0 } },
                { "5B", { 2888, 368, 304, 144, 0, 0, 0, 0 } },
                { "7A", { 3192, 504, 264, 328, 3432, 832, 80, 400 } },
                { "7B", { 3352, 1232, 136, 280, 0, 0, 0, 0 } },
                { "9A", { 3176, 1512, 184, 160, 2980, 1672, 232, 120 } },
                { "9B", { 2656, 1776, 328, 48, 0, 0, 0, 0 } },
                { "11A", { 2000, 1704, 656, 120, 0, 0, 0, 0 } },
                { "11B", { 1256, 1800, 744, 24, 0, 0, 0, 0 } },
                { "13A", { 712, 1704, 544, 120, 0, 0, 0, 0 } },
                { "13B", { 464, 1568, 248, 184, 0, 0, 0, 0 } },
                { "15A", { 280, 1112, 184, 456, 0, 0, 0, 0 } },
                { "15B", { 272, 800, 88, 312, 0, 0, 0, 0 } },
                //voie 2 selon le sens de circulation
                { "2A", { 576, 1280, 312, 296, 0, 0, 0, 0 } },
                { "2B", { 888, 1560, 328, 48, 0, 0, 0, 0 } },
                { "4A", { 1216, 1584, 640, 16, 1248, 1600, 128, 104 } },
                { "4B", { 1856, 1584, 376, 16, 0, 0, 0, 0 } },
                { "6A", { 2240, 1384, 888, 216, 2216, 1600, 272, 104 } },
                { "6B", { 3120, 1088, 104, 296, 0, 0, 0, 0 } },
                { "8A", { 3104, 768, 120, 312, 2848, 600, 256, 184 } },
                { "8B", { 2608, 584, 232, 32, 0, 0, 0, 0 } },
                { "10A", { 1880, 584, 728, 16, 2224, 480, 256, 104} },
                //{ "10B", { 1464, 584, 408, 224, 1080, 808, 1736, 264 } }, //Stockage
                { "10B", { 1464, 584, 408, 224, 0,0,0,0} },
                { "12A", { 856, 480, 608, 144, 568, 624, 312, 288 } },
                { "12B", { 536, 912, 48, 360, 0, 0, 0, 0 } },
                };

            Zone* zoneTrain1 = nullptr; //stores the position of the first train
            Zone* zoneTrain2 = nullptr; //stores the position of the second train (not yet implemented)
            
            short directionTrain1 = -1; //-1 for undefined 0 for normal, 1 for IPCS



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
            bool connectSignalsWithZone(int idSigNormal, int idIPCS, QString zoneName);
            bool connectAiguilleWithSignal(int idAig, int idProtectionSig);
            bool connectAiguilleConj(int aig, int conj); //NA atm
            bool connectSetup(int setup=1);

            
            

            void SET_ALL_VL();
            void SET_ALL_DIR(Direction dir);
            void TURN_OFF_ZONES();
            
            static int getIPCSsig(int sig);
            static int getZoneNum(Zone *z);

            //to show some debug informations
            struct DebugFlags{
                bool signal = false;
                bool aiguille = false;
                bool zone = false;
            };
            DebugFlags debug;
           
};



#endif //MAQUETTE_HANDLER_H