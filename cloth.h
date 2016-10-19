#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include "particle.h"
#include "constrain.h"
#include <string.h>
#include "ball.h"
#include "wind.h"

Vector3 computeNorml(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);

class cloth
{
public:
  cloth();
  cloth(float distance, float x, float y, float h, float w);

  void crt_cloth(float w, float h);
  int index(int u, int v);
  int getx() {return xSegs;}
  int gety() {return ySegs;}
  Vector3 getballp();
  float getballs();

  void simulate(time_t time);//计算当前时刻各个质点的合力，具体根据力计算质点位置并进行约束
 std::vector<Particle>& getp(){return particles;}
 std::vector<constrain>& getc(){return constrains;}

private:
  int pins[10] ;//悬挂点
  float height, width;//貌似没有用，gl窗口的尺寸。不应在这
  float DAMPING ; //阻尼
  float DRAG;//拖拉，阻力//这个是弹性系数k？/不知道干什么用
  float MASS ;  //质量，集中。所以是质心？m
  float restDistance ;//初始距离
  float xSegs ;//横着的质点数
  float ySegs ;//竖着的质点数
  float GRAVITY;  //引力大小
  Vector3 gravity;
  float TIMESTEP ;//时间间隔
  float TIMESTEP_SQ ;//时间间隔平方

  ball balls;
  wind winds;

public://目前用函数将其值传递出去有问题，暂时使public，待改进。
  std::vector<Particle> particles;
  std::vector<constrain> constrains;
};

#endif // CLOTH_H
