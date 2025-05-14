#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

//Qt include
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>


class SerialHandler : public QObject{
    Q_OBJECT
    
    public:
        //Finds and opens the serial port of arduino A and B
        SerialHandler(QObject *parent=nullptr);
        ~SerialHandler();

        
        void closeSerial();

        //write some data, 
        void writeData(const QString &data);

        //read sumData
        QString readData(QSerialPort *mySerialPort);

    signals:
        void dataReceived(const QString &data);
        void errorOccurred(const QString &error);
        //void portOpened(bool status);

    private  slots:
        //void handleReadyRead();
        void handleError(QSerialPort::SerialPortError error);
        
    private:
        QSerialPort *mySerialA; //need to create another SP to manage 2 arduinos
        QSerialPort *mySerialB;
        QByteArray mySerialBuffer;
    
};

#endif