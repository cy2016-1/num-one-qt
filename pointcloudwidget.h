#ifndef POINTCLOUDWIDGET_H
#define POINTCLOUDWIDGET_H
#pragma execution_character_set("utf-8") //允许中文

#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "PointsDrawWindow.h"
#include "WeSocketClient.h"

class PointCloudWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PointCloudWidget(QWidget *parent = nullptr);
    ~PointCloudWidget();

private:
    void CreateGui();
    void LinkSignalSlot();
    QHBoxLayout * TotalLayout, * WebsocketBtnsHLyt;
    QGroupBox * PntsCloudGBox, * OperateGBox;
    QVBoxLayout * PntsCloudVLyt, * operateVLyt;
    QLabel * WsconnectedStateLabel;
    QPushButton * LoadDatasBtn,*ClearDatasBtn, * WebsocketOpenBtn, * WebsocketCloseBtn, * SaveDatasBtn;
    PointsDrawWindow * PointCloudShow = nullptr;
    WeSocketClient * aWebSocketClient;

private slots:
   void RecieveWsDatas(const QString &message);
   void updateConnState(bool state);
   void openws();
   void closews();
   void savedatas();

};

#endif // POINTCLOUDWIDGET_H
