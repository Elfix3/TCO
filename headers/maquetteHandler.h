#ifndef MAQUETTE_HANDLER_H
#define MAQUETTE_HANDLER_H

//file includes
#include "lightSignal.h"
#include "aiguille.h"
#include "zone.h"

//Qt includes
#include <QObject>
#include <QMap>

class MaquetteHandler : public QObject {
    Q_OBJECT

    public:
        explicit MaquetteHandler(QObject *parent = nullptr);
        //~MaquetteHandler();
        QMap<int,LightSignal*> getAllSignals();
        QMap<int,Aiguille*> getAllAiguilles();
        
        //slots
        void zoneUpdateFromSensor(const QString &command);
        void updateSignalFromCombo(int id, Aspect newAspect);
        
        signals:
            void sendCommand(const QString &command); //??
            void lightSignalChanged(int id, Aspect newAspect);
        
        private:
            QMap <int,LightSignal*> lightSignals; //Qmap with all my lightSignals
            QMap <int,Aiguille*> aiguilles; //Qmap with all my aiguilles
            QMap <QString,Zone*> zones; //Qmap with relais or zones ? équivalent ???


            
            bool connectSignalsById(int next, int previous);
            bool connectAiguilleConj(int aig, int conj);
            bool connectSetup(int setup=1);


            void addSignalToMaquette(LightSignal *mySignal);
            void addAiguilleToMaquette(Aiguille *myAiguille);

            //Setup  functions to call in constructor
            void SETUP_SIGNALS();
            void SETUP_AIGUILLES();

            void SET_ALL_VL();
            void SET_ALL_DIR(Direction dir);

};



#endif //MAQUETTE_HANDLER_H