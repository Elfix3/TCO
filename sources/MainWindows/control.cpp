#include "control.h"


Control::Control(QWidget *parent) : QMainWindow(parent), ui(new Ui::CONTROL) {
    ui->setupUi(this);
    
    loadSignalQvariant(); //maps the combobox text to a Aspect Argument
    SetupConnections();
}

Control::~Control() {
    delete ui;
}

void Control::SetupConnections(){
    //This function is very important and links the GUI elements to the internal slots of the control class.
    //Then the slots are sending signals to the other classes

    //comboBoxes and Signals
   
    const auto combos = findChildren<QComboBox*>(QRegularExpression("comboSig\\d+"));
    for (QComboBox* combo : combos) {
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [=](int) {
                    QString name = combo->objectName(); // comboSig42
                    int id = name.mid(QString("comboSig").length()).toInt();
                    Aspect asp = combo->currentData().value<Aspect>();
                    emit sendSignalUpdate(id, asp);  // ton signal personnalisé
                });
    }

}
 



void Control::loadMaquette(MaquetteHandler *maquette){
    //initialise les combobox pour les signaux :
     //maps the text to the actual aspect of the signal


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


void Control::updateSignalOnControl(int id, Aspect newAspect){
    QComboBox* combo = findChild<QComboBox*>(QString("comboSig%1").arg(id));
    if (!combo) {
        qWarning() << "ComboBox for signal ID" << id << "not found.";
        return;
    }
    int index = combo->findData(QVariant::fromValue(newAspect));
    if (index < 0) {
        qWarning() << "Aspect not found in combo box for signal ID" << id;
        return;
    }
    // Empêche temporairement le signal d’être émis, utile ??????
    combo->blockSignals(true);
    combo->setCurrentIndex(index);
    combo->blockSignals(false);
}

void Control::updateZoneOnControl(QString name, bool state){
    QFrame *frame= findChild<QFrame*>(QString("Z%1").arg(name));
    if (!frame) {
        qWarning() << "Frame Z" << name << "non trouvée";
        return;
    }
    QRadioButton *buttonOn = nullptr;
    QRadioButton *buttonOff = nullptr;
    for(QRadioButton* button : frame->findChildren<QRadioButton*>()){
        if(button->text().trimmed().toLower() == "on"){
            buttonOn = button;
        }
        if(button->text().trimmed().toLower() == "off"){
            buttonOff = button;
        }
    }

    if(!buttonOn || !buttonOff) {
        qWarning() << "Button not correctly found";
        return;
    }
    QSignalBlocker bk1(buttonOn);
    QSignalBlocker bk2(buttonOff);
    buttonOn->setChecked(state);
    buttonOff->setChecked(!state);
    
}

void Control::updateAiguilleOnControl(int id, Direction newDir){

}

/* void Control::updateSigComboBox(int signalId, Aspect newAspect){
    QComboBox* combo = findChild<QComboBox*>(QString("comboSig%1").arg(signalId));
    if (!combo) {
        qWarning() << "ComboBox for signal ID" << signalId << "not found.";
        return;
    }

    // Trouve l’index correspondant au nouvel aspect
    int index = combo->findData(QVariant::fromValue(newAspect));
    if (index < 0) {
        qWarning() << "Aspect not found in combo box for signal ID" << signalId;
        return;
    }

    // Empêche temporairement le signal d’être émis
    combo->blockSignals(true);
    combo->setCurrentIndex(index);
    combo->blockSignals(false);

} */


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