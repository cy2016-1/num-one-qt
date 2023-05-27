#ifndef MPU6050DISPLAY_H
#define MPU6050DISPLAY_H
#pragma execution_character_set("utf-8") //允许中文
#include <QMainWindow>
#include <QVBoxLayout>

class MPU6050Display: public QWidget
{
    Q_OBJECT
public:
    QVBoxLayout * aVBoxLyt;
    MPU6050Display(QWidget *parent=nullptr);
    ~MPU6050Display();
    void CreateGui();
    void LinkSignalSlot();
};

#endif // MPU6050DISPLAY_H
