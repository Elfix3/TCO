#include "serialHandler.h"

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent), mySerialPort(new QSerialPort(this))
{
    mySerialPort->setDataBits(QSerialPort::Data8);
    mySerialPort->setParity(QSerialPort::NoParity);
    mySerialPort->setStopBits(QSerialPort::OneStop);
    mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
    mySerialPort->setBaudRate(9600);
    //slots
    connect(mySerialPort, &QSerialPort::readyRead, 
        this, &SerialHandler::readData);
    connect(mySerialPort, &QSerialPort::errorOccurred, 
        this, &SerialHandler::handleError);
    
    if (!mySerialPort) {
        emit errorOccurred("Serial port initialization failed");
        qDebug() << "Serial port initialization failed!";
    } else {
        qDebug() << "<----Serial port initialized---->";
    }
}

SerialHandler::~SerialHandler(){
    closeSerial();
    delete mySerialPort;
}

bool SerialHandler::openSerial(const QString &portName, qint32 baudRate){
    mySerialPort->setPortName(portName);

    if(mySerialPort->open(QIODevice::ReadWrite)){
        qDebug() << "Port opened : " <<portName;
        return true;
    } else {
        emit errorOccurred(mySerialPort->errorString());
        qDebug() << "Port openning"<< portName << "failed";
        return false;
    }
}

void SerialHandler::closeSerial(){
    if(mySerialPort->isOpen()){
        mySerialPort->close();
        qDebug() << "Closing of the serial port";
    }
}

void SerialHandler::writeData(const QString &data){
    qDebug()<< "Write  data  called";
    if(!mySerialPort->isOpen()){
        emit errorOccurred("Error : serial port not open");
        return;
    }

    QByteArray byteArray = data.toUtf8();
    
    if(mySerialPort->write(byteArray) == -1){
        emit errorOccurred("Error : incorrect writing on serial port");
    }
} 

QString SerialHandler::readData() {
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
    if (error == QSerialPort::NoError)
        return;

    emit errorOccurred("Erreur occured : " + mySerialPort->errorString());
}