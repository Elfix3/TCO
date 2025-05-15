#include "aiguilleView.h"

AiguilleView::AiguilleView(Aiguille *aiguille, QWidget *parent)
    : QWidget(parent),aiguille(aiguille), isFlipped(false){
        setFixedSize(aiguille_width,aiguille_height);
        background.load(":images/aiguille.png");
        
        bulbs.append({QPoint(10,8),QColor(OFF)}); //right Direction
        bulbs.append({QPoint(10,34),QColor(OFF)}); //left Direction
        

    connect(aiguille, &Aiguille::positionChanged, this, &AiguilleView::onAiguilleUpdate);
    
    onAiguilleUpdate();
}

AiguilleView::~AiguilleView()
{
}

void AiguilleView::flipAiguille(){
    isFlipped = !isFlipped;
    onAiguilleUpdate();
}

void AiguilleView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,aiguille_width,aiguille_height,background);

    drawBulbs(painter);

    
}

void AiguilleView::drawBulbs(QPainter &painter){
    for(Bulb &bulb : bulbs){
        painter.setBrush(bulb.color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(bulb.position, 4, 4); // Un petit cercle rayon 5px

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::white, 1.5)); // Pen blanc, épaisseur 2 pixels
        painter.drawEllipse(bulb.position, 4, 4);
    }
            

}

void AiguilleView::onAiguilleUpdate(){
    Direction dir = aiguille->getDirection();
    
    if((isFlipped && dir == DROITE) || (!isFlipped && dir == GAUCHE)){
        bulbs[0].color = QColor(YELLOW);
        bulbs[1].color = QColor(OFF);
    } else {
        bulbs[0].color = QColor(OFF);
        bulbs[1].color = QColor(YELLOW);
        
    }
    
    update();

    
    //mystère et boule de gomme...
}