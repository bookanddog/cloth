#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>//to get millisec making the ball move smothly,non stand library.use kernal lib sys(in linux).

#include <QtGui/QImage>
#include <QtOpenGL/QGLWidget>

#include <GL/freeglut.h>
#include <GL/glu.h>

#include "particle.h"
#include "constrain.h"
#include "camera.h"
#include "key.h"

using namespace std;

vector<Particle> particles;
vector<constrain> constrains;

//mouse
int posx,posy;

//camera
Camera camera;
//key
Keys keys;

//sphere
Vector3 ballPosition(0.0, -45, 0.0);
bool ball = true;
float ballSize = 60;
float alpha = 0;//ball move

//wind
bool wind = true;
float windStrength = 2;//风力数值
Vector3 windForce;

int pins[10] = {0,1,2,3,4, 5,6, 7,8,9};
//int pins[10] = {0,9};
float height, width;
float DAMPING = 0.03; //阻尼
float DRAG = 1 - DAMPING;//拖拉，阻力//这个是弹性系数k？/不知道干什么用
float MASS = 0.1;  //质量，集中。所以是质心？m
float restDistance = 25;//初始距离
float xSegs = 10;
float ySegs = 10;
float GRAVITY = 981 * 1.4;  //引力大小
Vector3 gravity = Vector3(0.0, -GRAVITY, 0.0) * MASS;
float TIMESTEP = (float)18 / 1000;//时间间隔
float TIMESTEP_SQ = TIMESTEP * TIMESTEP;//时间间隔平方

GLuint tex_id;


//void satisifyConstrains(Particle &p1, Particle &p2, float distance);
int index(int u, int v);//返回质点的位置
Vector3 computeNorml(const Vector3 &p1, const Vector3 &p2,const Vector3 &p3);//计算法向量

void timefunc(int i);//刷新画面
void setLight(); //设置光源
void reshape(GLsizei w,GLsizei h);//

/****主要的直接调用的函数****/
void cloth(float w, float h);//创建布料的模型，包括质点与约束
void glInit();//初始化，以及cloth函数调用，创建布料
void draw();//绘制的回调函数，调用simulate函数，计算质点位置，将数据绘制出来。

void simulate(time_t time);//计算当前时刻各个质点的合力，具体根据力计算质点位置并进行约束

//键盘与鼠标函数
void keyfunc(unsigned char key, int x, int y);
void mousefunc(int button, int state, int x, int y);
void motionfunc(int x, int y);
void keyupfunc(unsigned char key,int x,int y);

int main(int argc,char ** argv)
{
  /*初始化*/
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
  glutInitWindowSize(800,600);
  glutInitWindowPosition(400,400);

  /*创建窗口*/
  glutCreateWindow("cloth");

  /*绘制与显示*/
  glInit();
  glutReshapeFunc(reshape);
    glutDisplayFunc(draw);
    glutTimerFunc(18, timefunc, 1);
//    glutKeyboardFunc();
//    glutMouseFunc(mousefunc);
//    glutMotionFunc(motionfunc);
//    glutKeyboardFunc(keyfunc);
//    glutKeyboardUpFunc(keyupfunc);

  glutMainLoop();

  return 0;
}

void keyfunc(unsigned char key, int x, int y)
{
  keys.SetPressed(key);
}
void keyupfunc(unsigned char key,int x,int y)
{
  keys.SetReleased(key);
}
void mousefunc(int button, int state, int x, int y)
{
  if(state == GLUT_UP);
}
//void motionfunc(int x, int y);

void draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  time_t t = time(NULL);//time sec from 1970.1.1 0:0:0
  struct timeb t1;
  ftime(&t1);
  long t = t1.time * 1000 + t1.millitm;
  windStrength = cos( t / 7000 ) * 40 + 50;
//  windStrength = cos( t / 7 ) * 20 + 40;//风力大小是一个函数
  windForce.set(sin(t / 2000 ), cos( t / 3000 ), sin( t / 1000));
  windForce.normalize();
  windForce*windStrength;

  if(alpha <= 2 * M_PI)
  alpha = (alpha + 0.03) ;
  else alpha = 0.0;
  ballPosition.z = -sin( alpha  ) * 70 ;
  ballPosition.x = cos( alpha  )* 70;

  simulate(t);

  //texture envirement
//  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, tex_id);

  for(int i = 0; i < 10 - 1; ++i)
    {
      glBegin(GL_TRIANGLE_STRIP);
      for(int j = 0; j < 10; j ++)
        {
          glTexCoord2f(i * 0.1, j * 0.1); //it should be i * 1 / 11 i think for there are 11 coord,
           glVertex3f(particles.at(i * 10 + j).position.x, particles.at(i * 10 + j).position.y,particles.at(i * 10 + j).position.z);

           glTexCoord2f((i+ 1) * 0.1, j * 0.1); //it should be i * 1 / 11 i think for there are 11 coord,
            glVertex3f(particles.at( (i + 1) * 10 + j).position.x, particles.at((i + 1) * 10 + j).position.y,particles.at((i + 1) * 10 + j).position.z);
        }
      glEnd();
    }
/*
// glColor3f(0.0,1.0,1.0);
  for(int i = 0; i < 10; ++i)
    {
      glBegin(GL_LINE_STRIP);

//     glVertex3f(particles.at(i * 10).position.x, particles.at(i * 10).position.y,particles.at(i * 10).position.z);
     for (int j = 0; j < 10; j++)
       {
          glTexCoord2f( i * 0.1f, j * 0.1f);
       glVertex3f(particles.at(i * 10 + j).position.x, particles.at(i * 10 + j).position.y,particles.at(i * 10 + j).position.z);
       }
//     glVertex3f(particles.at(i * 10 + 9).position.x, particles.at(i * 10 + 9).position.y, particles.at(i * 10 + 9).position.z);
     glEnd();
    }

  for (int j = 0; j < 10; j++)//竖线
  {
      glBegin(GL_LINE_STRIP);
//glVertex3f(particles.at(j).position.x, particles.at(j).position.y,particles.at(j).position.z);
      for (int i = 0; i < 10; i++)
      {
 glVertex3f(particles.at(i * 10 + j).position.x, particles.at(i * 10 + j).position.y, particles.at(i * 10 + j).position.z);

      }

//      glVertex3f(particles.at(90 + j).position.x, particles.at(90 + j).position.y, particles.at(90 + j).position.z);
      glEnd();
  }*/

   glutSwapBuffers();
//   glDisable(GL_TEXTURE_2D);
}
void setLight()
{
  //定义光源属性值
          GLfloat lightAmbient[] = { 0.4, 0.4, 0.4, 1.0 };           //环境光参数
          GLfloat lightDiffuse[] = { 0.1, 0.1, 0.1, 1.0 };           //漫反射光
          GLfloat lightSpecular[] = { 0.9, 0.9, 0.9 ,1.0 };          //镜面光
          GLfloat lightPosition[] = { 0.0, 2.0, 2.0, 1.0 };          //光源位置

          //设置光源属性值
          glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
          glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
          glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
          glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

          //启用光源
          glEnable(GL_LIGHTING);
          glEnable(GL_LIGHT0);
}

void reshape(GLsizei w,GLsizei h)
{
    height = h;
    width = w;
   glViewport(0,0,w,h);//设置视口
   glMatrixMode(GL_PROJECTION);//指明当前矩阵为GL_PROJECTION
   glLoadIdentity();//将当前矩阵置换为单位阵

     //缺省值时视点是在原点的
     gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 1.0, 4000.0f);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
//     gluLookAt(0.0f,0.0f, 600.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
     camera.setCamera(0.0f,0.0f, 600.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f);
//     camera.setLook();
}

void glInit()
{
  //用户自定的初始化过程
          glClearColor(0.0, 0.0, 0.0, 0.5);
          glClearDepth(1.0);
          glDepthFunc(GL_LEQUAL);                             //如果输入的深度值小于或等于参考值，则通过
          glEnable(GL_DEPTH_TEST);
          glEnable(GL_TEXTURE2);//there is no GL_TEXTUR_2D? enable texture or don't need enable
          glShadeModel(GL_SMOOTH);                           //设置着色模式.采用恒定着色时（即GL_FLAT），使用图元中某个顶点的颜色来渲染整个图元。在使用光滑着色时（即GL_SMOOTH）,对于线段图元，线段上各点的颜色将根据两个顶点的颜色通过插值得到。对于多边形图元，多边形内部区域的颜色将根据所有顶点的颜色插值得到。
          glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//真正精细的透视修正。这一行告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点。

          QImage tex, buf;
          cout<<"load";
          if(buf.load("/home/gu/11.jpg") == true)cout<<"true";
          tex = QGLWidget::convertToGLFormat(buf);
//          GLuint tex_id;
//          glGenTextures(1, &tex_id);
//          glBindTexture( GL_TEXTURE_2D, tex_id );
          //控制滤波，纹理过滤与环绕,  控制纹理参数
                  /*==============================
                  *这里改动会比较多做个记号
                  */
                  //==================================
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

                  //texture envirement
                  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                    glEnable(GL_TEXTURE_2D);
//                    glBindTexture(GL_TEXTURE_2D, tex_id);

                  cloth(10, 10);

                  glColor3f(1.0,1.0,1.0);//设置第一个顶点为红色
}

void timefunc(int i)
{
//  physics.tick(physicsStep);
  glutPostRedisplay();
  glutTimerFunc(18, timefunc, 1);
}

/*
void satisifyConstrains(Particle &p1, Particle &p2, float distance)
{
  Vector3 diff = p2.position - p1.position;
  float currentDist = diff.length();
  if(currentDist == 0)return;
  diff = diff * (1 - distance / currentDist) * 0.5;
  p1.position = p1.position + diff;
  p2.position = p2.position - diff;
}
*/

int index(int u, int v)
{
//  return u + v * ( w + 1 );
  return u + v * ( 10  );
}

void cloth(float w, float h)
{
  for(int v = 0; v < h; v ++ ) {

      for (int  u = 0; u < w; u ++ ) {

              particles.push_back(Particle( u / w, v / h, 0, MASS,restDistance, xSegs,ySegs ));

        }
   }
   for (int v = 0; v < h -1; v ++ ) {
          for ( int u = 0; u < w -1; u ++ ) {
                          constrains.push_back(constrain( &particles.at(index( u, v )), &particles.at(index( u, v + 1)), restDistance));
//                              constrains.push( [particles[ index( u, v ) ],particles[ index( u, v + 1 ) ],restDistance] );

                           constrains.push_back(constrain( &particles.at(index( u, v )), &particles.at(index( u + 1, v )), restDistance));

              }

     }

   for ( int u = w -1, v = 0; v < h -1; v ++ ) {
constrains.push_back(constrain( &particles.at(index( u, v )), &particles.at(index( u, v + 1)), restDistance));

           }

           for (int  v = h -1, u = 0; u < w -1; u ++ ) {
constrains.push_back(constrain( &particles.at(index( u, v )), &particles.at(index( u + 1, v )), restDistance));

           }
           //share springs
//           float diagonalDist = sqrt(restDistance * restDistance * 2);
//            for (int v=0;v<h - 1;v++) {
//                        for (int u=0;u<w - 1;u++) {

//                                constrains.push_back(constrain(&particles.at(index(u, v)), &particles.at(index(u + 1, v + 1)), diagonalDist));

//                                constrains.push_back(constrain(&particles.at(index(u + 1, v)), &particles.at(index(u , v + 1)), diagonalDist));

//                        }
//                    }
}

Vector3 computeNorml(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
  Vector3 ret;
//  Vector3 temp1 = particles.at(index(u, v)).position - particles.at(index(u + 1, v)).position;
   Vector3 temp1 = p1 - p2;
  Vector3 temp2 = p2 - p3;
  ret = temp1.crossProduct(temp2).normalize();
  return ret;
}
void simulate(time_t time)
{

  if(wind)
    {
       Vector3 temp1;
      for(int u = 0; u < xSegs - 1; ++u)
        for(int v = 0; v < ySegs -1; ++v)
          {
              temp1 = computeNorml(particles.at(index(u, v)).position, particles.at(index(u + 1, v)).position, particles.at(index(u , v + 1)).position);
              temp1*temp1.dotProduct(windForce);

              particles.at(index(u, v)).addAcceleration(temp1);
              particles.at(index(u + 1, v)).addAcceleration(temp1);
              particles.at(index(u, v + 1)).addAcceleration(temp1);
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
    }

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
          if(ball)
            {

              glColor3f(1.0, 1.0, 1.0);
              glPushMatrix();
              glTranslatef(ballPosition.x, ballPosition.y,ballPosition.z);
              glutSolidSphere(ballSize,20,20);
              glPopMatrix();
              for (int i = 0; i < particles.size(); i ++ ) {
                  Vector3 diff = particles.at(i).position - ballPosition;
                                      if ( diff.length() < ballSize + 4) {

                                              // collided/碰撞
                                              diff.normalize();
                                              diff*(ballSize + 4);
                                              particles.at(i).position = ballPosition + diff;

                                      }

                              }
            }

}
