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




struct PositionAngle{
    QPoint position;
    int angle;
};

class Gui : public QMainWindow {
    public:
        explicit Gui(QWidget *parent = nullptr);
        ~Gui();
        Ui::GUI* getUI();
    


        void loadMaquette(MaquetteHandler* handler);
        
    protected :
        //void changeEvent(QEvent *event);
        void resizeEvent(QResizeEvent *event) override;
        
    private:
        Ui::GUI *ui;
        

        QMap<int, PositionAngle> signalPositions = {
        {1,PositionAngle{QPoint(30,190),22}}, //OK
        {3,PositionAngle{QPoint(322,3),90}}, //OK
        {5,PositionAngle{QPoint(710,3),90}}, //OK
        {7,PositionAngle{QPoint(1130,90),120}}, //OK
        {9,PositionAngle{QPoint(1178,500),210}},//OK
        {11,PositionAngle{QPoint(880,635),270}}, //OK
        {13,PositionAngle{QPoint(360,635),270}}, //OK
        {15,PositionAngle{QPoint(130,565),300}}, //OK
        
        {2,PositionAngle{QPoint(154,390),150}}, //OK
        {4,PositionAngle{QPoint(394,462),90}}, //OK
        {6,PositionAngle{QPoint(780,462),90}}, //OK
        
        {8,PositionAngle{QPoint(1090,300),350}}, //OK
        {10,PositionAngle{QPoint(855,176),270}}, //OK
        {12,PositionAngle{QPoint(430,176),270}}, //OK
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
        {10,PositionAngle{QPoint(940,648),180}},
    };

        

    signals:
        void sendSerialData(const QString &data);
        
    private slots :
        void slotMethod();
        void buttonPressed();



};

#endif //GUI_H