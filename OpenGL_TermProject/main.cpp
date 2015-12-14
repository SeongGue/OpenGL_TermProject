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
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void specialkeyUp(int key, int x, int y);
//-----------------------------------------------
void Camera_change();
void createCylinder(GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat radius, GLfloat h);//원기둥
void draw_canon();
void draw_ground();
//void draw_building();
void draw_shell();
void draw_castle(int castle_num);

void shellArc();//포물선 운동 함수
struct Point;
bool collide(const Point&, const Point&);

float t = 0.1;
float v = 0;


enum { XY_SURFACE = 0, PERSPECTIVE = 1, MULTY_VIEW = 2 };
enum {ORBIT_SET = 0, GAGE_CHARGE = 1, FIRE = 2,
		UP, DOWN, STOP, RIGHT, LEFT};//캐논 열거형

struct Point
{
	float x;
	float y;
	float z;
	float size_x;
	float size_y;
	float size_z;
};

//=======카메라 변수========//
int camera_viewpoint = XY_SURFACE;
int w1 = 0, h1 = 0;

//======캐논 변수==========//
int canon_state = ORBIT_SET;
int set_dir = STOP;
float canon_angle = 15;
float save_angle = 0;
Point cannon = { 0, 0, 0, 100, 100, 100 };

//======포탄 포물선 운동 변수======//
//float x = -X_END, y = -Y_END + 1000 , z = 0; //1픽셀당 1m
float missile_speed = 0; //초당 1000미터
float xz = 0;
float xy_angle = PIE/15;
float xz_angle = 0;
float shell_angle = PIE/12;

//Point missile_c = {-X_END, -Y_END + 1000, 0, 50, 50, 50};
Point missile_c = { -4300, -4400, 0, 50, 50, 50 };
Point building_c[3] = { { 4000, GROUND + 500, 0, 1000, 1000, 1000 },
						{ 2500, GROUND + 500, 3500, 1000, 1000, 1000 }, 
						{ 2500, GROUND + 500, -3500, 1000, 1000, 1000 }
					};
//Point b = { 0, 0, 0, 100,100,100 };

float angle = 0;
float angle_x = 0;
float angle_z = 0;


void main(int argc, char *argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Turmp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKey);
	glutSpecialUpFunc(specialkeyUp);
	glutTimerFunc(40, Timerfunction, 1);
	glutMainLoop();
}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 젂체를 칠하기

	Camera_change();

	glRotatef(angle, 0, 1, 0);
	glRotatef(angle_x, 1, 0, 0);
	glRotatef(angle_z, 0, 0, 1);

	draw_ground();
	draw_canon();
	//draw_building();
	draw_shell();
	for (int i = 0; i < 3; i++)
		draw_castle(i);

	//printf("%d \n", collide(test_a, test_b));
	printf("speed = %f \n", missile_speed);
	//printf("%d \n", orbit_up_down);
	printf("xy_angle = %f \n", xy_angle);
	printf("xz_angle = %f \n", xz_angle);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	w1 = w;
	h1 = h;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == '1')
	{
		camera_viewpoint = XY_SURFACE;
	}
	else if (key == '2')
	{
		camera_viewpoint = PERSPECTIVE;
	}
	else if (key == '3')
	{
		camera_viewpoint = MULTY_VIEW;
	}
	if (key == ' ')
	{
		canon_state = GAGE_CHARGE;
	}
	if (key == 'Y')
	{
		angle += 15;
	}
	if (key == 'y')
	{
		angle -= 15;
	}
	if (key == 'X')
	{
		angle_x += 15;
	}
	if (key == 'x')
	{
		angle_x -= 15;
	}
	if (key == 'Z')
	{
		angle_z += 15;
	}
	if (key == 'z')
	{
		angle_z -= 15;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		canon_state = FIRE;
	}
}

void specialKey(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		set_dir = UP;
	if (key == GLUT_KEY_DOWN)
		set_dir = DOWN;
	if (key == GLUT_KEY_LEFT)
		set_dir = LEFT;
	if (key == GLUT_KEY_RIGHT)
		set_dir = RIGHT;
}
void specialkeyUp(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		set_dir = STOP;
	else if (key == GLUT_KEY_DOWN)
		set_dir = STOP;
	else if (key == GLUT_KEY_LEFT)
		set_dir = STOP;
	else if (key == GLUT_KEY_RIGHT)
		set_dir = STOP;
}

void Timerfunction(int value)
{
	if (canon_state == FIRE)
		shellArc();
	if (canon_state == GAGE_CHARGE && missile_speed < 1000)
		missile_speed += 50;
	if (canon_state == ORBIT_SET)
	{
		if (set_dir == UP){
			if (xy_angle < PIE / 2)
				xy_angle += PIE / 360;
		}
		else if (set_dir == DOWN){
			if (xy_angle > 0)
				xy_angle -= PIE / 360;
		}
		else if (set_dir == LEFT){
			if (xz_angle > -PIE / 4)
				xz_angle -= PIE / 360;
		}
		else if (set_dir == RIGHT){
			if (xz_angle < PIE / 4)
				xz_angle += PIE / 360;
		}
		else
			canon_angle = canon_angle;
	}
	glutPostRedisplay();
	glutTimerFunc(10, Timerfunction, 1);
}
//===============그리기 함수===================//

void draw_ground()
{
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);//바닥
	{
		glVertex3f(-X_END, GROUND, -Z_END);
		glVertex3f(-X_END, GROUND, Z_END);
		glVertex3f(X_END, GROUND, -Z_END);
	}
	glEnd();
}


void draw_shell()
{
	glPushMatrix();//포탄
	{
		glTranslatef(missile_c.x, missile_c.y, missile_c.z);
		glColor3f(1, 0, 0);
		glutSolidCube(50);
	}
	glPopMatrix();
}

void draw_canon()
{
	glPushMatrix();
	{
		glTranslatef(-4350, -4500, 0);
		glRotatef(-90, 0, 1, 0);
		glPushMatrix();
		{
			glTranslatef(0, -50, 400);
			glRotatef(canon_angle, 1, 0, 0);
			glTranslatef(0, 50, -400);
			glColor3f(0.2, 0.2, 0.2);
			createCylinder(cannon.x, cannon.y, cannon.z, 50, 250);
			glColor3f(0.2, 0.2, 0.2);
			createCylinder(cannon.x, cannon.y, cannon.z + 250, 100, 250);
		}
		glPopMatrix();
		glColor3f(0, 0, 1);
		glPushMatrix();
		{
			glTranslatef(cannon.x, cannon.y - 50, cannon.x + 400);
			glPushMatrix();
			{
				glTranslatef(100, 0, 0);
				glScalef(0.1, 1, 1);
				glutSolidSphere(70, 20, 20);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(-100, 0, 0);
				glScalef(0.1, 1, 1);
				glutSolidSphere(70, 20, 20);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}
//=================기능 함수=====================//
void Camera_change()
{
	glLoadIdentity();
	if (camera_viewpoint == XY_SURFACE){
		glViewport(0, 0, w1, h1);
		glOrtho(-X_END, X_END, -Y_END, Y_END, -Z_END, Z_END);
	}

	else if (camera_viewpoint == PERSPECTIVE){
		glViewport(0, 0, w1, h1);
		gluPerspective(60.0f, w1 / h1, 1.0, Z_END * 4);
		gluLookAt(-X_END * 2, -Y_END / 2, 0.0,
			0.0, -Y_END + 2000, 0.0,
			1.0, 1.0, 0.0);
		//gluLookAt(0, 0, 1000.0,
		//	0.0, 0.0, 0.0,
		//0.0, 1.0, 0.0);
	}
	else if (camera_viewpoint == MULTY_VIEW)
	{
		glViewport(0, 0, 400, 300);
		glOrtho(-X_END, X_END, -Y_END, Y_END, -Z_END, Z_END);

		glLoadIdentity();
		glViewport(400, 0, 400, 300);
		gluPerspective(60.0f, w1 / h1, 1.0, Z_END * 4);
		gluLookAt(-X_END * 2, -Y_END / 2, 0.0,
			0.0, -Y_END + 2000, 0.0,
			1.0, 1.0, 0.0);
	}
}

void shellArc()
{
	t = 0.025;
	v += 200 * t;
	xz = missile_speed*cos(xy_angle);
	missile_c.x += xz*cos(xz_angle) * t;
	missile_c.y += missile_speed*sin(xy_angle) * t - v / 2 * t;
	missile_c.z += xz*sin(xz_angle) * t;
}

bool collide(const Point& a,const Point& b)
{
	float a_left = a.x - a.size_x / 2;
	float a_right = a.x + a.size_x / 2;
	float a_top = a.y + a.size_y / 2;
	float a_bottom = a.y - a.size_y / 2;
	float a_front = a.z + a.size_z / 2;
	float a_back = a.z - a.size_z / 2;

	float b_left = b.x - b.size_x / 2;
	float b_right = b.x + b.size_x / 2;
	float b_top = b.y + b.size_y / 2;
	float b_bottom = b.y - b.size_y / 2;
	float b_front = b.z + b.size_z / 2;
	float b_back = b.z - b.size_z / 2;


	if (a_right < b_left) return false;
	if (a_left > b_right) return false;
	if (a_bottom > b_top) return false;
	if (a_top < b_bottom) return false;
	if (a_front < b_back) return false;
	if (a_back > b_front) return false;

	return true;
	

}

//=================그리기 함수====================//
void createCylinder(GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat radius, GLfloat h)
{
	/* function createCyliner()
	원기둥의 중심 x,y,z좌표, 반지름, 높이를 받아 원기둥을 생성하는 함수(+z방향으로 원에서 늘어남)
	centerx : 원기둥 원의 중심 x좌표
	centery : 원기둥 원의 중심 y좌표
	centerz : 원기둥 원의 중심 z좌표
	radius : 원기둥의 반지름
	h : 원기둥의 높이
	*/
	GLfloat x, y, angle;

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 윗면
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(centerx, centery, centerz);// <p>< / p>

	for (angle = 0.0f; angle < (2.0f*PIE); angle += (PIE / 8.0f))
		{
			x = centerx + radius*sin(angle);
			y = centery + radius*cos(angle);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(x, y, centerz);
		}
	glEnd();

	glBegin(GL_QUAD_STRIP);            //원기둥의 옆면
	for (angle = 0.0f; angle < (2.0f*PIE); angle += (PIE / 8.0f))
	{
		x = centerx + radius*sin(angle);
		y = centery + radius*cos(angle);
		glNormal3f(sin(angle), cos(angle), 0.0f);
		glVertex3f(x, y, centerz);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 밑면
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(centerx, centery, centerz + h);
	for (angle = (2.0f * PIE); angle > 0.0f; angle -= (PIE / 8.0f))
	{
		x = centerx + radius*sin(angle);
		y = centery + radius*cos(angle);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();
}


void draw_castle(int castle_num)
{
	glPushMatrix();
	{
		glTranslatef(building_c[castle_num].x, building_c[castle_num].y, building_c[castle_num].z);
		glScalef(0.2, 0.2, 0.2);
		//가운데성
		glPushMatrix();
		{
			glTranslated(0, 2650, 0);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(4000);
		}
		glPopMatrix();
		//가운데성탑
		glPushMatrix();
		{
			glTranslated(0, 5200, -1000);
			glScaled(1, 1.5, 1);
			glColor3f(0.6, 0.6f, 0.6f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//가운데성탑뿔
		glPushMatrix();
		{
			glTranslated(0, 6000, -1000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//가운데오른성탑
		glPushMatrix();
		{
			glTranslated(1250, 5200, 1000);
			glColor3f(0.6, 0.6f, 0.6f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//가운데오른성탑뿔
		glPushMatrix();
		{
			glTranslated(1250, 5700, 1000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//가운데왼성탑
		glPushMatrix();
		{
			glTranslated(-1250, 5200, 1000);
			glColor3f(0.6, 0.6f, 0.6f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//가운데왼성탑뿔
		glPushMatrix();
		{
			glTranslated(-1250, 5700, 1000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//성벽앞
		glPushMatrix();
		{
			glTranslated(0, 1150, 3000);
			glScaled(2, 1, 0);
			glColor3f(0.2f, 0.2f, 0.2f);
			glutSolidCube(3000);
		}
		glPopMatrix();
		//앞왼쪽성탑
		glPushMatrix();
		{
			glTranslated(-3000, 1150, 3000);
			glScaled(1, 3, 1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//앞오른쪽성탑
		glPushMatrix();
		{
			glTranslated(3000, 1150, 3000);
			glScaled(1, 3, 1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//앞왼쪽위성탑
		glPushMatrix();
		{
			glTranslated(-3000, 3150, 3000);
			glScaled(1.5, 1, 1.5);
			glColor3f(0.4f, 0.4f, 0.4f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//앞오른쪽위성탑
		glPushMatrix();
		{
			glTranslated(3000, 3150, 3000);
			glScaled(1.5, 1, 1.5);
			glColor3f(0.4f, 0.4f, 0.4f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//앞왼쪽위성탑뿔
		glPushMatrix();
		{
			glTranslated(-3000, 3650, 3000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//앞오른쪽위성탑뿔
		glPushMatrix();
		{
			glTranslated(3000, 3650, 3000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//성벽뒤
		glPushMatrix();
		{
			glTranslated(0, 1150, -3000);
			glScaled(2, 1, 0);
			glColor3f(0.2f, 0.2f, 0.2f);
			glutSolidCube(3000);
		}
		glPopMatrix();
		//뒤오른쪽성탑
		glPushMatrix();
		{
			glTranslated(-3000, 1150, -3000);
			glScaled(1, 3, 1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//뒤왼쪽성탑
		glPushMatrix();
		{
			glTranslated(3000, 1150, -3000);
			glScaled(1, 3, 1);
			glColor3f(0.5f, 0.5f, 0.5f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//뒤오른쪽위성탑
		glPushMatrix();
		{
			glTranslated(-3000, 3150, -3000);
			glScaled(1.5, 1, 1.5);
			glColor3f(0.4f, 0.4f, 0.4f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//뒤왼쪽위성탑
		glPushMatrix();
		{
			glTranslated(3000, 3150, -3000);
			glScaled(1.5, 1, 1.5);
			glColor3f(0.4f, 0.4f, 0.4f);
			glutSolidCube(1000);
		}
		glPopMatrix();
		//뒤왼쪽위성탑뿔
		glPushMatrix();
		{
			glTranslated(-3000, 3650, -3000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//뒤오른쪽위성탑뿔
		glPushMatrix();
		{
			glTranslated(3000, 3650, -3000);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(750, 1000, 100, 100);
		}
		glPopMatrix();
		//성벽오른쪽
		glPushMatrix();
		{
			glTranslated(3000, 1150, 0);
			glScaled(0, 1, 2);
			glColor3f(0.2f, 0.2f, 0.2f);
			glutSolidCube(3000);
		}
		glPopMatrix();
		//성벽왼쪽
		glPushMatrix();
		{
			glTranslated(-3000, 1150, 0);
			glScaled(0, 1, 2);
			glColor3f(0.2f, 0.2f, 0.2f);
			glutSolidCube(3000);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();//충돌 박스
	{
		glTranslatef(building_c[castle_num].x, building_c[castle_num].y + 500, building_c[castle_num].z);
		glColor3f(0, 0, 1);
		glutSolidCube(1000);
	}
	glPopMatrix();

	printf("castle[%d] = %d \n", castle_num, collide(missile_c, building_c[castle_num]));
}