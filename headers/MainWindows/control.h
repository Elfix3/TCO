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
        
        void SetupConnections();

        void loadMaquette(MaquetteHandler* maquette);

    private:
        Ui::CONTROL *ui;

        void loadSignalQvariant();
        //loadAiguillesQVAR ?
        //loadZoneQVAR ?
        
    
    signals:
        void signalUpdateFromCombo(int signalId, Aspect newAspect);

    public slots :
        //void slotMethod();

    private slots :
        void onComboChange(int index);


};

#endif //CONTROL_H