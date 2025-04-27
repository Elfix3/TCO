#include "gui.h"
#include <iostream>

#include "maquetteHandler.h"
#include "lightSignal.h"
Gui::Gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::GUI) {
    //importe l'interface depuis le fichier .gui
    ui->setupUi(this);


    this->setWindowTitle("Tableau de commande optique"); //Title of gui
    

    
}

Gui::~Gui(){
    delete ui;
}

Ui::GUI *Gui::getUI(){
    return ui;
}

void Gui::addSignalView(LightSignal *SignalView)
{
}

void Gui::resizeEvent(QResizeEvent *event) {
   //pretty much useless
}



void Gui::buttonPressed() {
    //pretty much useless
}




/* void Gui::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (this->isMaximized()) {
            qDebug() << "Fenêtre en plein écran | Taille :" << this->size();

        } else if (this->isFullScreen()) {
            qDebug() << "Mode plein écran (sans bordure) | Taille :" << this->size();
        } else {
            qDebug() << "Fenêtre restaurée | Taille :" << this->size();
        }
    }
    QMainWindow::changeEvent(event); // Important : appeler l'implémentation parente
} */