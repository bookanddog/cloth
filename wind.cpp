#include "wind.h"

wind::wind():have(true),windStrength(2.0), windForce(Vector3(0,0,0))
{

}

void wind::timec()
{
  struct timeb t1;
  ftime(&t1);
  long t = t1.time * 1000 + t1.millitm;
  windStrength = cos( t / 7000 ) * 40 + 50;
//  windStrength = cos( t / 7 ) * 20 + 40;//风力大小是一个函数
  windForce.set(sin(t / 2000 ), cos( t / 3000 ), sin( t / 1000));
  windForce.normalize();
  windForce*windStrength;
}

Vector3 wind::getf()
{
  return windForce;
}

bool wind::has()
{
  return have;
}

