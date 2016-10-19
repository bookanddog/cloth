
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector.h"                 /**< 包含向量类头文件 */

/** 摄像机类 */
class Camera
{
public:
/* 构造函数和析构函数 */
Camera();
~Camera();

/* 设置摄像机的位置, 观察点和向上向量 */
void setCamera( float eyeX, float eyeY, float eyeZ,
                float lookX, float lookY, float lookZ,
                float upX, float upY, float upZ);
void roll(float angle);
void pitch(float angle);
void yaw(float angle);
void slide(float du, float dv, float dn);
float getDist();
void setShape(float viewAngle,float aspect,float Near,float Far);


private:
/* 摄像机属性 */
Vector3        eye,look,up;
float              speed;
Vector3        u,v,n;
float          viewAngle, aspect, nearDist, farDist;
void           setModelViewMatrix();

};

#endif //__CAMERA_H__
