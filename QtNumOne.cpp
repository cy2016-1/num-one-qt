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
    {//MPU6050数据显示
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new SingleDataDisplay(this);
        this->setCentralWidget(CurrentWgt);
    });
    connect(aMainGui->MUP6050_Display_Act, &QAction::triggered, this, [=]()
    {//MPU6050数据采集
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new MPU6050Display(nullptr);
        this->setCentralWidget(CurrentWgt);
    });
    connect(aMainGui->MUP6050_Interaction_Act, &QAction::triggered, this, [=]()
    {//MPU6050控制电脑鼠标
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new InteractiveActs(nullptr);
        this->setCentralWidget(CurrentWgt);
    });
    connect(aMainGui->PointCloud_Display_Act, &QAction::triggered, this, [=]()
    {//激光测距点云扫描显示
        if(CurrentWgt!=nullptr)
            delete CurrentWgt;
        CurrentWgt = new PointCloudWidget(nullptr);
        this->setCentralWidget(CurrentWgt);
    });
}

QtNumOne::~QtNumOne()
{
    delete aMainGui;
}

