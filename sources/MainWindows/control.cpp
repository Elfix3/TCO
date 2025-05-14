#include "control.h"

Control::Control(QWidget *parent) : QMainWindow(parent), ui(new Ui::CONTROL) {
    ui->setupUi(this);

    //setting up the connections :
    /* QList<QComboBox *> allComboBoxes = findChildren<QComboBox*>();
    QRegularExpression re("comboSig\\d+)");
    for(QComboBox* combo : allCombosBoxes) {
        QRegularExpressionMatch match = re.match(combo->objectName());
        if(match.hasMatch()) {
            // Extraire l'ID numérique
            int signalId = match.captured(1).toInt();
            
            // Connecter le signal
            connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                   [this, signalId](int index) {
                       onSignalComboIndexChanged(signalId, index);
                   });
        }
    } */
}

Control::~Control() {
    delete ui;
}

void Control::onSignalComboChanged(){
    
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


