#ifndef BALL_H
#define BALL_H

#include<vector.h>

//#define M_PI 3.141592653   /*pi*/
class ball
{
public:
  ball();
  void go();
  float getsize();
  Vector3& getposition();
  bool has();
private:
  bool have;
  Vector3 position;
  float size;
  float alpha;
};

#endif // BALL_H
