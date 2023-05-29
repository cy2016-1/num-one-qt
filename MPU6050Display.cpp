#include "MPU6050Display.h"
MPU6050Display::MPU6050Display(QWidget *parent):QWidget(parent)
{
    aWebSocketClient = new WeSocketClient(nullptr);
    CreateGui();
    LinkSignalSlot();
}

void MPU6050Display::CreateGui()
{
    resize(800,600);
    this->setStyleSheet("Background:rgb(240,240,240)");
    setAttribute(Qt::WA_StyledBackground);//设置背景颜色，缺少此命令将使设置样式失效

    //创建并设置总布局
    TotalLayout = new QHBoxLayout();

    Datas3DGBox = new QGroupBox();
    Datas3DGBox->setTitle("3D姿态");
    TotalLayout->addWidget(Datas3DGBox, 0, 0);
    Data3DVBLyt = new QVBoxLayout(Datas3DGBox);

    //绘制正方体
    aCube = new MyCube(nullptr);
    Data3DVBLyt->addWidget(aCube);

    OperateGBox = new QGroupBox();
    OperateGBox->setTitle("操作框");
    TotalLayout->addWidget(OperateGBox);
    DataOpsVBLyt = new QVBoxLayout(OperateGBox);

    DatasShowVBLyt = new QVBoxLayout();
    DataOpsVBLyt->addLayout(DatasShowVBLyt);
    YawValueLabel=new QLabel("Yaw:  0");
    DatasShowVBLyt->addWidget(YawValueLabel);

    PitchValueLabel=new QLabel("Pitch:  0");
    DatasShowVBLyt->addWidget(PitchValueLabel);

    RollValueLabel=new QLabel("Roll:  0");
    DatasShowVBLyt->addWidget(RollValueLabel);

    GyroXValueLabel=new QLabel("GyroX:  0");
    DatasShowVBLyt->addWidget(GyroXValueLabel);

    GyroYValueLabel=new QLabel("GyroY:  0");
    DatasShowVBLyt->addWidget(GyroYValueLabel);

    GyroZValueLabel=new QLabel("GyroZ:  0");
    DatasShowVBLyt->addWidget(GyroZValueLabel);

    AccXValueLabel=new QLabel("AccX:  0");
    DatasShowVBLyt->addWidget(AccXValueLabel);

    AccYValueLabel=new QLabel("AccY:  0");
    DatasShowVBLyt->addWidget(AccYValueLabel);

    AccZalueLabel=new QLabel("AccZ:  0");
    DatasShowVBLyt->addWidget(AccZalueLabel);


    DataOpsVBLyt->addStretch(0);
    connectedStateLabel = new QLabel("断开连接");
    connectedStateLabel->setStyleSheet("Background:rgb(240,100,100);");
    connectedStateLabel->setAlignment(Qt::AlignHCenter);
    DataOpsVBLyt->addWidget(connectedStateLabel);

    open_close_Btn_HBlyt = new QHBoxLayout();
    DataOpsVBLyt->addLayout(open_close_Btn_HBlyt);
    OpenWebSocketBtn = new QPushButton("打开websocket");
    open_close_Btn_HBlyt->addWidget(OpenWebSocketBtn);
    CloseWebSocketBtn = new QPushButton("关闭websocket");
    open_close_Btn_HBlyt->addWidget(CloseWebSocketBtn);

    SendDataHBlyt = new QHBoxLayout();
    DataOpsVBLyt->addLayout(SendDataHBlyt);
    SendMessageLedit= new QLineEdit("Ok!");
    SendDataHBlyt->addWidget(SendMessageLedit);
    SendMsgBtn = new QPushButton("发送");
    SendDataHBlyt->addWidget(SendMsgBtn);

    SaveDatasBtn = new QPushButton("保存数据");
    SaveDatasBtn->setStyleSheet("Background:rgb(200,200,200);");
    DataOpsVBLyt->addWidget(SaveDatasBtn);

    DataOpsVBLyt->addStretch(0);

    TotalLayout->setStretch(0,3);//setStretch放在addWidget之后起作用
    TotalLayout->setStretch(1,1);


    this->setLayout(TotalLayout);

}

void MPU6050Display::LinkSignalSlot()
{
    connect(aWebSocketClient, &WeSocketClient::SendReceiveMsg, this, &MPU6050Display::update3D);
    connect(aWebSocketClient, &WeSocketClient::SendConnectedState, this, &MPU6050Display::updateConnState);
    connect(OpenWebSocketBtn, &QPushButton::clicked, this, &MPU6050Display::openws);
    connect(CloseWebSocketBtn, &QPushButton::clicked, this, &MPU6050Display::closews);
    connect(SendMsgBtn, &QPushButton::clicked, this, &MPU6050Display::sendmsg);
    connect(SaveDatasBtn, &QPushButton::clicked, this, &MPU6050Display::savedatas);
}

void MPU6050Display::update3D(const QString &message)
{
    QStringList strs_list = message.split(",");
    DatasList.clear();
    if(strs_list.size() == 9)
    {
       for(int i =0;i<9;i++)
       {
           DatasList.push_back(strs_list[i].toFloat());
       }
       aCube->changeRotateByRPY(DatasList[2],DatasList[1],DatasList[0]);
       //显示数据
       YawValueLabel->setText("Yaw:"+strs_list[0]);
       PitchValueLabel->setText("Pitch:  "+strs_list[1]);
       RollValueLabel->setText("Roll:  "+strs_list[2]);
       GyroXValueLabel->setText("GyroX:  "+strs_list[3]);
       GyroYValueLabel->setText("GyroY:  "+strs_list[4]);
       GyroZValueLabel->setText("GyroZ:  "+strs_list[5]);
       AccXValueLabel->setText("AccX:  "+strs_list[6]);
       AccYValueLabel->setText("AccY:  "+strs_list[7]);
       AccZalueLabel->setText(" AccZ:  "+strs_list[8]);
       //保存数据
       if(DatasSaveFlag)
       {
           SaveDatasStream<<message<<endl;
       }
    }
}

void MPU6050Display::updateConnState(bool state)
{
    if(state)
    {
        connectedStateLabel->setText("已经连接");
        connectedStateLabel->setStyleSheet("Background:rgb(0,240,100);");
    }
    else
    {
        connectedStateLabel->setText("断开连接");
        connectedStateLabel->setStyleSheet("Background:rgb(240,100,100);");
    }
}

void MPU6050Display::openws()
{
    aWebSocketClient->open();
    aWebSocketClient->flush();
}

void MPU6050Display::closews()
{
    aWebSocketClient->close();
}

void MPU6050Display::sendmsg()
{
    aWebSocketClient->sendMessage(SendMessageLedit->text());
}

void MPU6050Display::savedatas()
{
    if(!DatasSaveFlag)
    {//开启数据
        SaveDatasBtn->setStyleSheet("Background:rgb(100,240,100);");
        SaveDatasBtn->setText("停止保存");
        DatasSaveFlag = true;
        //打开数据保存文件
        SaveDatasFile.setFileName("SavedDatas.txt");
        SaveDatasFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        SaveDatasStream.setDevice(&SaveDatasFile);
    }
    else
    {//关闭保存
        SaveDatasBtn->setStyleSheet("Background:rgb(200,200,200);");
        SaveDatasBtn->setText("保存数据");
        DatasSaveFlag = false;
        SaveDatasFile.close();
    }
}

MPU6050Display::~MPU6050Display()
{

}
