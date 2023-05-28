#include "QtNumOne.h"

QtNumOne::QtNumOne(QWidget *parent)
    : QMainWindow(parent)
{
    aMainGui = new MainGUi(this);
    LinkSignalSlot();
}

void QtNumOne::LinkSignalSlot()
{
    connect(aMainGui->SingleData_Aisplay_Act, &QAction::triggered, this, [=]()
    {
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new SingleDataDisplay(this);
        this->setCentralWidget(CurrentWgt);
    });
    connect(aMainGui->MUP6050_Display_Act, &QAction::triggered, this, [=]()
    {
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new MPU6050Display(nullptr);
        this->setCentralWidget(CurrentWgt);
    });
}

QtNumOne::~QtNumOne()
{
    delete aMainGui;
}

