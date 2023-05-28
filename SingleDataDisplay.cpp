#include "SingleDataDisplay.h"

SingleDataDisplay::SingleDataDisplay(QWidget *parent):QWidget(parent)
{
    aWebSocketClient = new WeSocketClient(nullptr);
    CreateGui();
    LinkSignalSlot();
}

void SingleDataDisplay::CreateGui()
{
    resize(800,600);
    this->setStyleSheet("Background:rgb(240,240,240)");
    setAttribute(Qt::WA_StyledBackground);//设置背景颜色，缺少此命令将使设置样式失效

    //创建并设置总布局
    TotalLayout = new QHBoxLayout();


    DataCurveGBox = new QGroupBox();
    DataCurveGBox->setTitle("数据曲线");
    TotalLayout->addWidget(DataCurveGBox);
    DataCurveVBLyt = new QVBoxLayout(DataCurveGBox);

    OperateGBox = new QGroupBox();
    OperateGBox->setTitle("操作框");
    TotalLayout->addWidget(OperateGBox);
    DataOpsVBLyt = new QVBoxLayout(OperateGBox);

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
    //初始化原始数据容器

    DataDeques.resize(SeriesDataNum);
    DatasLineSeries = new QLineSeries();
    for (int iNum = 0; iNum < SeriesDataNum; iNum++)
    {
        DatasLineSeries->append(iNum, 0);
    }//初始化曲线

    aChartView = new QChartView();
    aChartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    DataCurveVBLyt->addWidget(aChartView);
    aChartView->chart()->addSeries(DatasLineSeries);
    //显示设置
    aChartView->chart()->setTheme(QChart::ChartThemeHighContrast);
    aChartView->chart()->setTitle("数据");
    //显示轴
    datas_x_axis = new QValueAxis();
    datas_x_axis->setRange(0, 60);
    datas_x_axis->setTitleText("滑动采样区间");

    aChartView->chart()->setAxisX(datas_x_axis, DatasLineSeries);
    QValueAxis *datas_y_axis = new QValueAxis();
    datas_y_axis->setRange(-1, 1);//后期自动设置y轴方位
    datas_y_axis->setTitleText("数值");
    aChartView->chart()->setAxisY(datas_y_axis, DatasLineSeries);
    //显示legend
    aChartView->chart()->legend()->setVisible(false);
    //设置自动缩放
    aChartView->setRubberBand(QChartView::RectangleRubberBand);
}



void SingleDataDisplay::LinkSignalSlot()
{
    connect(aWebSocketClient, &WeSocketClient::SendReceiveMsg, this, &SingleDataDisplay::updateCurveDatas);
    connect(aWebSocketClient, &WeSocketClient::SendConnectedState, this, &SingleDataDisplay::updateConnState);
    connect(OpenWebSocketBtn, &QPushButton::clicked, this, &SingleDataDisplay::openws);
    connect(CloseWebSocketBtn, &QPushButton::clicked, this, &SingleDataDisplay::closews);
    connect(SendMsgBtn, &QPushButton::clicked, this, &SingleDataDisplay::sendmsg);
    connect(SaveDatasBtn, &QPushButton::clicked, this, &SingleDataDisplay::savedatas);
}

void SingleDataDisplay::updateCurveDatas(const QString &message)
{
    //更新曲线数据
    float getData = message.toFloat();
    DataDeques.push_back(getData);
    DataDeques.pop_front();
    DatasLineSeries->clear();
    //自动轴范围
    if(y_max_value<getData)
        y_max_value = getData;
    if(getData<y_min_value)
        y_min_value = getData;
    aChartView->chart()->axisY()->setRange(y_min_value, y_max_value);

    for (int iNum = 0; iNum < SeriesDataNum; iNum++)
    {
        DatasLineSeries->append(iNum, DataDeques.at(iNum));
    }
    aChartView->chart()->removeSeries(DatasLineSeries);
    aChartView->chart()->addSeries(DatasLineSeries);
    //保存数据
    if(DatasSaveFlag)
    {
        SaveDatasStream<<getData<<endl;
    }
}

void SingleDataDisplay::updateConnState(bool state)
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

void SingleDataDisplay::openws()
{
    aWebSocketClient->open();
    aWebSocketClient->flush();
}

void SingleDataDisplay::closews()
{
    aWebSocketClient->close();
}

void SingleDataDisplay::sendmsg()
{
    aWebSocketClient->sendMessage(SendMessageLedit->text());
}

void SingleDataDisplay::savedatas()
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

SingleDataDisplay::~SingleDataDisplay()
{

}
