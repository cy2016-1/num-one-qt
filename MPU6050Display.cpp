#include "MPU6050Display.h"
#include <QDebug>

MPU6050Display::MPU6050Display(QMainWindow *MainWind):QWidget(nullptr)
{
    CenteralWgt = new QWidget();
    MainWind->setCentralWidget(CenteralWgt);
    CreateGui();
    LinkSignalSlot();
}

void MPU6050Display::CreateGui()
{
    CenteralWgt->setStyleSheet("Background:rgb(40,240,240)");
}

void MPU6050Display::LinkSignalSlot()
{

}

MPU6050Display::~MPU6050Display()
{
    delete CenteralWgt;
}
