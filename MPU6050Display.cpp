#include "MPU6050Display.h"
MPU6050Display::MPU6050Display(QWidget *parent):QWidget(parent)
{
    CreateGui();
    LinkSignalSlot();
}

void MPU6050Display::CreateGui()
{
    resize(800,600);
    this->setStyleSheet("Background:rgb(240,240,240)");
    setAttribute(Qt::WA_StyledBackground);//设置背景颜色，缺少此命令将使设置样式失效

    //创建并设置总布局
    TotalLayout = new QGridLayout();
    TotalLayout->setColumnStretch(0, 1);
    TotalLayout->setColumnStretch(1, 1);

    Datas3DGBox = new QGroupBox();
    Datas3DGBox->setTitle("3D姿态");
    TotalLayout->addWidget(Datas3DGBox, 0, 0);
    Data3DVBLyt = new QVBoxLayout(Datas3DGBox);

    this->setLayout(TotalLayout);
    //布局下设置功能控件

    //绘制正方体
    aCube = new MyCube(nullptr);
    Data3DVBLyt->addWidget(aCube);

}

void MPU6050Display::LinkSignalSlot()
{

}

MPU6050Display::~MPU6050Display()
{

}
