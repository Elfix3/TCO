//Qt includes
#include <QApplication>
#include <QMainWindow>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

//file includes
#include "MainController.h"



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QTranslator translator;
    if (translator.load(QLocale::French, "qtbase", "_",
        QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&translator);
    }




    MainController myController;


    //Signal info example :
    //LightSignal *mySig = new LightSignal(1,SAVL);
    //mySig->info();
    
    

    myController.showGui();
    myController.showControl();
    return app.exec();
}
