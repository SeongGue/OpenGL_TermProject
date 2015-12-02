#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PIE	3.14
#define GROUND	-4900
#define X_END	5000
#define Y_END	5000
#define Z_END	5000

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Timerfunction(int value);
void Mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void Mouse_Move(int x, int y);
//-----------------------------------------------
void Cmera_change();


float t = 0.1;
float v = 0;

enum { XY_SURFACE = 0, PERSPECTIVE = 2 };

struct Point
{
	float x;
	float y;
	float z;
	bool collison;
};

int camera_viewpoint = XY_SURFACE;
int w1 = 0, h1 = 0;

float x = -X_END, y = -Y_END + 1000 , z = 0; //1픽셀당 1m
float speed = 1000; //초당 1000미터
float second = 0;
float xz = 0;
float xy_angle = 0;
float xz_angle = 0;

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
	Cmera_change();

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);//바닥
	{
		glVertex3f(-X_END, GROUND, -Z_END);
		glVertex3f(-X_END, GROUND, Z_END);
		glVertex3f(X_END, GROUND, Z_END);
		glVertex3f(X_END, GROUND, -Z_END);
	}
	glEnd();
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		glColor3f(1, 0, 0);
		glutSolidCube(50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(5000, GROUND, 0);
		glColor3f(0, 0, 1);
		glutSolidCube(1000);
	}
	glPopMatrix();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	w1 = w;
	h1 = h;
	//glEnable(GL_DEPTH_TEST);
}

void Mouse(int button, int state, int x, int y)
{

}

void Mouse_Move(int x, int y)
{

}

void keyboard(unsigned char key, int x, int y)
{
	if (key == '1')
	{
		camera_viewpoint = XY_SURFACE;
	}
	if (key == '2')
	{
		camera_viewpoint = PERSPECTIVE;
	}
}

void Timerfunction(int value)
{
	t = 0.025;
	second += t;
	xy_angle = PIE / 6;
	xz_angle = 0;
	v += 200 * t;
	xz = speed*cos(xy_angle);
	x += xz*cos(xz_angle) * t;
	y += speed*sin(xy_angle) * t - v / 2 * t;
	z += xz*sin(xz_angle) * t;
	printf("x = %.f, y = %.f, z = %.f, %.f \n", x, y, z, second);
	glutPostRedisplay();
	glutTimerFunc(10, Timerfunction, 1);
}

void Cmera_change()
{
	//glLoadIdentity();
	//if (camera_viewpoint == XY_SURFACE)
	//	glOrtho(-X_END, X_END, -Y_END, Y_END, -Z_END, Z_END);

	//if (camera_viewpoint == PERSPECTIVE){
	//	gluPerspective(60.0f, w1 / h1, 1.0, Z_END * 4);
	//	gluLookAt(-X_END * 2, -Y_END / 2, 0.0,
	//		0.0, -Y_END + 2000, 0.0,
	//		1.0, 1.0, 0.0);
	//}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 젂체를 칠하기
	
	glViewport(0, 0, 300, 300);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-X_END, X_END, -Y_END, Y_END, -Z_END, Z_END);
	glMatrixMode(GL_MODELVIEW);


	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);//바닥
	{
		glVertex3f(-X_END, GROUND, -Z_END);
		glVertex3f(-X_END, GROUND, Z_END);
		glVertex3f(X_END, GROUND, Z_END);
		glVertex3f(X_END, GROUND, -Z_END);
	}
	glEnd();
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		glColor3f(1, 0, 0);
		glutSolidCube(50);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(5000, GROUND, 0);
		glColor3f(0, 0, 1);
		glutSolidCube(1000);
	}
	glPopMatrix();

	glViewport(400, 0, 400, 300);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, w1 / h1, 1.0, Z_END * 4);
	gluLookAt(-X_END * 2, -Y_END / 2, 0.0,
		0.0, -Y_END + 2000, 0.0,
		1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
}
