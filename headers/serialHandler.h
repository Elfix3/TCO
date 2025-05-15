#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

//myIncludes
#include "lightSignal.h"
#include "aiguille.h"
#include "zone.h"

//Qt include
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

enum Arduino {Ard_A,Ard_B,BOTH};

class SerialHandler : public QObject{
    Q_OBJECT
    
    public:
        //Finds and opens the serial port of arduino A and B
        SerialHandler(QObject *parent=nullptr);
        ~SerialHandler();

        void INIT();
        void closeSerial();

        
        void readDataFromArduinoA(); //when data is avilable on Arduino A, reads it
        void readDataFromArduinoB(); //when data is available on Arduino B, reads it
        void processBuffer(QByteArray &buffer); //fills the dedicated buffer for A and B, to process command, sends signal data recieved.


        void writeData(const QString &data, Arduino myArduino); //writes on a specific Arduino (BOTH NOT IMPLEMENTED)


        

    public slots :
        void sendCommandSignal(int id, Aspect aspect);
        void sendCommandAiguille(int id, Direction dir);
        void sendCommandZone(QString name, bool state);

        

    signals:
        void commandReady(const QString &data);
        void errorOccurred(const QString &error);
        //void portOpened(bool status);

    
    private  slots:
    
        void handleError(QSerialPort::SerialPortError error);
        
    private:
        QSerialPort *mySerialA; //need to create another SP to manage 2 arduinos
        QSerialPort *mySerialB;

        QByteArray bufferA;
        QByteArray bufferB;


        //Signals on A and signals on B
        QVector<int> idSignalsOnArdA = {1,2,3,4,12,13,14,15,16,17,19,21,24,31};
        QVector<int> idSignalsOnArdB = {5,6,7,8,9,10,11,18,20,22,23,25,27,29};

        //Zones on A and zones on B
        QVector<QString> nameZoneOnArdA = {"11B","13A","13B","15A","15B","1A","1B","3A","3B","4A","2B","2A","12B","12A","10B"};
        QVector<QString> namezoneOnArdB = {"5A","5B","7A","7B","9A","9B","11A","10A","8B","8A","6B","6A","4B"};

        //We consider all aiguilles are on B
};

#endif