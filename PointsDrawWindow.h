#pragma once
#pragma execution_character_set("utf-8") //允许中文
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <QMouseEvent>
#include <QWheelEvent>

#include "Shader.h"
#include "Camera.h"
#include "Scene.h"

#include<iostream>
using namespace std;
#include <QVector>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextCodec>


class PointsDrawWindow :public QOpenGLWidget, protected QOpenGLFunctions_3_3_Compatibility
{
    //QPointsDrawWindow相当于GLFW，QOpenGLFunctions_3_3_Compatibility相当于Glad
    Q_OBJECT;
public:
    PointsDrawWindow(QWidget* parent = nullptr);//构造函数，空构造
    ~PointsDrawWindow();//析构函数
    void LoadPointsFromFile();
    void ClearDatas();
    void PushBackPointColor(float x,float y,float z,float r,float g,float b);
    void SaveDatas();


private:
    Shader* m_shader=nullptr;//渲染器
    Camera myCamera;//相机类
    Scene * pntcloudscene=nullptr;
    mat4 viewMat, projectionMat, modelMat;//变换矩阵
    int MousePressX, MousePressY, MouseMoveX, MouseMoveY, MouseReleaseX, MouseReleaseY, DeltaMouseX, DeltaMouseY, WheelDeltaValue;//视图变换变量
    float deltatime;//单次运行时长
    QVector<QVector<float>> DatasVectors_n_6d;
    int ShowedPointNum = 0;
    bool RecieveDataFlag = true;

    void initializeGL();//初始化Opengl
    void resizeGL(int w, int h);//当窗口大小改变时被调用的回调函数
    void paintGL();//循环绘制
    //鼠标事件
    virtual void mousePressEvent(QMouseEvent* theEvent);//鼠标事件，用来操作窗口的旋转和平移
    virtual void mouseReleaseEvent(QMouseEvent* theEvent);
    virtual void mouseMoveEvent(QMouseEvent * theEvent);
    virtual void wheelEvent(QWheelEvent * theEvent);

    virtual void onLButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onRButtonDown(const int theFlags, const QPoint thePoint);
    virtual void onMouseWheel(const int theFlags, const int theDelta, const QPoint thePoint);
};

