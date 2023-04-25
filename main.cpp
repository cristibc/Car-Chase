#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>
#include <random>
#include <string> 

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
double iObstacol1, iObstacol2, iObstacol3;
bool collisionCheck = false;
double score = 0;



void delay(unsigned int milisecunde)
{
	clock_t goal = milisecunde + clock();
	while (goal > clock());
}

void increaseScore(int value) {
		score += 1;

	glutTimerFunc(100, increaseScore, 0);
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
	// Initializam indicele pentru translatie
	iObstacol1 = 0;
	iObstacol2 = 0;
	iObstacol3 = 0;

	// Check-uri pentru existenta obstacolelor
	initObstacle1 = false;
	initObstacle2 = false;
	initObstacle3 = false;

	// Generam un set de obstacole pozitionate random, intre 1 si 2 obstacole
	if (collisionCheck != true) {
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
		cout << "Good obstacle path: " << obstacol1 << " " << obstacol2 << " " << obstacol3 << endl;


		if (obstacol1 == 1) {
			initObstacle1 = true;
		}

		if (obstacol2 == 1) {
			initObstacle2 = true;
		}

		if (obstacol3 == 1) {
			initObstacle3 = true;
		}
	}
		// Timer pentru loop-ul obstacolelor
		glutTimerFunc(4000, Obstacole, 0);
	
}

void Collision(double x1, double y1, double x2, double y2, double xWidth, double xHeight, double yWidth, double yHeight) {
	//double x1, y1, x2, y2, width, height;
	if (x1 < x2 + yWidth && x1 + xWidth > x2 && y1 < y2 + yHeight && y1 + xHeight > y2)
	{
		cout << "Collision detected\n";
		collisionCheck = true;
	}


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

	// Pentru input-ul left key - right key
	if (leftUpPressed && i > -190) {
		i -= 1.3 + score / 1600;
	}

	if (rightUpPressed && i < 90) {
		i += 1.3 + score / 1600;
	}

	// Indice pentru axul drumului, indica viteza scenei
	iViteza += 1 + score/400;
	if (iViteza > 519) {
		iViteza = 260;
	}

	// Indice pentru translatia obstacolelor
	if (initObstacle1 == 1 && iObstacol1 > -650) {
		iObstacol1 -= 1 + score/400;
	}

	if (initObstacle2 == 1 && iObstacol2 > -650) {
		iObstacol2 -= 1 + score / 400;
	}

	if (initObstacle3 == 1 && iObstacol3 > -650) {
		iObstacol3 -= 1 + score / 400;
	}

	// Collision check
	Collision(-160, iObstacol1 + 360, i + 20, -90, 65, 90, 65, 90);
	Collision(-35, iObstacol2 + 360, i + 20, -90, 65, 90, 65, 90);
	Collision(95, iObstacol3 + 360, i + 20, -90, 65, 90, 65, 90);


	//TODO add power-ups


	glutPostRedisplay();
}

static void init(void)
{
	scrPt cerc;
	GLdouble theta;
	GLint aux;
	GLint n = 2000;

	// 159 / 255 , 61 / 255 , 0 , 1
	glClearColor(0, 0.839, 0.082, 1);
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

// Functie check activata la eliberarea tastelor
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

// Functie check activata la apasarea tastelor, implementare smooth
void keyPressed(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		leftUpPressed = true;
		break;
	case GLUT_KEY_RIGHT:
		rightUpPressed = true;
		break;
	case GLUT_KEY_HOME:
		glClearColor(0, 0.839, 0.082, 1);
		collisionCheck = false;
		score = 0;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void desenDrum(void)
{
	if (collisionCheck != true)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(0, 0.522, 0.051);
		glRectf(-1000., -1000., 1000, 1000.);


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
		//glRectf(-175, 270, -105, 300); // placeholder cube obstacle
		glRectf(-160, 270, -95, 360);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, iObstacol2, 0);
		glColor3f(0, 0, 0);
		//glRectf(-35, 270, 35, 300);
		glRectf(-35, 270, 30, 360);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, iObstacol3, 0);
		glColor3f(0, 0, 0);
		//glRectf(110, 270, 180, 300);
		glRectf(95, 270, 160, 360);
		glPopMatrix();

		// incrementare pentru girofar si alte translatii
		glutIdleFunc(miscareGirofar);

		displayText(-240, 220, 1, 1, 1, "Score: ");
		string scoreString = to_string(int(score));
		const char* scoreDisplayable = scoreString.c_str();
		displayText(-195, 219, 1, 1, 1, scoreDisplayable);

		glutSwapBuffers();
		glFlush();

	}
	else {
		int highScore = score;
		string highScoreString = to_string(highScore);
		const char* highScoreDisplayable = highScoreString.c_str();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glutIdleFunc(NULL);
		iObstacol1 = 0;
		iObstacol2 = 0;
		iObstacol3 = 0;
		glColor3f(0.5, 1, 1);
		displayText(-50, 0, 1, 1, 1, "Game Over!");
		displayText(-60, -30, 1, 1, 1, "High Score: ");
		displayText(23, -31, 1, 1, 1, highScoreDisplayable);


		glutSwapBuffers();
	}

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
	glutTimerFunc(5000, Obstacole, 0);
	glutTimerFunc(100, increaseScore, 0);

	glutSpecialFunc(keyPressed);
	glutSpecialUpFunc(keyUp);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();


}

