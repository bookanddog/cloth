#include "ball.h"
#include<math.h>

ball::ball():have(true), position(Vector3(0.0, -45, 0.0)),size(60),alpha(0)
{

}

void ball::go()
{
  if(alpha <= 2 * M_PI)
  alpha = (alpha + 0.03) ;
  else alpha = 0.0;
  position.z = -sin( alpha  ) * 70 ;
  position.x = cos( alpha  )* 70;
}

float ball::getsize()
{
  return size;
}

Vector3 &ball::getposition()
{
  return position;
}

bool ball::has()
{
  return have;
}

