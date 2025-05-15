#include "serialHandler.h"

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent),mySerialA(nullptr),mySerialB(nullptr){  
}


SerialHandler::~SerialHandler(){
    closeSerial(); //closes all the serial communication before deleting internal pointers
    delete mySerialA;
    delete mySerialB;
}

void SerialHandler::INIT(){
    
    //finds every serialPort found on the device
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    
    for(const QSerialPortInfo &port : ports){
        QSerialPort *tempPort = new QSerialPort(port,this);
        //tries to open each serialPort on the device
        if(tempPort->open(QIODevice::ReadWrite)){
            
            //sets the port communication foor
            tempPort->setBaudRate(9600);
            tempPort->setParity(QSerialPort::NoParity);
            tempPort->setDataBits(QSerialPort::Data8);
            tempPort->setStopBits(QSerialPort::OneStop);
            tempPort->setFlowControl(QSerialPort::NoFlowControl);

            //sends Id request for the device identification;
            tempPort->write("ID_REQUEST\n");
           
            
            //waites for the response of the arduino
            if (tempPort->waitForReadyRead(300)) {
                QByteArray response = tempPort->readAll();
                while (tempPort->waitForReadyRead(50)) {
                    response += tempPort->readAll(); //identification response QByteArray
                }

                if(response.contains("Arduino_A") && !mySerialA){
                    mySerialA = tempPort; //Arduino plaque A found
                    qDebug() << "Arduino A found on port" << port.portName().toStdString().c_str();;
                    continue;
                }else if(response.contains("Arduino_B") && !mySerialB){
                    mySerialB = tempPort; //Arduino plaque B found
                    qDebug() << "Arduino B found on port" << port.portName().toStdString().c_str();
                    continue;
                }

                else {
                    tempPort->close();
                    qWarning() << "Unidentified device on "<< port.portName();
                }
            }

        }
        delete tempPort;
    }

    //problem in the detection
    if(!mySerialA || !mySerialB){
        QString errorMessage;
        
        if(mySerialA){
            errorMessage = "Arduino B not detected";
        } else if(mySerialB){
            errorMessage = "Arduino A not detected";
        } else {
            errorMessage = "No arduino detected";
        }
        qDebug() << errorMessage.toStdString().c_str();

        //to disable at app use :)

        //QMessageBox::critical(nullptr,"Error : ",errorMessage,QMessageBox::Ok,QMessageBox::Ok);
        //qFatal("End of the program");
    }
    
    else {
        qDebug() << "\n<----Sucessfull initialization of serial A and B communication---->";
    
        connect(mySerialA,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoA);
        connect(mySerialB,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoB);
    }

    //for test purposes to delete :
    connect(mySerialA,&QSerialPort::readyRead,this,&SerialHandler::readDataFromArduinoA);
}


void SerialHandler::closeSerial(){
    if(mySerialA->isOpen() && mySerialA){
        mySerialA->close();
        qDebug() << "Closing of the serial A port";
    }

    if(mySerialB->isOpen() && mySerialB){
        mySerialB->close();
        qDebug() << "Closing of the serial B port";
    }
}

void SerialHandler::readDataFromArduinoA(){
    
    bufferA.append(mySerialA->readAll());
    processBuffer(bufferA);
}

void SerialHandler::readDataFromArduinoB(){
    bufferB.append(mySerialB->readAll());
    processBuffer(bufferB);

    
}

void SerialHandler::processBuffer(QByteArray &buffer){
    int endIndex;
    while ((endIndex = buffer.indexOf("\r\n")) != -1) {
        QString message = QString(buffer.left(endIndex).trimmed());
        buffer.remove(0, endIndex + 2);
        
        if(message.startsWith("/")){
            qDebug() << message;
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
    QString command = "/Z_"+name+"_"+(state==1 ? "ON" : "OFF");
    if(nameZoneOnArdA.contains(name) && !namezoneOnArdB.contains(name)){
        qDebug() << "Command sent on Arduino A : " << command;
        writeData(command,Ard_A);
    } else if(!nameZoneOnArdA.contains(name) && namezoneOnArdB.contains(name)){
        qDebug() << "Command sent on Arduino B : " << command;
        writeData(command,Ard_B);
    }
}


