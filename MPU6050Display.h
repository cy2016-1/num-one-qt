#ifndef MPU6050DISPLAY_H
#define MPU6050DISPLAY_H
#pragma execution_character_set("utf-8") //允许中文
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineSeries>
#include <QChartView>
#include <QValueAxis>

#include <deque>

#include "MyCube.h"

using namespace std;
using namespace QtCharts;

class MPU6050Display: public QWidget
{
    Q_OBJECT
public:
    QGridLayout * TotalLayout;
    QGroupBox * Datas3DGBox;
    QVBoxLayout * Data3DVBLyt;

    MyCube * aCube;

    MPU6050Display(QWidget *parent=nullptr);
    ~MPU6050Display();
    void CreateGui();
    void LinkSignalSlot();
};

#endif // MPU6050DISPLAY_H
