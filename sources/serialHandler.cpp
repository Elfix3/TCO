#include "serialHandler.h"

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent),mySerialA(nullptr),mySerialB(nullptr)
{
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

    //if(!mySerialA) qFatal() << "No arduino A detected";
    //if(!mySerialB) qFatal() << "No arduino B detected";

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
        QMessageBox::critical(nullptr,"Error : ",errorMessage,QMessageBox::Ok,QMessageBox::Ok);
        qFatal("End of the program");
    }
    
    else {
        qDebug() << "\n<----Sucessfull initialization of serial A and B communication---->";
    }   
}

SerialHandler::~SerialHandler(){
    closeSerial(); //closes all the serial communication before deleting internal pointers
    delete mySerialA;
    delete mySerialB;
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

void SerialHandler::writeData(const QString &data){
    //automatically decides to which arduino the command shoud be sent
    qDebug() << "Write data Called";
    QByteArray byteArray = data.toUtf8();
    if(data == "/C1\n"){
        if(mySerialA->write(byteArray) == -1){
            emit errorOccurred("Error : incorrect writing on serial port");
        }
    }

    if(data == "/C2\n"){
        if(mySerialB->write(byteArray) == -1){
            emit errorOccurred("Error : incorrect writing on serial port");
        }
    }
    
    /* qDebug()<< "Write  data  called";
    if(!mySerialPort->isOpen()){
        emit errorOccurred("Error : serial port not open");
        return;
    }

    QByteArray byteArray = data.toUtf8();
    
    if(mySerialPort->write(byteArray) == -1){
        emit errorOccurred("Error : incorrect writing on serial port");
    } */
} 

QString SerialHandler::readData(QSerialPort *mySerialPort) {
    QByteArray data = mySerialPort->readAll();
    mySerialBuffer.append(data);

    int endIndex = mySerialBuffer.indexOf("\r\n");
    if (endIndex != -1) {
        // Extraire le message complet
        QByteArray message = mySerialBuffer.left(endIndex);
        mySerialBuffer.remove(0, endIndex + 2); // Enlever le message + \r\n

        // Nettoyer et convertir en QString
        QString strData = QString(message.trimmed());
        //qDebug() << (strData == "Patapim");
        emit dataReceived(strData);
        return strData;
    }

    return QString();
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