#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PIE	3.14

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Timerfunction(int value);
void Mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void Mouse_Move(int x, int y);
//-----------------------------------------------

float t = 0.1;
float v = 0;

struct Point
{
	float x;
	float y;
	float z;
	bool collison;
};

float x = -400, y =-300, z = 0;
float speed = 200;

void main(int argc, char *argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Homework");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Mouse_Move);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(40, Timerfunction, 1);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 젂체를 칠하기

	glPushMatrix();
	{
		glTranslatef(x, y, z);
		glColor3f(1, 0, 0);
		glutSolidCube(50);
	}
	glPopMatrix();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(-400, 400, -300, 300, -300, 300);
}

void Mouse(int button, int state, int x, int y)
{

}

void Mouse_Move(int x, int y)
{

}

void keyboard(unsigned char key, int x, int y)
{

}

void Timerfunction(int value)
{
	t = 0.1;
	v += 9.8 * t;
	x += speed*cos(PIE/4) * t;
	y += speed*sin(PIE/4)*t - v/2;
	glutPostRedisplay();
	glutTimerFunc(40, Timerfunction, 1);
}
