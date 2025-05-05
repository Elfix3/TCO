//Qt includes
#include <QApplication>
#include <QMainWindow>

//file includes
#include "MainController.h"



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainController myController;


    //Signal info example :
    //LightSignal *mySig = new LightSignal(1,SAVL);
    //mySig->info();
    
    

    myController.showControl();
    //myController.showGui();
    return app.exec();
}
