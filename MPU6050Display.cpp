#include "MPU6050Display.h"
#include <QPushButton>

MPU6050Display::MPU6050Display(QWidget *parent):QWidget(parent)
{
    CreateGui();
    LinkSignalSlot();
}

void MPU6050Display::CreateGui()
{
    resize(800,600);
    QPushButton *btn1 = new QPushButton("子窗口1",this);
    QPushButton *btn2 = new QPushButton("子窗口2",this);
    this->setStyleSheet("Background:rgb(10,200,100)");
    aVBoxLyt = new QVBoxLayout(this);
    aVBoxLyt->addWidget(btn1);
     aVBoxLyt->addWidget(btn2);
     this->setStyleSheet("Background:rgb(200,100,100)");
     setAttribute(Qt::WA_StyledBackground);
}

void MPU6050Display::LinkSignalSlot()
{

}

MPU6050Display::~MPU6050Display()
{

}
