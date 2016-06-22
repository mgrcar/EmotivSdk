/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - GyroData
** This example allows built-in 2-axis gyroscope position
** Simply turn your head from left to right, up and down.
** You will also notice the red indicator dot move in accordance with
** the movement of your head/gyroscope
**
****************************************************************************/

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#include <iostream>

#if __linux__|| __APPLE__
#include <unistd.h>
#endif
#include <cmath>

#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"

#define PI 3.1418

bool  outOfBound = false;
float currX      = 0,
      currY      = 0;
float xmax       = 0,
      ymax       = 0,
      x          = 0;
float preX       = 0,
      preY       = 0;
int   incOrDec   = 10;
int   count      = 0;
float oldXVal    = 0,
      oldYVal    = 0;
double maxRadius = 10000;
unsigned long pass = 0,
              globalElapsed = 0;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void drawCircle( float Radius, int numPoints )
{
  glBegin( GL_LINE_STRIP );
    for( int i=0; i<numPoints; i++ )
    {
		float Angle = i * (2.0*PI/numPoints); // use 360 instead of 2.0*PI if
		float X = cos( Angle )*Radius;  // you use d_cos and d_sin
		float Y = sin( Angle )*Radius;
		glVertex2f( X, Y );		
	}
  glEnd();
}

void drawFilledCircle(float radius)
{
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	//glPoint(0, 0, 0); 
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
  
   glColor3f(1.0,1.0,1.0);
   drawCircle(800,100);
   glColor3f(0.0,0.0,1.0);
   drawCircle(maxRadius-4000,800);
   glColor3f(0.0,1.0,1.0);
   drawCircle(maxRadius,1000);
	
  
   glColor3f(1.0, 0.0, 0.0);   
   glRectf(currX-400.0, currY-400.0, currX+400.0, currY+400.0);
   
   glPopMatrix();
   glutSwapBuffers();
}

void changeXY(int x) // x = 0 : idle
{
    if( currX >0 )
    {
        float temp = currY/currX;
        currX -= incOrDec;
        currY = temp*currX;
    }
    else if( currX < 0)
    {
        float temp = currY/currX;
        currX += incOrDec;
        currY = temp*currX;
    }
    else
    {
        if( currY > 0 ) currY -= incOrDec;
        else if( currY <0 ) currY += incOrDec;
    }
    if( x == 0)
    {
        if( (std::abs(currX) <= incOrDec) && (std::abs(currY) <= incOrDec))
        {
            xmax = 0;
            ymax = 0;
        }
        else
        {
            xmax = currX;
            ymax = currY;
        }
    }
    else
    {
        if( (std::abs(currX) <= incOrDec) && (std::abs(currY) <= incOrDec))
        {
            xmax = 0;
            ymax = 0;
        }
    }
}


void updateDisplay(void)
{	  
   int gyroX = 0, gyroY = 0;
   IEE_HeadsetGetGyroDelta(0, &gyroX, &gyroY);
   xmax += gyroX;
   ymax += gyroY;

   if( outOfBound )
   {
	   if( preX != gyroX && preY != gyroY )
	   {
		   xmax = currX;
		   ymax = currY;
	   }
   }

   double val = sqrt((float)(xmax*xmax + ymax*ymax));
  
    std::cout <<"xmax : " << xmax <<" ; ymax : " << ymax << std::endl;

   
   if( val >= maxRadius )
   {
	   changeXY(1);	
	   outOfBound = true;
	   preX = gyroX;
	   preY = gyroY;
   }
   else
   {		
	   outOfBound = false;
		if(oldXVal == gyroX && oldYVal == gyroY)
		{
			++count;
			if( count > 10 )
			{									
				changeXY(0);
			}
		}
		else
		{
			count = 0;
			currX = xmax;
			currY = ymax;
			oldXVal = gyroX;
			oldYVal = gyroY;			
		}
   }
#ifdef _WIN32
      Sleep(15);
#endif
#if __linux__ || __APPLE__
      usleep(10000);
#endif
   glutPostRedisplay(); 
}
void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50000.0, 50000.0, -50000.0, 50000.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(updateDisplay);
         break;
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   } 	
}

#if __linux__
double GetTickCount()
{
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now))
      return 0;
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}
#endif
#ifdef __APPLE__
double GetTickCount()
{
    struct timespec ts;
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}
#endif


/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
	EmoEngineEventHandle hEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();
	unsigned int userID = -1;
	int state           = 0;
	bool ready          = false;
	
	if (IEE_EngineConnect() != EDK_OK) {
		std::cout << "Emotiv Driver start up failed.";
		return -1;
	}
	
	std::cout << "Please make sure your headset is on and don't move your head.";
	std::cout << std::endl;

	while(true)
	{

		state = IEE_EngineGetNextEvent(hEvent);

		if (state == EDK_OK) 
		{
		    IEE_Event_t eventType = IEE_EmoEngineEventGetType(hEvent);
			IEE_EmoEngineEventGetUserId(hEvent, &userID);

			if (eventType == IEE_UserAdded) {
		         std::cout << "User added" << std::endl << std::endl;
				 userID = 0;
		         ready = true;
			}
		}

		if(!ready) continue;

		int gyroX = 0, gyroY = 0;
		int err = IEE_HeadsetGetGyroDelta(userID, &gyroX, &gyroY);
				
		if (err == EDK_OK){
			std::cout << std::endl;
			std::cout << "You can move your head now." << std::endl;

#ifdef _WIN32
			Sleep(1000);
#endif
#if __linux__ || __APPLE__
            usleep(10000);
#endif
			break;
		}else if (err == EDK_GYRO_NOT_CALIBRATED){
			std::cout << "Gyro is not calibrated. Please stay still." << std::endl;
		}else if (err == EDK_CANNOT_ACQUIRE_DATA){
			std::cout << "Cannot acquire data" << std::endl;
		}else{
			std::cout << "No headset is connected" << std::endl;
		}

#ifdef _WIN32
		Sleep(100);
#endif
#if __linux__ || __APPLE__
        usleep(10000);
#endif
	}

#ifdef _WIN32
   globalElapsed = GetTickCount();
#endif
#if __linux__|| __APPLE__
   globalElapsed = ( unsigned long ) GetTickCount();
#endif

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (650, 650); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutIdleFunc(updateDisplay);
   glutMainLoop();  
      
   IEE_EngineDisconnect();
   IEE_EmoStateFree(eState);
   IEE_EmoEngineEventFree(hEvent);	

   return 0;
}


while (true) {
	if (kbkit) {
	}
	else {
	}
}