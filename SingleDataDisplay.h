#ifndef SINGLEDATADISPLAY_H
#define SINGLEDATADISPLAY_H

#pragma execution_character_set("utf-8") //允许中文
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineSeries>
#include <QChartView>
#include <QValueAxis>
#include <QWebSocket>
#include <QLabel>
#include <QPushButton>
#include "WeSocketClient.h"
#include <QLineEdit>
#include <QFile>

#include <deque>

using namespace std;
using namespace QtCharts;

class SingleDataDisplay: public QWidget
{
    Q_OBJECT
public:
    WeSocketClient * aWebSocketClient;

    QHBoxLayout * TotalLayout,  * open_close_Btn_HBlyt, * SendDataHBlyt;
    QGroupBox * DataCurveGBox, * OperateGBox;
    QVBoxLayout * DataCurveVBLyt, * DataOpsVBLyt;
    QLabel * connectedStateLabel;
    QPushButton  * OpenWebSocketBtn, * CloseWebSocketBtn, *SendMsgBtn, *SaveDatasBtn;
    QLineEdit *SendMessageLedit;
    QFile SaveDatasFile;
    QTextStream SaveDatasStream;
    bool DatasSaveFlag = false;

    deque<float>  DataDeques;
    QLineSeries * DatasLineSeries;
    QChartView * aChartView;
    int SeriesDataNum = 60;
    QValueAxis *datas_x_axis, * datas_y_axis;
    float y_max_value = 0.0f, y_min_value = 0.0f;

    SingleDataDisplay(QWidget *parent=nullptr);
    ~SingleDataDisplay();
    void CreateGui();
    void LinkSignalSlot();
 private slots:
    void updateCurveDatas(const QString &message);
    void updateConnState(bool state);
    void openws();
    void closews();
    void sendmsg();
    void savedatas();
};


#endif // SINGLEDATADISPLAY_H
