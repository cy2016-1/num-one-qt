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
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QList>

#include "WeSocketClient.h"

#include "MyCube.h"

using namespace std;
using namespace QtCharts;

class MPU6050Display: public QWidget
{
    Q_OBJECT
public:
    WeSocketClient * aWebSocketClient;

    QHBoxLayout * TotalLayout, * open_close_Btn_HBlyt, * SendDataHBlyt;
    QGroupBox * Datas3DGBox, * OperateGBox;
    QVBoxLayout * Data3DVBLyt, * DataOpsVBLyt, * DatasShowVBLyt;

    QLabel * YawValueLabel, *PitchValueLabel, *RollValueLabel, *GyroXValueLabel, *GyroYValueLabel, *GyroZValueLabel;

    QLabel * AccXValueLabel, *AccYValueLabel, *AccZalueLabel;
    QLabel * connectedStateLabel;
    QPushButton  * OpenWebSocketBtn, * CloseWebSocketBtn, *SendMsgBtn, *SaveDatasBtn;
    QLineEdit *SendMessageLedit;
    QFile SaveDatasFile;
    QTextStream SaveDatasStream;
    bool DatasSaveFlag = false;
    QList<float> DatasList;

    MyCube * aCube;

    MPU6050Display(QWidget *parent=nullptr);
    ~MPU6050Display();
    void CreateGui();
    void LinkSignalSlot();
private slots:
   void update3D(const QString &message);
   void updateConnState(bool state);
   void openws();
   void closews();
   void sendmsg();
   void savedatas();
};

#endif // MPU6050DISPLAY_H
