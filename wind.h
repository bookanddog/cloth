#ifndef WIND_H
#define WIND_H

#include <vector.h>
#include <sys/timeb.h>
#include <math.h>

class wind
{
public:
  wind();
  void timec();
  Vector3 getf();
  bool has();

private:
  bool have ;
  float windStrength;//风力数值
  Vector3 windForce;
};

#endif // WIND_H
