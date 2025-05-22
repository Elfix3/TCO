#include "control.h"
#include <QMessageBox>

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

    //finds every combobox with the object name comboSig{id of the signal}
    const auto combos = findChildren<QComboBox*>(QRegularExpression("comboSig\\d+"));
    for (QComboBox* combo : combos) {
        
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [=](int) {
                    QString name = combo->objectName(); // comboSig42
                    int id = name.mid(QString("comboSig").length()).toInt();
                    Aspect asp = combo->currentData().value<Aspect>();
                    isUserUpdate = true;
                    emit signalChangedFromControl(id,asp);
                    isUserUpdate =false;
                    qDebug()<< isUserUpdate;
                });
    }

    //finds all the frames with the object name starting with Z for the zones
    const auto framesZones = findChildren<QFrame*>(QRegularExpression("^Z"));

    for(QFrame *frame : framesZones){
        QString zoneName = frame->objectName().mid(1); //removes the Z in front of the zone name
        
        //ptrs to store the qRadioButtons
        QRadioButton* radioOn = nullptr;
        QRadioButton* radioOff = nullptr; 

        //finds all the radio buttons with On and Off texts in the zone frame
        for(QRadioButton *radio : frame->findChildren<QRadioButton*>()){
            if(radio->text().toLower() == "on"){
                radioOn = radio;
            } else if(radio->text().toLower() == "off"){
                radioOff = radio;
            }
        }
        //connects the radioButtonOn press with its signal 
        connect(radioOn,&QRadioButton::toggled,this,[=](bool checked){
            
            isUserUpdate = true; 
            emit zoneChangedFromControl(zoneName,true);
            isUserUpdate = false;
        });

        //connects the radioButtonOff press with its signal 
        connect(radioOff,&QRadioButton::toggled,this,[=](bool checked){
            if(!checked);
            isUserUpdate = true;
            emit zoneChangedFromControl(zoneName,false);
            isUserUpdate = false;
        });

        //connects the press of the QpushButton with the press of allOn and allOff buttons
        connect(ui->allOn,&QPushButton::clicked,radioOn,&QRadioButton::toggled);
        connect(ui->allOff,&QPushButton::clicked,radioOff,&QRadioButton::toggled);
    }

    
    connect(ui->BALdisabled,&QRadioButton::clicked,this,&Control::BALisDisabled);
    connect(ui->BALEnabled,&QRadioButton::clicked,this,&Control::BALisEnabled);
    
    QRadioButton *gauche = ui->gauche;
    QRadioButton *droie = ui->droite;



    const auto framesAiguilles = findChildren<QFrame*>(QRegularExpression("Aig\\d+"));
    for(QFrame *frame : framesAiguilles){
        bool isValidId = false;
        QString frameName = frame->objectName();
        int id = frameName.mid(3).toInt(&isValidId);
        if(!isValidId){
            qWarning() << "Error ! Id not recognized";
            return;
        }



        QRadioButton *droite = nullptr;
        QRadioButton *gauche = nullptr;
        QLabel *labelAiguille = nullptr;

        for(QRadioButton *radio : frame->findChildren<QRadioButton*>()){
            if(radio->text().toLower() == "droite"){
                    droite = radio;
                } else if(radio->text().toLower() == "gauche"){
                    gauche = radio;
            }
        }

        labelAiguille = frame->findChild<QLabel*>();


        connect(droite,&QRadioButton::toggled,this,[=](bool checked){
            if (checked) {
                emit sendAiguilleImpulse(id,DROITE);


                QString message = QString("L'%1 est elle bien en direction de droite ?").arg(labelAiguille->text());
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Changement aiguille", 
                message,
                QMessageBox::Yes|QMessageBox::No);                
            
                if(reply == QMessageBox::Yes) {
                    emit aiguilleChangedFromControl(id,DROITE);
                } else {
                    gauche->blockSignals(true);
                    gauche->setChecked(true);  // On décoche si annulation
                    gauche->blockSignals(false);
                    qDebug() << labelAiguille->text() << "non mise a jour, veuillez reessayer";
                }
            }
        });
        connect(gauche,&QRadioButton::toggled,this,[=](bool checked){
            if (checked) {
                QString message = QString("L'%1 est elle bien en direction de droite ?").arg(labelAiguille->text());
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Changement aiguille", 
                message,
                QMessageBox::Yes|QMessageBox::No);                  
                
                if(reply == QMessageBox::Yes) {
                    emit aiguilleChangedFromControl(id,GAUCHE);
                } else {
                    droite->blockSignals(true);
                    droite->setChecked(true);  // On décoche si annulation
                    droite->blockSignals(false);
                    qDebug() << labelAiguille->text() << "non mise a jour, veuillez reessayer";
                }
            }
        });
    }

}
 



void Control::loadMaquette(MaquetteHandler *maquette){
    //initialise les combobox pour les signaux :
     //maps the text to the actual aspect of the signal
/* 
    useless ????

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
    } */

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
    if(isUserUpdate){
        qDebug() <<"User update, no need to update again";
        return;
    }

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
    QFrame *frame = findChild<QFrame*>(QString("Aig%1").arg(id));
    if(!frame){
        qWarning() << "Frame aiguille" << id << "not found";
        return;
    }

    QRadioButton *gauche = nullptr;
    QRadioButton *droite = nullptr;
    for(QRadioButton* button : frame->findChildren<QRadioButton*>()){
        if(button->text().trimmed().toLower() == "gauche"){
            gauche = button;
        }
        if(button->text().trimmed().toLower() == "droite"){
            droite = button;
        }
    }

    if(!gauche || !droite) {
        qWarning() << "Button not correctly found";
        return;
    }


    QSignalBlocker bk1(gauche);
    QSignalBlocker bk2(droite);
    
    bool dir = (newDir == GAUCHE);

    gauche->setChecked(dir);
    droite->setChecked(!dir);
}

void Control::setUpBALstatus(bool status){
    if(status){
        ui->BALEnabled->blockSignals(true);
        ui->BALEnabled->toggle();
        ui->BALEnabled->blockSignals(false);
    } else {
        ui->BALdisabled->blockSignals(true);
        ui->BALdisabled->toggle();
        ui->BALdisabled->blockSignals(false);
    }
}


