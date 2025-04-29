#include "aiguilleView.h"

AiguilleView::AiguilleView(Aiguille *aiguille, QWidget *parent)
    : QWidget(parent),aiguille(aiguille){
        setFixedSize(aiguille_width,aiguille_height);
        background.load(":images/aiguille.png");
        if(aiguille->getDirection() == DROITE){
            bulbs.append({QPoint(11,10),QColor(OFF)}); //right Direction
            bulbs.append({QPoint(11,32),QColor(YELLOW)}); //left Direction
        } else {
            bulbs.append({QPoint(11,10),QColor(YELLOW)}); //right Direction
            bulbs.append({QPoint(11,32),QColor(OFF)}); 
        }

}

AiguilleView::~AiguilleView()
{
}

void AiguilleView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,aiguille_width,aiguille_height,background);

    drawBulbs(painter);

    
}

void AiguilleView::drawBulbs(QPainter &painter){
    for(const Bulb &bulb : bulbs){
        painter.setBrush(bulb.color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(bulb.position, 4, 4); // Un petit cercle rayon 5px

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::white, 1.5)); // Pen blanc, épaisseur 2 pixels
        painter.drawEllipse(bulb.position, 4, 4);
    }
            

}

void AiguilleView::onAiguilleUpdate(){

}