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
        {2,PositionAngle{QPoint(158,390),140}}, //OK
        {9,PositionAngle{QPoint(1178,500),210}} //OK

        };
        
        QMap<int, PositionAngle> aiguillePositions = {
        {1,PositionAngle{QPoint(438,130),0}}, //certified OK
        };

        

    signals:
        void sendSerialData(const QString &data);
        
    private slots :
        void slotMethod();
        void buttonPressed();



};

#endif //GUI_H