#include "control.h"


Control::Control(QWidget *parent) : QMainWindow(parent), ui(new Ui::CONTROL) {
    ui->setupUi(this);
    
    
    SetupConnections();
}

Control::~Control() {
    delete ui;
}

void Control::SetupConnections(){
    //Signals
    const auto combos = findChildren<QComboBox*>(QRegularExpression("comboSig\\d+"));
    
    for (QComboBox* combo : combos) {
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &Control::onComboChange);
    }

    //Switches

    //Zones
}
 



void Control::loadMaquette(MaquetteHandler *maquette){
    //initialise les combobox pour les signaux :
    loadSignalQvariant(); //maps the text to the actual aspect of the signal


    for(LightSignal* signal : maquette->getAllSignals()){
        QComboBox* combo = findChild<QComboBox*>(QString("comboSig%1").arg(signal->getId()));
        if(combo){

            int index = combo->findData(QVariant::fromValue(signal->getAspect()));

            combo->blockSignals(true);
            if (index >= 0) {
                combo->setCurrentIndex(index);
            } else {
                qWarning() << "No aspect found in the combo box";
                combo->setCurrentIndex(0);
            }
            combo->blockSignals(false);
        }
    }

}

void Control::loadSignalQvariant(){
    //CALL THIS FUNCTION ONCE !!


    //Maps the text to the selected QVariant
    const QMap<QString, Aspect> textToAspect = {
        {"Voie libre", VL},
        {"Sémaphore", S},
        {"Avertissement", A},
        {"Rappel Ralentissement", RR},
        {"Ralentissement", R},
        {"Carré", C},
        {"Inactif", IDLE}
    };
    const QList<QComboBox*> combos = findChildren<QComboBox*>(QRegularExpression("comboSig\\d+"));
    
    for (QComboBox* combo : combos) {
        // Associez les QVariant à chaque item selon son texte
        for (int i = 0; i < combo->count(); i++) {
            const QString text = combo->itemText(i);
            if(textToAspect.contains(text)){
                combo->setItemData(i,QVariant::fromValue(textToAspect[text]));
            } else {
                qWarning() << "Error, text " << text <<"is not valid in a signal comboBox";
            }
        }
    }

}

void Control::onComboChange(int index){
    Q_UNUSED(index); // On n'utilise pas directement l'index
    
    QComboBox* combo = qobject_cast<QComboBox*>(sender());
    if (combo){
        
        qDebug("changement");
        QString comboName = combo->objectName();
        int signalId = comboName.mid(8).toInt(); // "comboSig123" → 123

        // 2. Récupère la nouvelle valeur
        Aspect newAspect = combo->currentData().value<Aspect>();

        // 3. Émet le signal
        emit signalUpdateFromCombo(signalId, newAspect);
    }

    
    
}


/* void Control::updateComboBox(const QString &data){
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
    } 
}
 */ 