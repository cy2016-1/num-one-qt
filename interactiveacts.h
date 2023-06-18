#ifndef INTERACTIVEACTS_H
#define INTERACTIVEACTS_H
#pragma execution_character_set("utf-8") //允许中文

//  override byte to prevent clashes with <cstddef>,避免error: C2872: “byte”: 不明确的符号 d:\windows kits\10\include\10.0.17763.0\shared\rpcndr.h(191): note: 可能是“unsigned char byte”
#define byte win_byte_override

#include <windows.h>
#include <QWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include "WeSocketClient.h"


#pragma comment  (lib, "User32.lib")

class InteractiveActs:public QWidget
{
    Q_OBJECT
public:
    InteractiveActs(QWidget * parent =nullptr);
    POINT pos = {0, 0};
    float last_roll = 0;
    float last_yaw = 0;
    void MoveToCurPos(int x,int y);//全局移动
    void MoveDeltaPos(int dt_x,int dt_y);//相对移动
    void clickCurPos(int style);

private:
    QLabel * OpInstruction = nullptr;
    QHBoxLayout * TotalLayout, * open_close_Btn_HBlyt, * Paras_PFV_HBlyt;
    QGroupBox * OpInstructionGBox, * OperateGBox;
    QVBoxLayout * IstructionVBLyt, * DatasShowVBLyt, * DataOpsVBLyt;

    QLabel * ReciStrLabel,* connectedStateLabel, * Paras_PFV_Label;
    QLineEdit * Paras_PFV_LE;

    QPushButton * OpenWebSocketBtn, * CloseWebSocketBtn;

    WeSocketClient * aWebSocketClient;

    void CreateGui();
    void LinkSignalSlot();

private slots:
    void openws();
    void closews();
    void RevMsg(const QString &message);
    void updateConnState(bool state);
};

#endif
