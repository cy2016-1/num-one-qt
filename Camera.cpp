#include "Camera.h"
Camera::Camera(vec3 Pos, vec3 Up, float Pitch, float Yaw)
{
    //相机属性
    CameraPos = Pos;
    CameraUp = Up;
    CameraFront = vec3(0.0f, 0.0f, -1.0f);
    CameraRight = normalize(cross(CameraFront, Up));
    WorldUp = Up;
    CameraPitch = Pitch;
    CameraYaw = YAW;
    CameraSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    CameraZoom = ZOOM;
    UpdateCameraVector();
}
//通过分量参数进行构造
Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float Pitch, float Yaw)
{
    //相机属性
    CameraPos = vec3(PosX, PosY, PosZ);
    CameraUp = vec3(UpX, UpY, UpZ);
    CameraFront = vec3(UpX, UpY, UpZ);
    CameraRight = vec3(0.0f, 0.0f, -1.0f);
    WorldUp = CameraUp;
    CameraPitch = Pitch;
    CameraYaw = YAW;
    CameraSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    CameraZoom = ZOOM;
    UpdateCameraVector();
}

//获取ViewMatrix
mat4 Camera::GetViewMatrix()
{
    return lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void Camera::UpdateCameraVector()
{
    //根据Pitc和Yaw更新相机的朝向
    vec3 front;
    front.x = cos(radians(CameraPitch))*sin(radians(CameraYaw));
    front.y = sin(radians(CameraPitch));
    front.z = -cos(radians(CameraYaw))*cos(radians(CameraPitch));
    CameraFront = normalize(front);
    // also re-calculate the Right and Up vector
    CameraRight = normalize(cross(CameraFront, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    CameraUp = normalize(cross(CameraRight, CameraFront));

}


//键盘输入
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = CameraSpeed * deltaTime;
    float delta_angle = atan(velocity / (sqrt(CameraPos.x*CameraPos.x + CameraPos.y*CameraPos.y + CameraPos.z*CameraPos.z)))*180/3.141592653f;
    if (direction == FORWARD)
        CameraPos += CameraFront * velocity;
    if (direction == BACKWARD)
        CameraPos -= CameraFront * velocity;
    if (direction == LEFT)
    {
        CameraPos -= CameraRight * velocity;
        CameraYaw += delta_angle;
    }
    if (direction == RIGHT)
    {
        CameraPos += CameraRight * velocity;
        CameraYaw -= delta_angle;
    }
    if (direction == UP)
    {
        CameraPos -= CameraUp * velocity;
        CameraPitch += delta_angle;
    }
    if (direction == DOWN)
    {
        CameraPos += CameraUp * velocity;
        CameraPitch -= delta_angle;
    }
    //更新相机
    UpdateCameraVector();
}

//接收并处理鼠标输入
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool ConstrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    CameraPitch += yoffset;
    CameraYaw += xoffset;
    // 限制俯仰角
    if (ConstrainPitch)
    {
        if (CameraPitch > 89.0f)
            CameraPitch = 89.0f;
        if (CameraPitch < -89.0f)
            CameraPitch = -89.0f;
    }

    //更新相机
    UpdateCameraVector();
}
//接收滚轮信号
void Camera::ProcessMouseScroll(float yoffset)
{
    CameraZoom -= (float)yoffset;
    if (CameraZoom < 1.0f)
        CameraZoom = 1.0f;
    /*if (CameraZoom > 45.0f)
        CameraZoom = 45.0f;*/
    //更新相机
    UpdateCameraVector();
}
