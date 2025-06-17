#include "serialHandler.h"

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent),mySerialA(nullptr),mySerialB(nullptr){  
        qInfo()<<"<----------Serial Handler created---------->\n";
}


SerialHandler::~SerialHandler(){
    closeSerial(); //closes all the serial communication before deleting internal pointers
    delete mySerialA;
    delete mySerialB;
}

bool SerialHandler::INIT(){

    //finds every serialPort infos on the device
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    
    qDebug() << "-> Scanning serial  ports...\n";
    for(const QSerialPortInfo &port : ports){
        QSerialPort *tempPort = new QSerialPort(port,this);
        //tries to open each serialPort on the device in ReadWrite  mode

        if(tempPort->open(QIODevice::ReadWrite)){
            
            //sets the port communication to send the id request
            tempPort->setBaudRate(9600);
            tempPort->setParity(QSerialPort::NoParity);
            tempPort->setDataBits(QSerialPort::Data8);
            tempPort->setStopBits(QSerialPort::OneStop);
            tempPort->setFlowControl(QSerialPort::NoFlowControl);

            //waits the reboot for the arduino
            // *** AJOUTER ICI LE RESET ***
            /* tempPort->setDataTerminalReady(false);
            QThread::msleep(100);  // petit délai pour décharger le condo de reset
            tempPort->setDataTerminalReady(true);
            QThread::msleep(300);  // attendre le reboot de l’Arduino (~250 ms pour Uno)
            //sends Id request for the device identification; */

            QSerialPort resetPort;
            resetPort.setPort(port);
            resetPort.setBaudRate(9600);  // la vitesse importe peu
            if (resetPort.open(QIODevice::ReadWrite)) {
                resetPort.setDataTerminalReady(false);
                QThread::msleep(100);
                resetPort.setDataTerminalReady(true);
                resetPort.close();
                QThread::msleep(1000);  // attendre le démarrage complet de l’Arduino
            }
                        
            tempPort->write("ID_REQUEST\n");
            
            //waits for the response of the arduino
            if (tempPort->waitForReadyRead(300)) {
                QByteArray response = tempPort->readAll();
                while (tempPort->waitForReadyRead(50)) {
                    response += tempPort->readAll(); //identification response QByteArray
                }

                if(response.contains("Arduino_A") && !mySerialA){
                    mySerialA = tempPort; //Arduino plaque A found
                    //mySerialB = tempPort; //TO DELETE !!!!!
                    qDebug() << "  Arduino A found on port" << port.portName().toStdString().c_str();
                    continue;
                }else if(response.contains("Arduino_B") && !mySerialB){
                    mySerialB = tempPort; //Arduino plaque B found
                    qDebug() << "  Arduino B found on port" << port.portName().toStdString().c_str();
                
                    continue;
                }

                else {
                    tempPort->close();
                    qWarning() << "Unidentified device on "<< port.portName(); //if the programm didn't recieve the correct answer from arduino
                }
            }

        }
        delete tempPort;

    }

    //problem in the detection
    if(!mySerialA || !mySerialB){
        QString errorMessage;
        if(mySerialA){
            errorMessage = "  Arduino B not detected";
        } else if(mySerialB){
            errorMessage = "  Arduino A not detected";
        } else {
            errorMessage = "No arduino detected";
        }
        qDebug() << errorMessage.toStdString().c_str();

        //to disable at app use :)

        //QMessageBox::critical(nullptr,"Error : ",errorMessage,QMessageBox::Ok,QMessageBox::Ok);
        //qFatal("End of the program");
        return false;
    }
    
    else {
        qDebug() << "\033[1;32m\n -> Successful initialization of serial COM\n\033[0m";
        connect(mySerialA,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoA);
        connect(mySerialB,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoB);
        return true;
    }

    //for test purposes to delete :
    //connect(mySerialA,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoA);
    return false;
}


void SerialHandler::closeSerial(){
    if(mySerialA->isOpen() && mySerialA){
        mySerialA->close();
        qDebug() << "Closing of the serial A port";
    }
    delete mySerialA;

    if(mySerialB->isOpen() && mySerialB){
        mySerialB->close();
        qDebug() << "Closing of the serial B port";
    }
    delete mySerialB;
}

void SerialHandler::readDataFromArduinoA(){
    bufferA.append(mySerialA->readAll());
    processBuffer(&bufferA);
}

void SerialHandler::readDataFromArduinoB(){
    bufferB.append(mySerialB->readAll());
    processBuffer(&bufferB);
}

void SerialHandler::processBuffer(QByteArray *buffer){
    int endIndex;
    while ((endIndex = buffer->indexOf("\r\n")) != -1) {
        QString message = QString(buffer->left(endIndex).trimmed());
        buffer->remove(0, endIndex + 2);
        
        if(message.startsWith("/")){
            qDebug() <<((buffer == &bufferA) ? "\033[93m[Arduino A]  \033[0m :":"\033[93m[Arduino B]  \033[0m :") << message;
            /* if(buffer == &bufferB){
                qDebug() << "OUI";
                qDebug() << message;
            } */
            emit commandReady(message);
        } else {
            qWarning() << "Error : invalid command, must start with /";
        }
            
    }
}


void SerialHandler::writeData(const QString &data, Arduino myArduino){
    //conversion of the data
    QByteArray byteArray = data.toUtf8();
    
    //sends to arduino A
    if(myArduino == Ard_A && mySerialA && mySerialA->isOpen()){
        if(mySerialA->write(byteArray)==-1){
            emit errorOccurred("Error : incorrect writing on the serial port A");
        }
    }

    //sends to arduino B
    else if(myArduino == Ard_B && mySerialB && mySerialB->isOpen()){
        if(mySerialB->write(byteArray)==-1){    
            emit errorOccurred("Error : incorrect writing on the serial port A");
        }
    }

} 



/* void SerialHandler::handleReadyRead(){
    const QByteArray data = mySerialPort->readAll();
}
 */

void SerialHandler::handleError(QSerialPort::SerialPortError error)
{
    /* if (error == QSerialPort::NoError)
        return;

    emit errorOccurred("Erreur occured : " + mySerialPort->errorString()); */
}

void SerialHandler::sendCommandSignal(int id, Aspect aspect){
    QString command = "/sig_" + QString::number(id) + "_" + QString::fromStdString(LightSignal::toString(aspect))+ "\n";
    if(idSignalsOnArdA.contains(id) && !idSignalsOnArdB.contains(id)){//signal id detected on arduino A
        writeData(command,Ard_A);
        qDebug() << "Command sent on Arduino A: " << command;
    } else if(!idSignalsOnArdA.contains(id) && idSignalsOnArdB.contains(id)){
        qDebug() << "Command sent on Arduino B: " << command;
        writeData(command,Ard_B);
    } else {
        qWarning() << "Error : signal " <<id << "not found on the arduinos, no command has been sent";
    }

}

void SerialHandler::sendCommandAiguille(int id, Direction direction){
    QString command = "/aig_" + QString::number(id) + "_" +(direction==DROITE ? "DROITE": "GAUCHE")+ "\n";
    qDebug() << "Command sent on Arduino B :" <<command;
    writeData(command,Ard_B);
    
}

void SerialHandler::sendCommandZone(QString name, bool state){
    QString command = "/Z_"+name+"_"+(state==1 ? "ON" : "OFF")+"\n";
    if(nameZoneOnArdA.contains(name) && !nameZoneOnArdB.contains(name)){
        qDebug() << "Command sent on Arduino A : " << command;
        writeData(command,Ard_A);
    } else if(!nameZoneOnArdA.contains(name) && nameZoneOnArdB.contains(name)){
        qDebug() << "Command sent on Arduino B : " << command;
        writeData(command,Ard_B);
    }
}


