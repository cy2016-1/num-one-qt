#include "PointsDrawWindow.h"

PointsDrawWindow::PointsDrawWindow(QWidget* parent):QOpenGLWidget(parent)
{
    //构造函数

}


PointsDrawWindow::~PointsDrawWindow()
{
    //析构函数
}

void PointsDrawWindow::initializeGL()
{
    initializeOpenGLFunctions();//初始化Opengl
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);//背景颜色
    glEnable(GL_DEPTH_TEST);//允许深度缓存
    //1 配置着色器和着色器程序
    m_shader = new Shader("shader/shader.vs", "shader/shader.fs");
    pntcloudscene = new Scene();
    pntcloudscene->PointsSet();
}
void PointsDrawWindow::resizeGL(int w, int h)
{
    //当窗口大小改变时被调用的回调函数
    glViewport(0, 0, w, h);
}


void PointsDrawWindow::paintGL()
{
    //3 循环绘制
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader->use();


     projectionMat = perspective(radians(myCamera.CameraZoom), 1200.0f / 800.0f, 0.1f, 1000.0f);
     m_shader->SetMat4("projectionMat", projectionMat);

     viewMat = myCamera.GetViewMatrix();
     viewMat = rotate(viewMat, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
     viewMat = rotate(viewMat, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
     m_shader->SetMat4("viewMat", viewMat);

     modelMat = mat4(1.0f);
     m_shader->SetMat4("modelMat", modelMat);//modelMat= [Vx;Vy;Vz;[0,0,0,1]'],Vx为行向量

    glBindVertexArray(pntcloudscene->VAO_Pnt_Cloud);
    glBindBuffer(GL_ARRAY_BUFFER, pntcloudscene->VBO_Pnt_Cloud);
    if(ShowedPointNum<DatasVectors_n_6d.size())
    {
        for(int PointNum =ShowedPointNum;PointNum<DatasVectors_n_6d.size();PointNum++)
        {
           glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*PointNum, 6*sizeof(float), DatasVectors_n_6d.at(PointNum).data());
        }
        ShowedPointNum = DatasVectors_n_6d.size();
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPointSize(5);
    glDrawArrays(GL_POINTS, 0, ShowedPointNum);//绘制点

    glBindVertexArray(pntcloudscene->VAO_Coord);
    //绘制坐标系
    glLineWidth(5);
    glBindBuffer(GL_ARRAY_BUFFER, pntcloudscene->VBO_Coord);
    glDrawArrays(GL_LINES, 0, 2);//绘制基坐标系
    glDrawArrays(GL_LINES, 2, 2);
    glDrawArrays(GL_LINES, 4, 2);


    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    update();
}

 void PointsDrawWindow::LoadPointsFromFile()
 {
     //读取并解析刀路数据文件
     QString file, fileName, fileSuffix, filePath;
     file = QFileDialog::getOpenFileName(nullptr, "打开文件", "", "txt Files(*.txt)");
     QFileInfo fileInfo = QFileInfo(file);
     filePath = fileInfo.filePath();//文件地址
     fileName = fileInfo.fileName();//文件名
     fileSuffix = fileInfo.suffix();//文件后缀
     QFile LoadTxtFile(filePath);
     QString TxtContent;
     if (!LoadTxtFile.exists()) //如果文件不存在
     {
         QMessageBox::about(this, "提示", "未找到文件！");
         TxtContent = "";
     }
     else
     {
         DatasVectors_n_6d.clear();

         if (LoadTxtFile.open(QFile::ReadWrite | QFile::Text)) //打开文件
         {
             QString PathTxtContent = LoadTxtFile.readAll(); //读取信息
         }

         QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
         std::string dirPath_name = code->fromUnicode(filePath).data();

         ifstream ReadTxtDatas(dirPath_name);
         ReadTxtDatas.precision(2);
         float data_point_color;
         QVector<float> PointColorData_6d;
         int inum = 1;
         while (ReadTxtDatas >> data_point_color)
         {
            PointColorData_6d.push_back(data_point_color);
            if(inum>5)
            {
              DatasVectors_n_6d.push_back(PointColorData_6d);
              inum = 0;
              PointColorData_6d.clear();
            }
            inum+=1;
         }
         ReadTxtDatas.close();
        }
 }

void PointsDrawWindow::ClearDatas()
{
    ShowedPointNum =0;
    DatasVectors_n_6d.clear();

}

void PointsDrawWindow::PushBackPointColor(float x,float y,float z,float r,float g,float b)
{
    if(DatasVectors_n_6d.size()<10000&&RecieveDataFlag)
    {
        QVector<float> PointColorData_6d;
        PointColorData_6d = {x,y,z,r,g,b};
        DatasVectors_n_6d.push_back(PointColorData_6d);
    }
    else
    {
        if(RecieveDataFlag)
        {
            QMessageBox::about(this, "提示", "最多允许10000个点的显示，如果想显示更多的点，请修改源码。");
            RecieveDataFlag = false;
        }
    }
}

void PointsDrawWindow::SaveDatas()
{
    QString fileName_Suffix, fileSuffix, filePath, filename;
    QString dirPath = QFileDialog::getSaveFileName(nullptr, QStringLiteral("保存为*.txt文件"), "savedatas.txt", "txt Files(*.txt)",
        Q_NULLPTR, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
    std::string dirPath_name = code->fromUnicode(dirPath).data();
    if (!dirPath.isEmpty())
    {
        QFileInfo fileInfo(dirPath);
        filePath = fileInfo.filePath();//文件地址
        fileName_Suffix = fileInfo.fileName();//文件名
        fileSuffix = fileInfo.suffix();//文件后缀
        filename = fileInfo.baseName();
        if (fileInfo.exists())
        {
            QFile::remove(dirPath);
        }
        fstream output_stream;
        output_stream.precision(2);
        output_stream.setf(std::ios::fixed);
        output_stream.open(dirPath_name, ios::out | ios::app);
        QVector<float> PointColorData_6d;
        for (int PointNum = 0; PointNum < DatasVectors_n_6d.size(); PointNum++)
        {
            PointColorData_6d = DatasVectors_n_6d.at(PointNum);
            output_stream << PointColorData_6d.at(0) << " " << PointColorData_6d.at(1) << " " << PointColorData_6d.at(2) << " ";
            output_stream << PointColorData_6d.at(3) << " " << PointColorData_6d.at(4) << " " << PointColorData_6d.at(5) << endl;
        }
        output_stream.close();
    }
}

void PointsDrawWindow::mousePressEvent(QMouseEvent* theEvent)
{
    MousePressX = theEvent->x();
    MousePressY = theEvent->y();
}

void PointsDrawWindow::mouseMoveEvent(QMouseEvent * theEvent)
{
    if (theEvent->buttons() == 1)
    {
        MouseMoveX = theEvent->x();
        MouseMoveY = theEvent->y();
        DeltaMouseX = MouseMoveX - MousePressX;
        DeltaMouseY = MousePressY - MouseMoveY;
        myCamera.ProcessMouseMovement(-DeltaMouseX, -DeltaMouseY, true);
    }
    else if (theEvent->buttons() == 4)
    {
        MouseMoveX = theEvent->x();
        DeltaMouseX = MousePressX - MouseMoveX;
        MouseMoveY = theEvent->y();
        DeltaMouseY = MousePressY - MouseMoveY;
        if (DeltaMouseX > 0)
            myCamera.ProcessKeyboard(RIGHT, DeltaMouseX*0.001f);
        else
            myCamera.ProcessKeyboard(LEFT, -DeltaMouseX * 0.001f);
        if (DeltaMouseY > 0)
            myCamera.ProcessKeyboard(UP, DeltaMouseY*0.001f);
        else
            myCamera.ProcessKeyboard(DOWN, -DeltaMouseY * 0.001f);

    }
}

void PointsDrawWindow::mouseReleaseEvent(QMouseEvent* theEvent)
{
    if (theEvent->buttons() == 1)
    {
        MouseReleaseX = theEvent->x();
        MouseReleaseY = theEvent->y();
    }
}

void PointsDrawWindow::wheelEvent(QWheelEvent * theEvent)
{
    WheelDeltaValue = theEvent->delta() / 120;
    myCamera.ProcessMouseScroll(WheelDeltaValue);
}

void PointsDrawWindow::onLButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    cout << "EEEEE" << endl;
}

void PointsDrawWindow::onMButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    cout << "FFFFFF" << endl;
}

void PointsDrawWindow::onRButtonDown(const int /*theFlags*/, const QPoint thePoint)
{
    // Save the current mouse coordinate in min.
    cout << "GGGGGGG" << endl;
}

void PointsDrawWindow::onMouseWheel(const int /*theFlags*/, const int theDelta, const QPoint thePoint)
{
    cout << "HHHHHHHH" << endl;
}
