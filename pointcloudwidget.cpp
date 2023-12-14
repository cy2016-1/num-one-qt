#include "PointCloudWidget.h"

PointCloudWidget::PointCloudWidget(QWidget *parent): QWidget{parent}
{
    aWebSocketClient = new WeSocketClient(nullptr);
    CreateGui();
    LinkSignalSlot();
}

void PointCloudWidget::CreateGui()
{
    resize(800,600);
    this->setStyleSheet("Background:rgb(240,240,240)");
    setAttribute(Qt::WA_StyledBackground);//设置背景颜色，缺少此命令将使设置样式失效

    //创建并设置总布局
    TotalLayout = new QHBoxLayout();

    PntsCloudGBox = new QGroupBox();
    PntsCloudGBox->setTitle("点云显示");
    TotalLayout->addWidget(PntsCloudGBox, 0, 0);
    PntsCloudVLyt = new QVBoxLayout(PntsCloudGBox);
    PointCloudShow = new PointsDrawWindow(nullptr);
    PntsCloudVLyt->addWidget(PointCloudShow, 0, 0);

    OperateGBox = new QGroupBox();
    OperateGBox->setTitle("操作框");
    TotalLayout->addWidget(OperateGBox);
    operateVLyt = new QVBoxLayout(OperateGBox);
    LoadDatasBtn = new QPushButton("加载并显示数据");
    ClearDatasBtn = new QPushButton("清空数据");
    operateVLyt->addStretch(0);
    operateVLyt->addWidget(LoadDatasBtn);
    operateVLyt->addWidget(ClearDatasBtn);

    operateVLyt->addStretch(0);
    WsconnectedStateLabel = new QLabel("断开连接");
    WsconnectedStateLabel->setStyleSheet("Background:rgb(240,100,100);");
    WsconnectedStateLabel->setAlignment(Qt::AlignHCenter);
    operateVLyt->addWidget(WsconnectedStateLabel);

    WebsocketBtnsHLyt = new QHBoxLayout();
    operateVLyt->addLayout(WebsocketBtnsHLyt);
    WebsocketOpenBtn = new QPushButton("打开websocket");
    WebsocketBtnsHLyt->addWidget(WebsocketOpenBtn);
    WebsocketCloseBtn = new QPushButton("关闭websocket");
    WebsocketBtnsHLyt->addWidget(WebsocketCloseBtn);
    SaveDatasBtn = new QPushButton("保存数据");
    operateVLyt->addWidget(SaveDatasBtn);
    operateVLyt->addStretch(0);

    TotalLayout->setStretch(0,3);//setStretch放在addWidget之后起作用
    TotalLayout->setStretch(1,1);
    this->setLayout(TotalLayout);

}

void PointCloudWidget::LinkSignalSlot()
{
    connect(LoadDatasBtn, &QPushButton::clicked, this, [=](){PointCloudShow->LoadPointsFromFile();});
    connect(ClearDatasBtn, &QPushButton::clicked, this, [=](){PointCloudShow->ClearDatas();});
    connect(aWebSocketClient, &WeSocketClient::SendReceiveMsg, this, &PointCloudWidget::RecieveWsDatas);
    connect(aWebSocketClient, &WeSocketClient::SendConnectedState, this, &PointCloudWidget::updateConnState);
    connect(WebsocketOpenBtn, &QPushButton::clicked, this, &PointCloudWidget::openws);
    connect(WebsocketCloseBtn, &QPushButton::clicked, this, &PointCloudWidget::closews);
    connect(SaveDatasBtn, &QPushButton::clicked, this, &PointCloudWidget::savedatas);
}

void PointCloudWidget::RecieveWsDatas(const QString &message)
{
    QStringList strs_list = message.split(" ");
    if(strs_list.size() == 6)
    {
       float point_x = strs_list[0].toFloat(),point_y = strs_list[1].toFloat(),point_z = strs_list[2].toFloat();
       float color_r = strs_list[3].toFloat(),color_g = strs_list[4].toFloat(),color_b = strs_list[5].toFloat();
       PointCloudShow->PushBackPointColor(point_x,point_y,point_z,color_r,color_g,color_b);
    }
}

void PointCloudWidget::updateConnState(bool state)
{
    if(state)
    {
        WsconnectedStateLabel->setText("已经连接");
        WsconnectedStateLabel->setStyleSheet("Background:rgb(0,240,100);");
    }
    else
    {
        WsconnectedStateLabel->setText("断开连接");
        WsconnectedStateLabel->setStyleSheet("Background:rgb(240,100,100);");
    }
}

void PointCloudWidget::openws()
{
    PointCloudShow->ClearDatas();
    aWebSocketClient->open();
    aWebSocketClient->flush();
}

void PointCloudWidget::closews()
{
    aWebSocketClient->close();
}


void PointCloudWidget::savedatas()
{
    PointCloudShow->SaveDatas();
}

PointCloudWidget::~PointCloudWidget()
{

}
