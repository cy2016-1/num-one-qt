#include "QtNumOne.h"

QtNumOne::QtNumOne(QWidget *parent)
    : QMainWindow(parent)
{
    aMainGui = new MainGUi(this);

    LinkSignalSlot();
}

void QtNumOne::LinkSignalSlot()
{
    connect(aMainGui->MUP6050_Display_Act, &QAction::triggered, this, [=](){
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
                CurrentWgt = new MPU6050Display(this);
            });
}

QtNumOne::~QtNumOne()
{
    delete aMainGui;
}

