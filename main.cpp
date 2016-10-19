#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>//to get millisec making the ball move smothly,non stand library.use kernal lib sys(in linux).

#include <QtGui/QImage>
#include <QtOpenGL/QGLWidget>

#include <GL/freeglut.h>
#include <GL/glu.h>
#include "cloth.h"

#include "particle.h"
#include "constrain.h"
#include "camera.h"
#include "key.h"


using namespace std;

/*布料类 */
vector<cloth *> cloths;

/*实验测试数据，控制fps*/
bool idle = true;
bool finish = false;

float height,width;/*gl窗口大小*/
//mouse
int posx,posy;

//camera
Camera camera;
//key
Keys keys;

GLuint tex_id;

void timefunc(int i);//刷新画面
void setLight(); //设置光源
void reshape(GLsizei w,GLsizei h);//

void glInit();//初始化，以及cloth函数调用，创建布料
void draw();//绘制的回调函数，调用simulate函数，计算质点位置，将数据绘制出来。

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

    if(idle)
      glutIdleFunc(draw);
    else
    glutTimerFunc(18, timefunc, 1);
//    glutKeyboardFunc();
//    glutMouseFunc(mousefunc);
//    glutMotionFunc(motionfunc);
//    glutKeyboardFunc(keyfunc);
//    glutKeyboardUpFunc(keyupfunc);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);//使程序在mainloop结束之后继续执行之后的代码。
  glutMainLoop();

  cout<<endl;
  cout<<cloths[0]<<endl;
  cout<<cloths[0]->getx()<<endl;
for(int i = 0; i < cloths.size(); ++i)
  {
    delete cloths[i];
  }
cout<<endl;
cout<<cloths[0]<<endl;
cout<<cloths[0]->getx()<<endl;
cout<< "after glMainLoop, delete the cloth mem"<<endl;
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

  /*
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
  */
 struct timeb t1;
 ftime(&t1);
 long t =  t1.time * 1000 + t1.millitm;
 cloths[0]->simulate(t);
  //texture envirement
//  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, tex_id);

 /* 传递cloth中的质点类，有问题，用共有成员暂时解决
vector<Particle> particles;
particles = cloths[0]->getp();
*/
 glColor3f(1.0, 1.0, 1.0);
               glPushMatrix();
               Vector3 bp = cloths[0]->getballp();
               glTranslatef(bp.x, bp.y, bp.z);
               double ballsize = cloths[0]->getballs();
               glutSolidSphere(ballsize ,20,20);
               glPopMatrix();

  for(int i = 0; i < cloths[0]->getx() - 1; ++i)
    {
      glBegin(GL_TRIANGLE_STRIP);
      for(int j = 0; j < cloths[0]->gety(); j ++)
        {
          glTexCoord2f(i * 0.1, j * 0.1); //it should be i * 1 / 11 i think for there are 11 coord,
          Vector3 p = cloths[0]->particles[i * 10 + j].position;
           glVertex3f(p.x, p.y, p.z);
//           glVertex3f(cloths[0]->particles.at(i * 10 + j).position.x, cloths[0]->particles.at(i * 10 + j).position.y, cloths[0]->particles.at(i * 10 + j).position.z);

           glTexCoord2f((i+ 1) * 0.1, j * 0.1); //it should be i * 1 / 11 i think for there are 11 coord,
            glVertex3f(cloths[0]->particles.at( (i + 1) * 10 + j).position.x, cloths[0]->particles.at((i + 1) * 10 + j).position.y, cloths[0]->particles.at((i + 1) * 10 + j).position.z);
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

                  cloth *cloth1 = new cloth;
                  cloth1->crt_cloth(10, 10);
                  cloths.push_back(cloth1);

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


