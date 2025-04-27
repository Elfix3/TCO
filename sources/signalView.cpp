#include "signalView.h"

SignalView::SignalView(LightSignal *signal, QWidget *parent)
    : QWidget(parent) , signal(signal), rotationAngle(0){
    
    //loadBackGround();
    switch(signal->getType()){
        case SAVL :
            setFixedSize(SAVL_width,SAVL_height);
            background.load(":/images/SAVL.png");
            originalSize = QSize(SAVL_width, SAVL_height);

            //bulbs positions on the widget for SAVL
            bulbs.append({QPoint(66,80), QColor(OFF)});
            bulbs.append({QPoint(66,68), QColor(OFF)});
            bulbs.append({QPoint(66,56), QColor(OFF)});

            break;
        case SAVLR :
            setFixedSize(SAVLR_width,SAVLR_height);
            background.load(":/images/SAVLR.png");
            originalSize = QSize(SAVLR_width, SAVLR_height);

            bulbs.append({QPoint(64,90), QColor(OFF)});
            bulbs.append({QPoint(64,78), QColor(OFF)});
            bulbs.append({QPoint(64,66), QColor(OFF)});

            bulbs.append({QPoint(64,50), QColor(OFF)});
            bulbs.append({QPoint(83,50), QColor(OFF)});
            break;
        case CSAVLRR :
            setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
            background.load(":/images/CSAVLRRR.png");
            originalSize = QSize(CSAVLRRR_width, CSAVLRRR_height);

            bulbs.append({QPoint(54,102), QColor(BLACK)});

            bulbs.append({QPoint(68,99), QColor(OFF)});
            bulbs.append({QPoint(68,89), QColor(OFF)});
            bulbs.append({QPoint(68,79), QColor(OFF)});

            bulbs.append({QPoint(68,62), QColor(OFF)});
            
            
            bulbs.append({QPoint(89,62), QColor(OFF)});
            
            bulbs.append({QPoint(89,42), QColor(OFF)});



            //bulbs.append({QPoint(64,90), QColor(OFF)});
            //bulbs.append({QPoint(64,90), QColor(OFF)});
            break;
        case CSAVLRRR :
            setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
            background.load(":/images/CSAVLRRR.png");
            originalSize = QSize(CSAVLRRR_width, CSAVLRRR_height);

            bulbs.append({QPoint(54,102), QColor(BLACK)});

            bulbs.append({QPoint(68,99), QColor(OFF)});
            bulbs.append({QPoint(68,89), QColor(OFF)});
            bulbs.append({QPoint(68,79), QColor(OFF)});

            bulbs.append({QPoint(68,62), QColor(OFF)});
            bulbs.append({QPoint(68,53), QColor(OFF)});
            
            bulbs.append({QPoint(89,62), QColor(OFF)});
            bulbs.append({QPoint(89,52), QColor(OFF)});
            bulbs.append({QPoint(89,42), QColor(OFF)});

            //bulbs.append({QPoint(54,102), QColor(OFF)});

            break;
        default :
           break;
        onSignalUpdate();
    }
    /* background = background.scaled(this->size(), 
    Qt::IgnoreAspectRatio,  // ← C'est la clé
    Qt::SmoothTransformation); */

    connect(signal, &LightSignal::signalUpdated, this, &SignalView::onSignalUpdate);
    //setFixedSize(SAVL_width*3,SAVL_height*3);
    setFixedSize(150,150);
}

SignalView::~SignalView(){

}

void SignalView::setAngle(int degrees)
{
    rotationAngle = degrees;
    update();
}


void SignalView::resetBulbs(){
    for (Bulb &bulb : bulbs) {
        bulb.color = QColor(OFF);
    }
    if(signal->getType() == CSAVLRR || signal->getType() == CSAVLRRR){
        bulbs[0].color = QColor(BLACK);
    }
}

void SignalView::onSignalUpdate(){
    resetBulbs();
    SignalType type = signal->getType();
    switch(signal->getAspect()){
        case VL :
            qDebug()<< "Ici";
            if(type == CSAVLRR || type == CSAVLRRR){
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[3].color = QColor(GREEN);
            } else {
                bulbs[2].color = QColor(GREEN);
            }
            break;
        case A  :
            if(type == CSAVLRR || type == CSAVLRRR){
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[1].color = QColor(YELLOW);
            } else {
                bulbs[0].color = QColor(YELLOW);
            }
            break;
        case S  :
            if(type == CSAVLRR || type == CSAVLRRR){
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[2].color = QColor(RED);
            } else {
                bulbs[1].color = QColor(RED);
            }
            break;
        case C  :
            if(type == CSAVLRR || type == CSAVLRRR){
                bulbs[2].color = QColor(RED);
                bulbs[4].color = QColor(RED);
            } else {/*No changes allowed*/ }
            break;
        case R  :
            if(type == SAVLR){
                bulbs[3].color = QColor(YELLOW);
                bulbs[4].color = QColor(YELLOW);
            } else if(type == CSAVLRRR){
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[5].color = QColor(YELLOW);
                bulbs[7].color = QColor(YELLOW);
            } else {/*No changes allowed*/ }
            break;
    
        case RR :
            if(type == CSAVLRR){
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[5].color = QColor(YELLOW);
                bulbs[6].color = QColor(YELLOW);
            } else if(type == CSAVLRRR){
                
                bulbs[0].color = QColor(WHITE); //oeilleton
                bulbs[6].color = QColor(YELLOW);
                bulbs[8].color = QColor(YELLOW);
            } else {/*No changes allowed*/ }
            break;

        default: //set to idle ?? then nothing to do
            break;
    }


    update();
}

void SignalView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.save();

    QTransform transform;
    transform.translate(width() / 2, height() / 2);
    transform.rotate(rotationAngle);
    transform.translate(-width() / 2, -height() / 2);
    painter.setTransform(transform);

    // Calculer la position de départ pour centrer l'image
    int x = (width() - originalSize.width()) / 2;
    int y = (height() - originalSize.height()) / 2;

    // Dessiner l'image à sa taille originale, centrée
    painter.drawPixmap(x, y, originalSize.width(), originalSize.height(), background);

    //draw bulbs ici :)
    drawBulbs(painter);

    painter.restore();

    //debug purposes
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, width()-1, height()-1);
    
    
}




void SignalView::loadBackGround(){    

}


void SignalView::drawBulbs(QPainter &painter) {
    for(int i = 0; i<bulbs.size();i++){
        if(i==0 && (signal->getType() == CSAVLRR || signal->getType() == CSAVLRRR)){ 
            
            painter.setBrush(bulbs[i].color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(bulbs[i].position, 2, 2);


        } else {
            painter.setBrush(bulbs[i].color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(bulbs[i].position, 4, 4); // Un petit cercle rayon 5px

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(Qt::white, 1.5)); // Pen blanc, épaisseur 2 pixels
            painter.drawEllipse(bulbs[i].position, 4, 4); // Contour
        }
        
        
    }
    
}
