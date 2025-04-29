#include "signalView.h"

SignalView::SignalView(LightSignal *signal, QWidget *parent)
    : QWidget(parent) , signal(signal), rotationAngle(0){
    
    //loadBackGround();
    bool isIPCS = signal->getisIPCS();
    switch(signal->getType()){
        case SAVL :

            if(isIPCS){
                setFixedSize(SAVL_width,SAVL_height);
                background.load(":/images/SAVLipcs.png");
                originalSize = QSize(SAVL_IPCS_width, SAVL_IPCS_height);
                
                //bulbs positions on the widget for SAVL_IPCS
                bulbs.append({QPoint(105,112), QColor(OFF)});
                bulbs.append({QPoint(105,100), QColor(OFF)});
                bulbs.append({QPoint(105,88), QColor(OFF)});
            } else {
                setFixedSize(SAVL_width,SAVL_height);
                background.load(":/images/SAVL.png");
                originalSize = QSize(SAVL_width, SAVL_height);

                //bulbs positions on the widget for SAVL
                bulbs.append({QPoint(81,96), QColor(OFF)});
                bulbs.append({QPoint(81,84), QColor(OFF)});
                bulbs.append({QPoint(81,72), QColor(OFF)});

            }
            


            break;
        case SAVLR :

            if(isIPCS){
                setFixedSize(SAVLR_width,SAVLR_height);
                background.load(":/images/SAVLRipcs.png");
                originalSize = QSize(SAVLR_IPCS_width, SAVLR_IPCS_height);

                bulbs.append({QPoint(119,105), QColor(OFF)});
                bulbs.append({QPoint(119,93), QColor(OFF)});
                bulbs.append({QPoint(119,81), QColor(OFF)});

                bulbs.append({QPoint(119,64), QColor(OFF)});
                bulbs.append({QPoint(139,64), QColor(OFF)});
            } else {
                setFixedSize(SAVLR_width,SAVLR_height);
                background.load(":/images/SAVLR.png");
                originalSize = QSize(SAVLR_width, SAVLR_height);

                bulbs.append({QPoint(79,105), QColor(OFF)});
                bulbs.append({QPoint(79,93), QColor(OFF)});
                bulbs.append({QPoint(79,81), QColor(OFF)});

                bulbs.append({QPoint(79,64), QColor(OFF)});
                bulbs.append({QPoint(99,64), QColor(OFF)});
                
            }
            break;
            
        case CSAVLRR :
            if(isIPCS){
                setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
                background.load(":/images/CSAVLRRRipcs.png");
                originalSize = QSize(CSAVLRRR_IPCS_width, CSAVLRRR_IPCS_height);

                bulbs.append({QPoint(109,115), QColor(OFF)});

                bulbs.append({QPoint(124,112), QColor(OFF)});
                bulbs.append({QPoint(124,102), QColor(OFF)});
                bulbs.append({QPoint(124,92), QColor(OFF)});

                bulbs.append({QPoint(124,75), QColor(OFF)});

                bulbs.append({QPoint(145,76), QColor(OFF)});

                bulbs.append({QPoint(145,56), QColor(OFF)});
            } else {      
                setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
                background.load(":/images/CSAVLRRR.png");
                originalSize = QSize(CSAVLRRR_width, CSAVLRRR_height);

                bulbs.append({QPoint(70,115), QColor(OFF)});

                bulbs.append({QPoint(85,112), QColor(OFF)});
                bulbs.append({QPoint(85,102), QColor(OFF)});
                bulbs.append({QPoint(85,92), QColor(OFF)});

                bulbs.append({QPoint(85,75), QColor(OFF)});
                


                bulbs.append({QPoint(106,76), QColor(OFF)});
                
                bulbs.append({QPoint(106,56), QColor(OFF)});
        
            }
        break;
        case CSAVLRRR :

            if(isIPCS){
                setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
                background.load(":/images/CSAVLRRRipcs.png");
                originalSize = QSize(CSAVLRRR_IPCS_width, CSAVLRRR_IPCS_height);

                bulbs.append({QPoint(109,115), QColor(OFF)});

                bulbs.append({QPoint(124,112), QColor(OFF)});
                bulbs.append({QPoint(124,102), QColor(OFF)});
                bulbs.append({QPoint(124,92), QColor(OFF)});

                bulbs.append({QPoint(124,75), QColor(OFF)});
                bulbs.append({QPoint(124,66), QColor(OFF)});


                bulbs.append({QPoint(145,76), QColor(OFF)});
                bulbs.append({QPoint(145,66), QColor(OFF)});
                bulbs.append({QPoint(145,56), QColor(OFF)});
            } else {
                setFixedSize(CSAVLRRR_width,CSAVLRRR_height);
                background.load(":/images/CSAVLRRR.png");
                originalSize = QSize(CSAVLRRR_width, CSAVLRRR_height);

                bulbs.append({QPoint(70,115), QColor(OFF)});

                bulbs.append({QPoint(85,112), QColor(OFF)});
                bulbs.append({QPoint(85,102), QColor(OFF)});
                bulbs.append({QPoint(85,92), QColor(OFF)});

                bulbs.append({QPoint(85,75), QColor(OFF)});
                bulbs.append({QPoint(85,66), QColor(OFF)});


                bulbs.append({QPoint(106,76), QColor(OFF)});
                bulbs.append({QPoint(106,66), QColor(OFF)});
                bulbs.append({QPoint(106,56), QColor(OFF)});

            //bulbs.append({QPoint(54,102), QColor(OFF)});
            }
            break;
        
        default :
           break;
        
    }
    /* background = background.scaled(this->size(), 
    Qt::IgnoreAspectRatio,  // ← C'est la clé
    Qt::SmoothTransformation); */

    connect(signal, &LightSignal::signalUpdated, this, &SignalView::onSignalUpdate);
    //setFixedSize(SAVL_width*3,SAVL_height*3);
    setFixedSize(180,180);
    onSignalUpdate();
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

    if(border){
    
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, width()-1, height()-1);
    }
        
}




void SignalView::loadBackGround(){    

}


void SignalView::drawBulbs(QPainter &painter) {
    for(int i = 0; i<bulbs.size();i++){
        if(i==0 && (signal->getType() == CSAVLRR || signal->getType() == CSAVLRRR)){ 
            
            painter.setBrush(bulbs[i].color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(bulbs[i].position, 4, 4);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(Qt::white, 1.5)); // Pen blanc, épaisseur 2 pixels
            painter.drawEllipse(bulbs[i].position, 4, 4);

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
