#ifndef MAINGUI_H
#define MAINGUI_H
#pragma execution_character_set("utf-8") //允许中文

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

class MainGUi
{
public:

    QMenuBar *aMenuBar;//菜单栏
    //基础模块菜单
    QMenu *BasicModuleMenu;

    //基础模块菜单下的Action
    QAction *MUP6050_Display_Act;

    MainGUi(QMainWindow *MainWindow)
    {
        //设置图标
        MainWindow->setWindowIcon(QIcon(":/img/Resources/image/Cabot.ico"));
        MainWindow->resize(1600,900);
        MainWindow->setStyleSheet("Background:rgb(240,240,240)");
        aMenuBar = MainWindow->menuBar();//获取窗口的菜单栏
        aMenuBar->setStyleSheet("Background:rgb(255,255,255)");

        //基础模块菜单栏

        BasicModuleMenu = new QMenu("基础模块");
        aMenuBar->addMenu(BasicModuleMenu);

        //基础模块菜单栏下的Actions
        MUP6050_Display_Act = new QAction("01_MPU6050数据显示");
        BasicModuleMenu->addAction(MUP6050_Display_Act);
    }
};

#endif // MAINGUI_H
