#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

//Qt include
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialHandler : public QObject{
    Q_OBJECT
    
    public:
        SerialHandler(QObject *parent=nullptr);
        ~SerialHandler();

        //Open and write port
        bool openSerial(const QString &portName, qint32 baudRate = QSerialPort::Baud9600);
        void closeSerial();

        //write sumData
        void writeData(const QString &data);

        //read sumData
        QString readData();

    signals:
        void dataReceived(const QString &data);
        void errorOccurred(const QString &error);
        //void portOpened(bool status);

    private  slots:
        //void handleReadyRead();
        void handleError(QSerialPort::SerialPortError error);
        
    private:
        QSerialPort *mySerialPort;
        QByteArray mySerialBuffer;
    
};

#endif