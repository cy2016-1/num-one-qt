#ifndef MYCUBE_H
#define MYCUBE_H
#pragma execution_character_set("utf-8") //允许中文
#include <QWidget>
#include <QMouseEvent>

#include <QVector3D>
#include <QMatrix4x4>

class MyCube : public QWidget
{
  Q_OBJECT
public:
  explicit MyCube(QWidget *parent = nullptr);
  void changeRotateByRPY(float roll,float pitch, float yaw);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QPointF getPoint(const QVector3D &vt,int w) const;
  QVector<QVector3D> vertexArr;   //八个顶点
  QVector<QVector<int>> elementArr; //六个面
  QMatrix4x4 rotateMat;   //旋转矩阵
};

#endif // MYCUBE_H
