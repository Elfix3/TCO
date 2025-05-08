#include "control.h"

Control::Control(QWidget *parent) : QMainWindow(parent), ui(new Ui::CONTROL) {
    ui->setupUi(this);


    
}

Control::~Control() {
    delete ui;
}

void Control::onComboBoxIndexChanged()
{
}

void Control::updateComboBox(const QString &data){
    /* if(data=="/C1"){
        int index = ui->Sig1choice->findText("Voie libre");
        if(index != -1) ui->Sig1choice->setCurrentIndex(index);
    }
    if(data=="/C2"){
        int index = ui->Sig1choice->findText("Avertissement");
        if(index != -1) ui->Sig1choice->setCurrentIndex(index);
    }
    if(data=="/C3"){
        int index = ui->Sig1choice->findText("Sémaphore");
        if(index != -1) ui->Sig1choice->setCurrentIndex(index);
    } */
}


