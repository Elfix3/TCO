#ifndef MAQUETTE_HANDLER_H
#define MAQUETTE_HANDLER_H

//file includes
#include "lightSignal.h"
//#include "aiguilles.h"

//Qt includes
#include <QObject>
#include <QMap>

class MaquetteHandler : public QObject {
    Q_OBJECT

    public:
        explicit MaquetteHandler(QObject *parent = nullptr);
        //~MaquetteHandler();
        QMap<int,LightSignal*> getAllSignals();

        
        //
        void INIT_VL();
        
        void processCommand(QString command);
        
        signals:
            void sendCommand(const QString &command);
        
        private:
            QMap <int,LightSignal*> lightSignals;
            
            
            bool connectSignalsById(int next, int previous);
            bool connectSetup(int setup=1);
            void addSignalToMaquette(LightSignal *mySignal);

};



#endif //MAQUETTE_HANDLER_H