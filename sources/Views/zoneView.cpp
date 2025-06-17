#include "zoneView.h"


ZoneView::ZoneView(Zone *zone,QWidget *parent)
    :QWidget(parent),zone(zone){
        setFixedSize(zone->GetModificationAreaWidth(), zone->GetModificationAreaHeight());
        connect(zone,&Zone::powerChanged,this,&ZoneView::onZoneUpdate);
}

ZoneView::~ZoneView()
{
}

//static int* lastone = 0x0;


#include <QWidget>
#include <QWindow>

void ZoneView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    static thread_local bool inPaint = false;
    if (inPaint) {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::transparent);
        return;
    }
    
    QScopedValueRollback<bool> guard(inPaint, true);

    QPainter painter(this);
    
    // Version simplifiée et plus robuste pour vérifier le device
    if (painter.device() == this)
    {
        QRect qzone(zone->getHint()[TI_RECT1_X],
                  zone->getHint()[TI_RECT1_Y],
                  zone->getHint()[TI_RECT1_WIDTH],
                  zone->getHint()[TI_RECT1_HEIGHT]);
        QRect qzone2(zone->getHint()[TI_RECT2_X],
                   zone->getHint()[TI_RECT2_Y],
                   zone->getHint()[TI_RECT2_WIDTH],
                   zone->getHint()[TI_RECT2_HEIGHT]);

        bool updatesEnabled = this->updatesEnabled();
        if (updatesEnabled) setUpdatesEnabled(false);

        QPixmap ref = parentWidget()->grab(qzone);
        QPixmap ref2 = parentWidget()->grab(qzone2);

        if (updatesEnabled) setUpdatesEnabled(true);

        QPixmap mask(qzone.size());
        QPixmap mask2(qzone2.size());

        mask.fill(zone->isZoneEnabled() ? QColor::fromRgb(255, 0, 0) : QColor(122, 122, 122));
        mask2.fill(zone->isZoneEnabled() ? QColor::fromRgb(255, 0, 0) : QColor(122, 122, 122));

        painter.drawImage(zone->GetRelativeOrigineRect1(), mask.toImage());
        painter.drawImage(zone->GetRelativeOrigineRect2(), mask2.toImage());

        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.drawImage(zone->GetRelativeOrigineRect1(), ref.toImage());
        painter.drawImage(zone->GetRelativeOrigineRect2(), ref2.toImage());
    }
}



void ZoneView::onZoneUpdate(){
    update();
}