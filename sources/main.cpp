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
    
    //translations for the qinfo box
    QTranslator translator;
    if (translator.load(QLocale::French, "qtbase", "_",
        QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&translator);
    }


    //instance of my main controller
    MainController myController;

    myController.showGui();
    myController.showControl();
    return app.exec();
}
