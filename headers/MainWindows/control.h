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

        void loadSignalQvariant();
        //loadAiguillesQVAR ?
        //loadZoneQVAR ?
        
    
    signals:
        //this signals allows the sending of the newAspectChange to the maquetteHandler
        void sendSignalUpdate(int signalId, Aspect newAspect);
        
        
        //void aiguilleChangedFromControl(int id, Direction newDir);
        //void signalChangedFromControl(int id, Aspect newAspect);
        //void zoneChangedFromControl(QString name, bool state);

        //void sendAiguilleUpdate(); //// KEJGBKJEGOEGJBEKGB TROP DE TRUCS A FAIRE
        //void sendZoneUpdate();


    public slots :
        //void updateSigComboBox(int signalId, Aspect newAspect);
        void updateSignalOnControl(int id, Aspect newAspect);
        void updateZoneOnControl(QString name, bool state);
        void updateAiguilleOnControl(int id, Direction newDir);

};

#endif //CONTROL_H