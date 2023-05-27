#ifndef MPU6050DISPLAY_H
#define MPU6050DISPLAY_H
#include <QMainWindow>

class MPU6050Display: public QWidget
{
    Q_OBJECT
public:
    MPU6050Display(QMainWindow *MainWind);
    ~MPU6050Display();
    void CreateGui();
    void LinkSignalSlot();
private:
    QWidget * CenteralWgt = nullptr;
};

#endif // MPU6050DISPLAY_H
