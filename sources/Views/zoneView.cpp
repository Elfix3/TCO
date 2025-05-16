#include "zoneView.h"


ZoneView::ZoneView(Zone *zone,QWidget *parent)
    :QWidget(parent),zone(zone){
        setFixedSize(zone_view_size,zone_view_size);
        qDebug() << "cstrct called";

        connect(zone,&Zone::powerChanged,this,&ZoneView::onZoneUpdate);
}

ZoneView::~ZoneView()
{
}

void ZoneView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int centerX = width() / 2;
    int centerY = height() / 2;
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
}


void ZoneView::onZoneUpdate(){
    update();
}