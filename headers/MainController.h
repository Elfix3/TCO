#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H


//this file has the purpose to link all the other components of this app



//file includes
#include "gui.h"
#include "control.h"
#include "maquettehandler.h"
#include "serialhandler.h"

//Qt includes
#include <QObject>


class MainController : public QObject {
    Q_OBJECT

    public:
        MainController(QObject* parent=nullptr);
        ~MainController();
    
        void showGui();
        void showControl();
        void RESET();

    private slots:

        void manageButton();

    private:
        Gui *myGui;
        Control *myControl;
        MaquetteHandler *myMaquetteHandler;
        SerialHandler *mySerialHandler;

        bool buttonState = false;
        
        //all my tests setups
        void dumbTestSetup();
        
};




#endif // MAIN_CONTROLLER_H