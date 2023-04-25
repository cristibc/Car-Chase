
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>
#include <random>

using namespace std;

const double TWO_PI = 6.2831853;
double i = -50;
// unde se va opri masina pe axa x, cand face schimbarea de benzi
double j = 1.0;
double k = 0;
double iViteza = 260;
GLsizei winWidth = 750, winHeight = 750;
GLuint traficID;
static GLfloat rotTheta = 0.0;
GLboolean leftUpPressed = false;
GLboolean rightUpPressed = false;
int obstacol1, obstacol2, obstacol3;
bool initObstacle1, initObstacle2, initObstacle3;
double iObstacol1;
double iObstacol2;
double iObstacol3;


void delay(unsigned int milisecunde)
{
	clock_t goal = milisecunde + clock();
	while (goal > clock());
}

class scrPt
{
public:
	GLfloat x, y, z;
	void print(const char* text = " ") {
		cout << text << ' ' << x << ' ' << y << ' ' << z << endl;
	}
	scrPt operator + (scrPt const& a) {
		scrPt res;
		res.x = x + a.x;
		res.y = y + a.y;
		res.z = z + a.z;
		return res;
	}
	scrPt operator - (scrPt const& a) {
		scrPt res;
		res.x = x - a.x;
		res.y = y - a.y;
		res.z = z - a.z;
		return res;
	}
};

void displayText(int x, int y, float r, float g, float b, const char* string)
{
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void Obstacole(int value) {
	iObstacol1 = 0;
	iObstacol2 = 0;
	iObstacol3 = 0;

	initObstacle1 = false;
	initObstacle2 = false;
	initObstacle3 = false;

	srand(time(NULL));
	cout << "Called function" << endl;
	obstacol1 = rand() % 2;
	obstacol2 = rand() % 2;
	obstacol3 = rand() % 2;
	int sumObstacol = obstacol1 + obstacol2 + obstacol3;
	while (sumObstacol < 1 || sumObstacol > 2) {
		cout << obstacol1 << " " << obstacol2 << " " << obstacol3 << endl;
		obstacol1 = rand() % 2;
		obstacol2 = rand() % 2;
		obstacol3 = rand() % 2;
		sumObstacol = obstacol1 + obstacol2 + obstacol3;
	}
	cout << "Good obstacle path: "<< obstacol1 << " " << obstacol2 << " " << obstacol3 << endl;


	if (obstacol1 == 1) {
		initObstacle1 = true;
	}

	if (obstacol2 == 1) {
		initObstacle2 = true;
	}

	if (obstacol3 == 1) {
		initObstacle3 = true;
	}
	glutTimerFunc(3000, Obstacole, 0);  //repost timer 
}


void miscareGirofar(void) {
	// pentru girofar
	j += 0.05;
	if (j > 360) {
		j = 0;
	}
	if (j < 0) {
		j = 360;
	}

	if (leftUpPressed && i > -190) {
		i -= 1.3;
	}

	if (rightUpPressed && i < 90) {
		i += 1.3;
	}

	iViteza += 1.5;
	if (iViteza > 519) {
		iViteza = 260;
	}


	if (initObstacle1 == 1 && iObstacol1 > -600) {
		iObstacol1 -= 1.5;
	}

	if (initObstacle2 == 1 && iObstacol2 > -600) {
		iObstacol2 -= 1.5;
	}

	if (initObstacle3 == 1 && iObstacol3 > -600) {
		iObstacol3 -= 1.5;
	}

	// add power-ups


	glutPostRedisplay();
}

static void init(void)
{
	scrPt cerc;
	GLdouble theta;
	GLint aux;
	GLint n = 2000;

	// 159 / 255 , 61 / 255 , 0 , 1
	glClearColor(0.6235294, 0.239215, 0, 1);
	gluOrtho2D(-100, 100, -100, 100);


	traficID = glGenLists(1);


	for (aux = 0; aux < 20; aux++)
	{
		glColor3f(0, 1, 0);
		glRectf(20, 100 + aux * 20, 60, 160 + aux * 20);

		glColor3f(0.5, 0, 0.5);
		glRectf(-120, 250 + aux * 20, -80, 310 + aux * 20);
	}
	glEndList();

	// pentru opacitatea culorilor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void keyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		leftUpPressed = false;
		break;
	case GLUT_KEY_RIGHT:
		rightUpPressed = false;
		break;
	default:
		break;
	}

}

void keyPressed(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		leftUpPressed = true;
		break;
	case GLUT_KEY_RIGHT:
		rightUpPressed = true;
		break;
	default:
		break;
	}
}


void desenDrum(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	// DRUMUL
	glColor3f(0.2, 0.2, 0.2);
	glRectf(-200., -1000., 200., 1000.);

	// Linia ce separa benzile - dashed
	glEnable(GL_LINE_STIPPLE);
	glColor3f(1, 1, 1);
	glLineStipple(3, 0x0FFF);
	glLineWidth(7.5);
	glBegin(GL_LINES);

	glVertex2f(-75, -iViteza);
	glVertex2f(-75, iViteza);

	glVertex2f(75, -iViteza);
	glVertex2f(75, iViteza);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	// initializare pentru tranzitii
	glPushMatrix();
	glTranslated(i, 0, 0);

	// MASINA DE POLITIE
	// sasiu
	glColor3f(0, 0, 0);
	glRectf(20, -180, 80, -155);
	glColor3f(1, 1, 1);
	glRectf(20, -155, 80, -115);
	glColor3f(0, 0, 0);
	glRectf(20, -115, 80, -90);


	// stopuri
	glColor3f(1, 0, 0);
	glRectf(25, -185, 35, -180);
	glColor3f(1, 0, 0);
	glRectf(65, -185, 75, -180);

	// faruri fata
	glColor3f(1, 1, 0);
	glRectf(25, -90, 35, -85);
	glColor3f(1, 1, 0);
	glRectf(65, -90, 75, -85);

	// faruri triunghi original
	//glColor3f(0.8, 0.5, 0);
	//glRectf(25, -90, 35, -85);
	//glColor3f(0.8, 0.5, 0);
	//glRectf(65, -90, 75, -85);


	//glColor3f(1, 1, 0);
	//glBegin(GL_TRIANGLES);
	//glVertex2f(30, -85);
	//glVertex2f(0, 0);
	//glVertex2f(60, 0);

	//glVertex2f(40, 0);
	//glVertex2f(100, 0);
	//glVertex2f(70, -85);

	glEnd();


	// GIROFAR


	glTranslated(50, -135, 0);
	glRotated(j, 0, 0, 1);
	glTranslated(-50, 135, 0);

	//girofar
	glColor4f(1, 0, 0, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex2f(10, -115);
	glVertex2f(50, -135);
	glVertex2f(10, -155);
	glEnd();


	glColor4f(0, 0, 1, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex2f(50, -135);
	glVertex2f(90, -115);
	glVertex2f(90, -155);
	glEnd();

	glPopMatrix();

	glPopMatrix();

	// Obstacole

	glPushMatrix();
	glTranslated(0, iObstacol1, 0);
	glColor3f(0, 0, 0);
	glRectf(-175, 270, -105, 300);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, iObstacol2, 0);
	glColor3f(0, 0, 0);
	glRectf(-35, 270, 35, 300);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, iObstacol3, 0);
	glColor3f(0, 0, 0);
	glRectf(110, 270, 180, 300);
	glPopMatrix();

	// incrementare pentru girofar si alte translatii
	glutIdleFunc(miscareGirofar);

	displayText(-250, 230, 1, 1, 1, "Alpha Build");

	glutSwapBuffers();
	glFlush();
}


void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
	//gluOrtho2D(-500.0, 500.0, -500.0, 500.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Trafic - Masina de politie");

	init();

	glutDisplayFunc(desenDrum);
	glutReshapeFunc(winReshapeFcn);
	glutTimerFunc(3000, Obstacole, 0);

	glutSpecialFunc(keyPressed);
	glutSpecialUpFunc(keyUp);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();

}

