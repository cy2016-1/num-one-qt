#include "interactiveacts.h"
int  startHook();
bool stopHook();
bool GloablKeyListen = false;


//MPU6050：Roll移动上下示教，Yaw移动左右示教，pitch左右移动-转为鼠标控制；鼠标：左键射击、中键前进，右键后退，
//1键左移，2键右移，3键蹲下，4键跳起
enum ClickStyle {LS};

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);

HMODULE WINAPI ModuleFromAddress(PVOID pv);
static HHOOK hHook;


#define XBUTTON3      0x0003
#define XBUTTON4      0x0004


InteractiveActs::InteractiveActs(QWidget * parent):QWidget(parent)
{
    aWebSocketClient = new WeSocketClient();
    startHook();
    CreateGui();
    LinkSignalSlot();
};

void InteractiveActs::CreateGui()
{
    resize(800,600);
    this->setStyleSheet("Background:rgb(240,240,240)");
    setAttribute(Qt::WA_StyledBackground);//设置背景颜色，缺少此命令将使设置样式失效

    //创建并设置总布局
    TotalLayout = new QHBoxLayout();

    OpInstructionGBox = new QGroupBox();
    OpInstructionGBox->setTitle("使用说明");
    TotalLayout->addWidget(OpInstructionGBox, 0, 0);
    IstructionVBLyt = new QVBoxLayout(OpInstructionGBox);
    //操作说明
    OpInstruction = new QLabel("操作说明：\n1.本程序用于使用NumOne的触摸引脚以及MPU6050模拟鼠标;\n2.触摸引脚实现鼠标左键单击，MPU6050的姿态实现CS1.6视角的变化;\n3.按下P键启动，按下L键关闭");
    OpInstruction->setParent(this);
    IstructionVBLyt->addWidget(OpInstruction);

    OperateGBox = new QGroupBox();
    OperateGBox->setTitle("操作框");
    TotalLayout->addWidget(OperateGBox);
    DataOpsVBLyt = new QVBoxLayout(OperateGBox);

    DatasShowVBLyt = new QVBoxLayout();
    DataOpsVBLyt->addLayout(DatasShowVBLyt);
    ReciStrLabel=new QLabel("接收数据: ");
    DatasShowVBLyt->addWidget(ReciStrLabel);


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

    Paras_PFV_HBlyt = new QHBoxLayout();
    DataOpsVBLyt->addLayout(Paras_PFV_HBlyt);
    Paras_PFV_Label = new QLabel("视角移动比例：");
    DataOpsVBLyt->addWidget(Paras_PFV_Label);
    Paras_PFV_LE = new QLineEdit("2000");
    DataOpsVBLyt->addWidget(Paras_PFV_LE);


    DataOpsVBLyt->addStretch(0);

    TotalLayout->setStretch(0,1);//setStretch放在addWidget之后起作用
    TotalLayout->setStretch(1,1);


    this->setLayout(TotalLayout);
}
void InteractiveActs::LinkSignalSlot()
{
    connect(aWebSocketClient, &WeSocketClient::SendReceiveMsg, this, &InteractiveActs::RevMsg);
    connect(aWebSocketClient, &WeSocketClient::SendConnectedState, this, &InteractiveActs::updateConnState);
    connect(OpenWebSocketBtn, &QPushButton::clicked, this, &InteractiveActs::openws);
    connect(CloseWebSocketBtn, &QPushButton::clicked, this, &InteractiveActs::closews);
}

void InteractiveActs::openws()
{
    aWebSocketClient->open();
    aWebSocketClient->flush();
}

void InteractiveActs::closews()
{
    aWebSocketClient->close();
    stopHook();
}

void InteractiveActs::RevMsg(const QString &message)
{
        QStringList strs_list = message.split(",");
        ReciStrLabel->setText("接收数据: "+message);
        if(GloablKeyListen)
        {
            //根据MPU6050的数据控制光标或键盘动作
            float Yaw = strs_list[0].toFloat();
            float Roll = strs_list[1].toFloat();
            float attack = strs_list[2].toFloat();
            //枪口准芯视角移动
            qDebug()<<Paras_PFV_LE->text().toInt()<<endl;
            MoveDeltaPos(Paras_PFV_LE->text().toInt()*(tan(Roll/180*3.141592653)-tan(last_roll/180*3.141592653)),Paras_PFV_LE->text().toInt()*(tan(Yaw/180*3.141592653)-tan(last_yaw/180*3.141592653)));
            last_roll = Roll;
            last_yaw = Yaw;

             if(attack<50)
             {
                 clickCurPos(LS);
             }
        }
}

void InteractiveActs::updateConnState(bool state)
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


void InteractiveActs::MoveToCurPos(int x,int y) {
     SetCursorPos(x,y);
}
void InteractiveActs::MoveDeltaPos(int dt_x,int dt_y)
{
    GetCursorPos(&pos);
    SetCursorPos(pos.x+dt_x,pos.y+dt_y);
}


void InteractiveActs::clickCurPos(int style) {
    GetCursorPos(&pos);
    switch (style) {
    case LS:
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, pos.x, pos.y, 0, 0);
        mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, pos.x, pos.y, 0, 0);
        break;
    }
}

LRESULT KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *Key_Info = (KBDLLHOOKSTRUCT*)lParam;
    if (HC_ACTION == nCode)
    {
        if (WM_KEYDOWN == wParam || WM_SYSKEYDOWN == wParam)  //如果按键为按下状态
        {
            if (Key_Info->vkCode <= 254 && Key_Info->vkCode >= 65)
            {
                qDebug() << Key_Info->vkCode;
                if(Key_Info->vkCode==80)//按下P开启
                {
                    GloablKeyListen = true;
                }
                else if(Key_Info->vkCode==76)//按下L键关闭
                {
                    GloablKeyListen = false;
                }
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

HMODULE ModuleFromAddress(PVOID pv)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) {
        return (HMODULE)mbi.AllocationBase;
    } else {
        return NULL;
    }
}

int startHook()
{
    hHook = SetWindowsHookExW(WH_KEYBOARD_LL, KeyboardHookProc, ModuleFromAddress((PVOID)KeyboardHookProc), 0);
    int error = GetLastError();
    return error;
}

bool stopHook()
{
    return UnhookWindowsHookEx(hHook);
}

