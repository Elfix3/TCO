#ifndef GUI_H
#define GUI_H

//such mess, please clean

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QDebug>
#include <QPalette>
#include <QMessageBox>
#include <QTransform>

#include "ui_gui.h"
#include "signalView.h"
#include "aiguilleView.h"


#define DEFAULT_WIDTH       1920
#define DEFAULT_HEIGHT      1080 



class Gui : public QMainWindow {
    public:
        explicit Gui(QWidget *parent = nullptr);
        ~Gui();
        Ui::GUI* getUI();
    

        void addSignalView(LightSignal *SignalView);
        
    protected :
        //void changeEvent(QEvent *event);
        void resizeEvent(QResizeEvent *event) override;
        
    private:
        Ui::GUI *ui;
        QMap <int,SignalView*> signalViews;

    signals:
        void sendSerialData(const QString &data);
        
    private slots :
        void slotMethod();
        void buttonPressed();



};

#endif //GUI_H