#include "zoneView.h"

ZoneView::ZoneView(Zone *zone,QWidget *parent)
    :QWidget(parent),zone(zone){
    setFixedSize(zone->GetModificationAreaWidth(), zone->GetModificationAreaHeight());
    connect(zone,&Zone::powerChanged,this,&ZoneView::onZoneUpdate);
}

ZoneView::~ZoneView()
{
}

static int* lastone = 0x0;

void ZoneView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);
    QObject* paintDevice = (QObject*)painter.device();
    //qDebug() << "=> " << (int*)paintDevice << "\r\n=> " << ((int*)this + 0x4) << "\r\n=> " << lastone << "\r\n=> " << (int*)parentWidget() <<"\r\n\t=> " << ((int*)paintDevice == ((int*)this + 0x4));
    //qDebug() << zone->getName() << "0000";
    if ((int*)this + 0x4 == (int*)paintDevice
        && (int*)parentWidget() != lastone)
    {
        //qDebug() << "AAAA";
        if (zone->isZoneEnabled())
        {
            // Contre la boucle infinie du redraw/paintevent qui se recall
            lastone = (int*)parentWidget();

            //Rectangle de modification dans l'espace global
            QRect qzone(zone->getHint()[TI_RECT1_X],
                        zone->getHint()[TI_RECT1_Y],
                        zone->getHint()[TI_RECT1_WIDTH],
                        zone->getHint()[TI_RECT1_HEIGHT]);
            QRect qzone2(zone->getHint()[TI_RECT2_X],
                        zone->getHint()[TI_RECT2_Y],
                        zone->getHint()[TI_RECT2_WIDTH],
                        zone->getHint()[TI_RECT2_HEIGHT]);
            //On récupère ce qui est dessiné à ce niveau
            auto ref = (parentWidget())->grab(qzone);
            auto ref2 = (parentWidget())->grab(qzone2);

            //Création de la base du "masque"
            QPixmap mask = QPixmap(zone->getHint()[TI_RECT1_WIDTH], zone->getHint()[TI_RECT1_HEIGHT]);
            QPixmap mask2 = QPixmap(zone->getHint()[TI_RECT2_WIDTH], zone->getHint()[TI_RECT2_HEIGHT]);
            //Colorié entièrement de la couleur de l'activation
            mask.fill(QColor::fromRgb(255,0,0));
            mask2.fill(QColor::fromRgb(255,0,0));//(QColor(Z_ENABLED));

            //Contour a dessiner (premier type, zone "non noire"
            //auto tmask = ref.createMaskFromColor(QColor::fromRgb(0,0,0), Qt::MaskOutColor);
            //Contour a dessiner (second type, zone "gris 122"
            //auto tmask = ref.createMaskFromColor(QColor::fromRgb(122, 122, 122), Qt::MaskInColor);
            //auto tmask2 = ref2.createMaskFromColor(QColor::fromRgb(122, 122, 122), Qt::MaskInColor);

            //On applique la base du masque (la couleur de fond coloré)
            painter.drawImage(zone->GetRelativeOrigineRect1(), mask.toImage());
            painter.drawImage(zone->GetRelativeOrigineRect2(), mask2.toImage());

            //On demande que les prochains draw soit en [RGB]*[RGB] (noir x n'importe = noir | blanc * n'importe = n'importe)
            painter.setCompositionMode(QPainter::CompositionMode_Multiply);

            //On plaque le contour
            painter.drawImage(zone->GetRelativeOrigineRect1(), ref.toImage());
            painter.drawImage(zone->GetRelativeOrigineRect2(), ref2.toImage());

            //reset du lastone pour prochaine itération
            lastone = nullptr;
        }
    }
    return;

   // painter.setRenderHint(QPainter::Antialiasing);

/*

    int centerX = (zone->getHint()[TI_POINT_X] - zone->getHint()[TI_RECT_X]);//width() / 2;
    int centerY = (zone->getHint()[TI_POINT_Y] - zone->getHint()[TI_RECT_Y]);//height() / 2;
    int radius = 6;

    if(zone->isZoneEnabled()){
        painter.setBrush(QColor(Z_ENABLED));
        painter.setPen(Qt::NoPen);
        
    } else {
        painter.setBrush(QColor(OFF));
        painter.setPen(Qt::NoPen);

    }
    painter.drawEllipse(QPoint(centerX, centerY), radius, radius);


    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawEllipse(QPoint(centerX, centerY), radius, radius);


    /*painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(zone->getHint(), zone->getHint());*/
    //qDebug() << "X : " << zone->getHint()[TI_POINT_X] << " Y: "<< zone->getHint()[TI_POINT_Y];
}


void ZoneView::onZoneUpdate(){
    update();
}
