#ifndef CONTROL_H
#define CONTROL_H

//myIncludes
#include "maquetteHandler.h"
//Qt includes
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QVariant>
#include "ui_control.h"


class Control : public QMainWindow {
    Q_OBJECT
    public:
        explicit Control(QWidget *parent = nullptr);
        ~Control();
        
        void INIT();

        void SetupConnections();

        void loadMaquette(MaquetteHandler* maquette);

    private:
        Ui::CONTROL *ui;

        bool isUserUpdate = false; //alows the distinction beetween a user update and a 
        
        
        
        
        void loadSignalQvariant();
        //loadAiguillesQVAR ?
        //loadZoneQVAR ?
        
    
    signals:
    
        void aiguilleChangedFromControl(int id, Direction newDir);
        void signalChangedFromControl(int id, Aspect newAspect);
        void zoneChangedFromControl(QString name, bool state);

        void sendAiguilleImpulse(int id, Direction newDir);


        void BALisDisabled();
        void BALisEnabled();
        //void sendAiguilleUpdate(); //// KEJGBKJEGOEGJBEKGB TROP DE TRUCS A FAIRE
        //void sendZoneUpdate();


    public slots :
        //void updateSigComboBox(int signalId, Aspect newAspect);
        void updateSignalOnControl(int id, Aspect newAspect);
        void updateZoneOnControl(QString name, bool state);
        void updateAiguilleOnControl(int id, Direction newDir);

        void setUpBALstatus(bool status);

};

#endif //CONTROL_H