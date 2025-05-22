#ifndef GUI_H
#define GUI_H

//such mess, please clean

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QPalette>
#include <QMessageBox>
#include <QTransform>

#include "ui_gui.h"
#include "maquetteHandler.h"
#include "signalView.h"
#include "aiguilleView.h"
#include "lightSignal.h"
#include "zoneView.h"



struct PositionAngle{
    QPoint position;
    int angle;
};

class Gui : public QMainWindow {
    public:
        explicit Gui(QWidget *parent = nullptr);
        ~Gui();
        Ui::GUI* getUI();
    
        void INIT();

        void loadMaquette(MaquetteHandler* handler);
        
    protected :
        //void changeEvent(QEvent *event);
        void resizeEvent(QResizeEvent *event) override;
        
    private:
        Ui::GUI *ui;
        

        QMap<int, PositionAngle> signalPositions = {
        
        /*VOIE 1*/
        {1,PositionAngle{QPoint(15,168),22}},       //OK
        {3,PositionAngle{QPoint(322,3),90}},        //OK
        {5,PositionAngle{QPoint(710,3),90}},        //OK
        {7,PositionAngle{QPoint(1130,90),120}},     //OK
        {9,PositionAngle{QPoint(1190,522),210}},    //OK
        {11,PositionAngle{QPoint(880,635),270}},    //OK
        {13,PositionAngle{QPoint(360,635),270}},    //OK
        {15,PositionAngle{QPoint(60,505),310}},     //OK

        /*VOIE 2*/
        {2,PositionAngle{QPoint(176,400),160}},     //OK
        {4,PositionAngle{QPoint(394,462),90}},      //OK
        {6,PositionAngle{QPoint(780,462),90}},      //OK
        {8,PositionAngle{QPoint(1073,290),355}},    //OK
        {10,PositionAngle{QPoint(855,176),270}},    //OK
        {12,PositionAngle{QPoint(430,176),270}},    //OK

        /*IPCS VOIE 1*/
        {17,PositionAngle{QPoint(175,50),250}},     //OK
        {19,PositionAngle{QPoint(-28,360),180}},    //OK
        {21,PositionAngle{QPoint(180,628),118}},    //OK
        {27,PositionAngle{QPoint(1263,350),10}},    //OK
        {23,PositionAngle{QPoint(693,644),90}},     //OK  
        {25,PositionAngle{QPoint(1050,595),75}},    //OK
        {29,PositionAngle{QPoint(970,-7),282}},     //OK
        {31,PositionAngle{QPoint(560,-5),270}},     //OK
        
        /*IPCS VOIE 2*/
        {14,PositionAngle{QPoint(245,440),300}},    //OK
        {16,PositionAngle{QPoint(187,240),20}},     //OK
        {18,PositionAngle {QPoint(645,184),90}},    //OK
        {20,PositionAngle{QPoint(996,188),100}},    //OK
        {22,PositionAngle{QPoint(1020,420),220}},   //OK
        {24,PositionAngle{QPoint(575,455),270}},    //OK
        };
        
        QMap<int, PositionAngle> aiguillePositions = {
        {1,PositionAngle{QPoint(438,130),0}}, //OK for placement... id ???
        {2,PositionAngle{QPoint(480,188),180}}, //OK for placement... id ???
        {3,PositionAngle{QPoint(805,130),0}}, //OK for placement... id ???
        {4,PositionAngle{QPoint(874,200),180}},
        {5,PositionAngle{QPoint(940,130),180}},

        {6,PositionAngle{QPoint(438,648),0}},
        {7,PositionAngle{QPoint(480,590),180}},
        {8,PositionAngle{QPoint(805,648),0}},
        {9,PositionAngle{QPoint(874,578),0}},
        {11,PositionAngle{QPoint(940,648),180}},  
        };

        QMap<QString,QPoint> zonePositions = {
            
        };

        

    signals:
        //void sendSerialData(const QString &data);
        
    private slots :
        void slotMethod();
        void buttonPressed();
        void hideIPCS(bool isHidden);



};

#endif //GUI_H