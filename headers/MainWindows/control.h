#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include "ui_control.h"


class Control : public QMainWindow {
    public:
        explicit Control(QWidget *parent = nullptr);
        ~Control();

    private:
        Ui::CONTROL *ui;


    
    public slots:
        void updateComboBox(const QString &data);
    private slots :
        void slotMethod();
        void onSignalComboChanged();


};

#endif //CONTROL_H