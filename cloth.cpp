#include "cloth.h"

Vector3 computeNorml(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
  Vector3 ret;
//  Vector3 temp1 = particles.at(index(u, v)).position - particles.at(index(u + 1, v)).position;
   Vector3 temp1 = p1 - p2;
  Vector3 temp2 = p2 - p3;
  ret = temp1.crossProduct(temp2).normalize();
  return ret;
}

cloth::cloth():height(10),width(10),DAMPING(0.03),DRAG(1 - DAMPING),MASS(0.1),restDistance(25),xSegs(10),ySegs(10),GRAVITY(981 * 1.4),gravity(Vector3(0.0, -GRAVITY, 0.0) * MASS),TIMESTEP((float)18 / 1000),TIMESTEP_SQ(TIMESTEP * TIMESTEP)
{
  int temp_arr[10] = {0,1,2,3,4, 5,6, 7,8,9};
  memcpy(pins,temp_arr,sizeof(temp_arr));
}

cloth::cloth(float distance, float x, float y, float h, float w):height(h),width(w), DAMPING(0.03),DRAG(1 - DAMPING),MASS(0.1),restDistance(distance),xSegs(x),ySegs(y),GRAVITY(981 * 1.4),gravity(Vector3(0.0, -GRAVITY, 0.0) * MASS),TIMESTEP((float)18 / 1000),TIMESTEP_SQ(TIMESTEP * TIMESTEP)
{
  int temp_arr[10] = {0,1,2,3,4, 5,6, 7,8,9};
  memcpy(pins,temp_arr,sizeof(temp_arr));
}

int cloth::index(int u, int v)
{
//  return u + v * ( w + 1 );
  return u + v * ( xSegs );
}

Vector3 cloth::getballp()
{
  return balls.getposition();
}

float cloth::getballs()
{
  return balls.getsize();
}

void cloth::simulate(time_t time)
{
  winds.timec();
  balls.go();
  if(winds.has())
    {
       Vector3 temp1;
      for(int u = 0; u < xSegs - 1; ++u)
        for(int v = 0; v < ySegs -1; ++v)
          {
              temp1 = computeNorml(particles.at(index(u, v)).position, particles.at(index(u + 1, v)).position, particles.at(index(u , v + 1)).position);
              temp1*temp1.dotProduct(winds.getf());

              particles.at(index(u, v)).addAcceleration(temp1);
              particles.at(index(u + 1, v)).addAcceleration(temp1);
              particles.at(index(u, v + 1)).addAcceleration(temp1);
          }
}
  /*
  for (int i = 0, il = faces.length; i < il; i ++ ) {

                          face = faces[ i ];
                          normal = face.normal;

                          tmpForce.copy( normal ).normalize().multiplyScalar( normal.dot( windForce ) );//点积。求当前各个面的加速度
                          particles[ face.a ].addForce( tmpForce );//猜测一下，应该是对于面的各个点的加速度？
                          particles[ face.b ].addForce( tmpForce );
                          particles[ face.c ].addForce( tmpForce );

                  }
                  */
  //计算加上重力的加速度。然后计算每个点的位置
          for ( int i = 0; i < particles.size(); i ++ ) {

                  particles.at(i).addAcceleration(gravity);
//                  particle.addForce( gravity );
                   particles.at(i).integrate(TIMESTEP_SQ, DRAG);//计算位置

          }

//          for(int i = 0; i < 3; ++i)
//            {
          for (int  i = 0; i < constrains.size(); i ++ ) {
              constrains.at(i).satisifyConstrains();//衣服的约束条件，什么原理呢，先计算每个粒子的位置，再对粒子位置进行调整，满足约束。这个约束自己定义

                  }
//          }

          for (int i = 0; i < 10; i ++ ) {

                  particles.at(pins[i]).position.set( particles.at(pins[i]).original);
                   particles.at(pins[i]).previous.set( particles.at(pins[i]).original);

          }

//add ball or not
          if(balls.has())
            {

             /*球体绘制
              glColor3f(1.0, 1.0, 1.0);
              glPushMatrix();
              glTranslatef(ballPosition.x, ballPosition.y,ballPosition.z);
              glutSolidSphere(ballSize,20,20);
              glPopMatrix();
              */
              for (int i = 0; i < particles.size(); i ++ ) {
                  Vector3 diff = particles.at(i).position - balls.getposition();
                                      if ( diff.length() < balls.getsize() + 4) {

                                              // collided/碰撞
                                              diff.normalize();
                                              diff*(balls.getsize() + 4);
                                              particles.at(i).position = balls.getposition() + diff;

                                      }

                              }
            }
}

void cloth::crt_cloth(float w, float h)
{
  for(int v = 0; v < h; v ++ ) {

      for (int  u = 0; u < w; u ++ ) {

              particles.push_back(Particle( u / w, v / h, 0, MASS,restDistance, xSegs,ySegs ));

        }
   }
   for (int v = 0; v < h -1; v ++ ) {
          for ( int u = 0; u < w -1; u ++ ) {
                          constrains.push_back(constrain( &(particles.at(index( u, v ))), &(particles.at(index( u, v + 1))), restDistance));
//                              constrains.push( [particles[ index( u, v ) ],particles[ index( u, v + 1 ) ],restDistance] );

                           constrains.push_back(constrain( &(particles.at(index( u, v ))), &(particles.at(index( u + 1, v ))), restDistance));

              }

     }

   for ( int u = w -1, v = 0; v < h -1; v ++ ) {
constrains.push_back(constrain( &(particles.at(index( u, v ))), &(particles.at(index( u, v + 1))), restDistance));

           }

           for (int  v = h -1, u = 0; u < w -1; u ++ ) {
constrains.push_back(constrain( &(particles.at(index( u, v ))), &(particles.at(index( u + 1, v ))), restDistance));

           }
           /*share springs
           float diagonalDist = sqrt(restDistance * restDistance * 2);
            for (int v=0;v<h - 1;v++) {
                        for (int u=0;u<w - 1;u++) {

                                constrains.push_back(constrain(&particles.at(index(u, v)), &particles.at(index(u + 1, v + 1)), diagonalDist));

                                constrains.push_back(constrain(&particles.at(index(u + 1, v)), &particles.at(index(u , v + 1)), diagonalDist));

                        }
                    }*/
}

