#ifndef SIGNALVIEW_H
#define SIGNALVIEW_H

//file includes
#include "lightSignal.h"

//Qt includes
#include <QWidget>
#include <QPainter>


struct Bulb {
    QPoint position;
    QColor color; // on pourra aussi ajouter "allumé/éteint" plus tard
};


//color palette
#define GRAY            0x808080
#define OFF             0x400b06
#define YELLOW          0xFFFF00
#define RED             0xFF0000
#define GREEN           0x00FF00
#define WHITE           0xFFFFFF
#define BLACK           0x000001




class SignalView : public QWidget{
    Q_OBJECT
    public :
        explicit SignalView(LightSignal* signal, QWidget *parent = nullptr);
        ~SignalView();

        void setAngle(int degrees);

    protected:
        void paintEvent(QPaintEvent *event) override;

        
    private slots:
        void onSignalUpdate();
    
    private:
        LightSignal *signal;
        QPixmap background;
        
        QVector<Bulb> bulbs;
        int rotationAngle;
        QSize originalSize;
        
        
        void loadBackGround();
        void resetBulbs();
        void drawBulbs(QPainter &painter);



        const int SAVL_width = 41;
        const int SAVL_height = 55;
        const int SAVLR_width = 41;
        const int SAVLR_height = 71;
        const int CSAVLRRR_width = 51;
        const int CSAVLRRR_height = 91;
};
#endif //SIGNALVIEW_H