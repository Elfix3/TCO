#ifndef AIGUILLEVIEW_H
#define AIGUILLEVIEW_H

//file include
#include "aiguille.h"
#include "signalView.h"
//Qt includes
#include <QWidget>
#include <QPainter>

class AiguilleView : public QWidget{
    Q_OBJECT
    public :
        explicit AiguilleView(Aiguille *aiguille, QWidget *parent = nullptr);
        ~AiguilleView();
    
    protected:
        void paintEvent(QPaintEvent *event) override;
    private slots :
        void onAiguilleUpdate();

    private :
        Aiguille *aiguille;
        QPixmap background;

        QVector<Bulb> bulbs;

        const int aiguille_width = 20;
        const int aiguille_height = 42;


        void drawBulbs(QPainter &painter);
        
};

#endif //AIGUILLEVIEW_H