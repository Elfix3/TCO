#ifndef ZONEVIEW_H
#define ZONEVIEW_H

//file include
#include "zone.h"
#include "signalView.h"

//Qt includes
#include <QWidget>
#include <QPainter>

class ZoneView : public QWidget{
    Q_OBJECT
    public :
        explicit ZoneView(Zone *zone, QWidget *parent=nullptr);
        ~ZoneView();

        protected:
            void paintEvent(QPaintEvent * event) override;

        private slots:
            void onZoneUpdate();

        private :
            Zone *zone;

            const int zone_view_size = 30;


};

#endif //ZONEVIEW_H