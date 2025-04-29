#ifndef MAQUETTE_HANDLER_H
#define MAQUETTE_HANDLER_H

//file includes
#include "lightSignal.h"
#include "aiguille.h"

//Qt includes
#include <QObject>
#include <QMap>

class MaquetteHandler : public QObject {
    Q_OBJECT

    public:
        explicit MaquetteHandler(QObject *parent = nullptr);
        //~MaquetteHandler();
        QMap<int,LightSignal*> getAllSignals();

        
        void processCommand(QString command);
        
        signals:
            void sendCommand(const QString &command);
        
        private:
            QMap <int,LightSignal*> lightSignals; //Qmap with all my lightSignals
            QMap <int,Aiguille*> aiguilles; //Qmap with all my aiguilles
            //Qmap with relais or zones ? équivalent ???


            
            bool connectSignalsById(int next, int previous);
            bool connectSetup(int setup=1);


            void addSignalToMaquette(LightSignal *mySignal);
            void addAiguilleToMaquette(Aiguille *myAiguille);

            //Setup  functions to call in constructor
            void SETUP_SIGNALS();
            void SETUP_AIGUILLES();

};



#endif //MAQUETTE_HANDLER_H