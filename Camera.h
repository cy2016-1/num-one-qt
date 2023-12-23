#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
using namespace glm;
//定义几个特定的移动方式
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//缺省的相机参数
const float PITCH = 0.0f;
const float YAW = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.0025f;
const float ZOOM = 45.0f;

//一个相机抽象类，用来处理输入和计算欧拉角等，做opengl的输入
class Camera
{
public:
    //相机属性
    vec3 CameraPos, CameraFront, CameraUp, CameraRight, WorldUp;
    float CameraPitch, CameraYaw;//欧拉角
    float CameraSpeed, MouseSensitivity, CameraZoom;//相机选项
    //采用向量来构造
    Camera(vec3 Pos = vec3(0.0f, 1.0f, 20.0f), vec3 Up = vec3(0.0f, 1.0f, 0.0f), float Pitch = PITCH, float Yaw = YAW);
    //通过分量参数进行构造
    Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Pitch = PITCH, float Yaw = YAW);
    //获取ViewMatrix
    mat4 GetViewMatrix();
    //键盘输入
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    //接收并处理鼠标输入
    void ProcessMouseMovement(float xoffset, float yoffset, bool ConstrainPitch);
    //接收滚轮信号
    void ProcessMouseScroll(float yoffset);
private:
    void UpdateCameraVector();
};

