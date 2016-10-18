#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include <particle.h>
#include <constrain.h>
#include <string.h>
#include <ball.h>
#include <wind.h>

class cloth
{
public:
  cloth();
  cloth(float distance, float x, float y, float h, float w);

  void crt_cloth(float w, float h);
  int index(int u, int v);
  void simulate(time_t time);//计算当前时刻各个质点的合力，具体根据力计算质点位置并进行约束

  Vector3 computeNorml(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
  {
    Vector3 ret;
  //  Vector3 temp1 = particles.at(index(u, v)).position - particles.at(index(u + 1, v)).position;
     Vector3 temp1 = p1 - p2;
    Vector3 temp2 = p2 - p3;
    ret = temp1.crossProduct(temp2).normalize();
    return ret;
  }

private:
  int pins[10] ;//悬挂点
  float height, width;
  float DAMPING ; //阻尼
  float DRAG;//拖拉，阻力//这个是弹性系数k？/不知道干什么用
  float MASS ;  //质量，集中。所以是质心？m
  float restDistance ;//初始距离
  float xSegs ;
  float ySegs ;
  float GRAVITY;  //引力大小
  Vector3 gravity;
  float TIMESTEP ;//时间间隔
  float TIMESTEP_SQ ;//时间间隔平方

  ball balls;
  wind winds;
  std::vector<Particle> particles;
  std::vector<constrain> constrains;
};

#endif // CLOTH_H
