#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>
#include <random>
#include <string> 
#include "SOIL.h"

using namespace std;

const double TWO_PI = 6.2831853;
double i = -50;
// unde se va opri masina pe axa x, cand face schimbarea de benzi
double j = 1.0;
double k = 0;
double iViteza = 260;
GLsizei winWidth = 850, winHeight = 750;
GLuint traficID;
static GLfloat rotTheta = 0.0;
GLboolean leftUpPressed = false;
GLboolean rightUpPressed = false;
int obstacol1, obstacol2, obstacol3;
bool initObstacle1, initObstacle2, initObstacle3;
double iObstacol1, iObstacol2, iObstacol3;
bool collisionCheck = false;
double score = 0;
double speed = 0;
float genR, genG, genB;
float genR1, genG1, genB1;
float genR2, genG2, genB2;
float genR3, genG3, genB3;
int chance = 50;
double xCoin = 0;
double yCoin = 320;
double scaleCoin = 0.9;
int switcher = 1;
bool displayCoin = false;
GLuint textureWater, textureCrash, textureLilypad;
double yLilypad;

// Functie pentru incrementarea scorului 
void increaseScore(int value) {
	if (collisionCheck != true) {
		score += 1;
	}
	glutTimerFunc(100, increaseScore, 0);
}

// Viteza jocului creste cu timpul
void increaseSpeed(int value) {
	if (collisionCheck != true) {
		speed += 1;
	}
	glutTimerFunc(100, increaseSpeed, 0);
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

// Functie pentru adaugarea textului mai usor
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

// Functie legata de coin, putem ajusta cat de des apare prin variabila chance
void generateLucky() {
	int isLucky;
	xCoin = 0;
	yCoin = 320;
	displayCoin = false;
	isLucky = rand() % chance;
	if (isLucky == 0) {
		cout << "Lucky one!" << endl;
		chance = 50;
		displayCoin = true;
	}
	else {
		cout << "You're not lucky! chance: 1/" << chance << endl;
		chance = chance / 2;
	}
}

// Functie care genereaza culori random pentru masinile obstacole
void generateColor(void) {

	int pickPrimary = rand() % 6;
	if (pickPrimary == 0) {
		genR = 1;
		genG = (float)rand() / (float)RAND_MAX;
		genB = 0;
	}
	else if (pickPrimary == 1) {
		genR = (float)rand() / (float)RAND_MAX;
		genG = 1;
		genB = 0;
	}
	else if (pickPrimary == 2) {
		genR = 0;
		genG = 1;
		genB = (float)rand() / (float)RAND_MAX;
	}
	else if (pickPrimary == 3) {
		genR = 0;
		genG = (float)rand() / (float)RAND_MAX;
		genB = 1;
	}
	else if (pickPrimary == 4) {
		genR = (float)rand() / (float)RAND_MAX;
		genG = 0;
		genB = 1;
	}
	else if (pickPrimary == 5) {
		genR = 1;
		genG = 0;
		genB = (float)rand() / (float)RAND_MAX;
	}
}

// Generatorul de masini obstacole, acestea sunt generate random
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
		generateLucky();

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
			generateColor();
			genR1 = genR;
			genG1 = genG;
			genB1 = genB;
		}

		if (obstacol2 == 1) {
			initObstacle2 = true;
			generateColor();
			genR2 = genR;
			genG2 = genG;
			genB2 = genB;
		}

		if (obstacol3 == 1) {
			initObstacle3 = true;
			generateColor();
			genR3 = genR;
			genG3 = genG;
			genB3 = genB;
		}
	}

	// Timer pentru loop-ul obstacolelor
	glutTimerFunc(4000, Obstacole, 0);

}

// Functia care detecteaza coliziuni, foloseste formula AABB
bool Collision(double x1, double y1, double x2, double y2, double xWidth, double xHeight, double yWidth, double yHeight) {
	if (x1 < x2 + yWidth && x1 + xWidth > x2 && y1 < y2 + yHeight && y1 + xHeight > y2)
	{
		//collisionCheck = true;
		return 1;
	}
	return 0;
}

// Un generator de masini pentru obstacole
void buildCar(int x, int y, float R, float G, float B) {
	//Model Initial
	//glRectf(-160, 270, -100, 360);
	if (initObstacle1 == 0 && initObstacle2 == 0 && initObstacle3 == 0) {
		generateColor();
	}

	//Sasiu
	glColor3f(R, G, B);
	glRectf(x, y, x + 60, y + 25);
	glColor3f(R - 0.3, G - 0.3, B - 0.3);
	glRectf(x, y + 25, x + 60, y + 65);
	glColor3f(R, G, B);
	glRectf(x, y + 65, x + 60, y + 90);
	// stopuri
	glColor3f(1, 0, 0);
	glRectf(x + 5, y - 5, x + 15, y);
	glColor3f(1, 0, 0);
	glRectf(x + 45, y - 5, x + 55, y);
	// faruri fata
	glColor3f(1, 1, 0);
	glRectf(x + 5, y + 90, x + 15, y + 95);
	glColor3f(1, 1, 0);
	glRectf(x + 45, y + 90, x + 55, y + 95);
}

// Generatorul de coin-uri
void drawCoin() {
	glPushMatrix();
	glScalef(40, 40, 0);
	glColor3f(0.855, 0.647, 0.125);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);
	for (int i = 0; i <= 360; i += 5) {
		float radian = i * (3.14159 / 180.0);
		float x = 0.5 * cos(radian);
		float y = 0.5 * sin(radian);
		glVertex2f(x, y);
	}
	glEnd();

	glColor3f(1.0, 0.937, 0.537);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);
	for (int i = 0; i <= 360; i += 5) {
		float radian = i * (3.14159 / 180.0);
		float x = 0.4 * cos(radian);
		float y = 0.4 * sin(radian);
		glVertex2f(x, y);
	}
	glEnd();

	//Steluta din mijloc
	glLineWidth(2.0);
	glColor3f(0.855, 0.647, 0.125);
	glBegin(GL_TRIANGLES);
	float radius = 0.35;
	for (int i = 0; i < 5; i++) {
		float angle1 = i * 72 * (3.14159 / 180.0);
		float angle2 = (i + 2) % 5 * 72 * (3.14159 / 180.0);
		float x1 = radius * cos(angle1);
		float y1 = radius * sin(angle1);
		float x2 = 0.5 * radius * cos(angle2);
		float y2 = 0.5 * radius * sin(angle2);

		glVertex2f(x1, y1);
		glVertex2f(0, 0);
		glVertex2f(x2, y2);
	}
	glEnd();
	glLineWidth(1.0);
	glPopMatrix();
}

// Functia care se ocupa de incrementarea valorilor si de toate miscarile din joc + coliziuni
void events(void) {
	// Pentru girofar
	j += 0.05;
	if (j > 360) {
		j = 0;
	}
	if (j < 0) {
		j = 360;
	}

	// Pentru input-ul left key - right key
	if (leftUpPressed && i > -190) {
		i -= 1.3 + speed / 1600;
	}

	if (rightUpPressed && i < 90) {
		i += 1.3 + speed / 1600;
	}

	// Indice pentru axul drumului, indica viteza scenei
	iViteza += 2 + speed / 400;
	if (iViteza > 519) {
		iViteza = 260;
	}

	yLilypad -= 1;
	if (yLilypad < -300) {
		yLilypad = 1000;
	}

	// Scale pentru coin, mai usor de vazut
	scaleCoin += 0.005 * switcher;
	if (scaleCoin > 1.1 || scaleCoin < 0.9) {
		switcher = switcher * -1;
	}

	// Indice pentru translatia obstacolelor
	if (initObstacle1 == 1 && iObstacol1 > -650) {
		iObstacol1 -= 1 + speed / 400;
	}

	if (initObstacle2 == 1 && iObstacol2 > -650) {
		iObstacol2 -= 1 + speed / 400;
	}

	if (initObstacle3 == 1 && iObstacol3 > -650) {
		iObstacol3 -= 1 + speed / 400;
	}

	if (initObstacle1 == 0 && displayCoin == true) {
		xCoin = -150;
		yCoin -= 1 + speed / 400;
	}
	else if (initObstacle2 == 0 && displayCoin == true) {
		xCoin = 0;
		yCoin -= 1 + speed / 400;
	}
	else if (initObstacle3 == 0 && displayCoin == true) {
		xCoin = 135;
		yCoin -= 1 + speed / 400;
	}

	// Collision check for obstacles
	if (Collision(-160, iObstacol1 + 360, i + 20, -90, 60, 90, 60, 90) == 1) {
		collisionCheck = true;
	}

	if (Collision(-35, iObstacol2 + 360, i + 20, -90, 60, 90, 60, 90) == 1) {
		collisionCheck = true;
	}

	if (Collision(95, iObstacol3 + 360, i + 20, -90, 60, 90, 60, 90) == 1) {
		collisionCheck = true;
	}

	// Collision check for coins
	if (Collision(xCoin - 15, yCoin + 65, i + 20, -90, 40, 40, 60, 90) == 1) {
		cout << "Touched coin!";
		xCoin = 0;
		yCoin = -300;
		score += 100;
		// Sunet optional pentru coin
		//sndPlaySound(TEXT("coin.wav"), SND_ASYNC);
	}

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

	// Pentru opacitatea culorilor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Muzica fundal
	PlaySound(TEXT("nightrider.wav"), NULL, SND_ASYNC | SND_LOOP);

	// Initializarea texturii de apa
	textureWater = SOIL_load_OGL_texture("water4.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MIPMAPS);
	textureCrash = SOIL_load_OGL_texture("crash.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
	textureLilypad = SOIL_load_OGL_texture("lily.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);

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
	case GLUT_KEY_UP:
		if (collisionCheck == true) {
			glClearColor(0, 0.839, 0.082, 1);
			collisionCheck = false;
			iObstacol1 = 600;
			iObstacol2 = 600;
			iObstacol3 = 600;
			yCoin = -600;
			score = 0;
			speed = 0;
			break;
		}
	default:
		break;
	}
	glutPostRedisplay();
}

// Functia care se ocupa cu desenarea scenei
void desenDrum(void)
{
	if (collisionCheck != true)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Textura apa
		glPushMatrix();
		glTranslatef(0, -iViteza/2, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureWater);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);			glVertex3f(-1000, -1000, 0);
		glTexCoord2f(10, 0);			glVertex3f(1000, -1000, 0);
		glTexCoord2f(10, 10);			glVertex3f(1000, 1000, 0);
		glTexCoord2f(0, 10);			glVertex3f(-1000, 1000, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Fundal fara textura
		//glColor3f(0, 0, 0.2);
		//glRectf(-1000., -1000., 1000, 1000.);

		// DRUMUL + umbra pod
		glColor3f(0.2, 0.2, 0.2);
		glRectf(-200., -1000., 200., 1000.);

		glColor3f(0.9, 0.9, 0.9);
		glRectf(-195., -1000, -205, 1000);

		glColor3f(0.9, 0.9, 0.9);
		glRectf(195., -1000, 205, 1000);

		glColor4f(0, 0, 0, 0.6);
		glRectf(205, -1000, 270, 1000);

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

		glColor4f(1, 1, 0, 0.05);
		glBegin(GL_TRIANGLES);
		glVertex2f(30, -85);
		glVertex2f(0, 0);
		glVertex2f(60, 0);

		glVertex2f(40, 0);
		glVertex2f(100, 0);
		glVertex2f(70, -85);

		glEnd();

		// GIROFAR
		glTranslated(50, -135, 0);
		glRotated(j, 0, 0, 1);
		glTranslated(-50, 135, 0);

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

		// Prima banda (stanga -> dreapta)
		glPushMatrix();
		glTranslated(0, iObstacol1, 0);
		//glColor3f(0, 0, 0);
		//glRectf(-160, 270, -100, 360);
		buildCar(-160, 270, genR1, genG1, genB1);

		glPopMatrix();

		// A doua banda
		glPushMatrix();
		glTranslated(0, iObstacol2, 0);
		//glColor3f(0, 0, 0);
		//glRectf(-35, 270, 25, 360);
		buildCar(-35, 270, genR2, genG2, genB2);

		glPopMatrix();

		// A treia banda
		glPushMatrix();
		glTranslated(0, iObstacol3, 0);
		//glColor3f(0, 0, 0);
		//glRectf(95, 270, 155, 360);

		buildCar(95, 270, genR3, genG3, genB3);
		glPopMatrix();

		// Coin, translatie + scale

		glPushMatrix();
		glTranslated(xCoin, yCoin, 0);
		glScalef(scaleCoin, scaleCoin, 0);
		drawCoin();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-260, yLilypad, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureLilypad);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);			glVertex3f(-10, -10, 0);
		glTexCoord2f(1, 1);			glVertex3f(10, -10, 0);
		glTexCoord2f(1, 0);			glVertex3f(10, 10, 0);
		glTexCoord2f(0, 0);			glVertex3f(-10, 10, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Layer transparent overlay blue
		glColor4f(0.012, 0.0, 0.529, 0.25);
		glRectf(-1000., -1000., 1000, 1000.);

		// incrementare pentru girofar si alte translatii
		glutIdleFunc(events);

		// Textul cu scor, stanga sus
		displayText(-190, 220, 1, 1, 1, "Score: ");
		string scoreString = to_string(int(score));
		const char* scoreDisplayable = scoreString.c_str();
		displayText(-145, 219, 1, 1, 1, scoreDisplayable);

		glutSwapBuffers();
		glFlush();

	}

	// Daca este detectata o coliziune, scena este inlocuita cu una neagra + high score
	// Si majoritatea functiilor sunt puse pe pauza, pana la restart
	else {
		int highScore = score;
		string highScoreString = to_string(highScore);
		const char* highScoreDisplayable = highScoreString.c_str();
		glClear(GL_COLOR_BUFFER_BIT);
		glutIdleFunc(NULL);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureCrash);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);			glVertex3f(-300, -250, 0);
		glTexCoord2f(1, 1);			glVertex3f(300, -250, 0);
		glTexCoord2f(1, 0);			glVertex3f(300, 250, 0);
		glTexCoord2f(0, 0);			glVertex3f(-300, 250, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		iObstacol1 = 0;
		iObstacol2 = 0;
		iObstacol3 = 0;

		// Schimbam environment-ul texturii pentru a nu intra in conflict cu culorile font-ului generat mai jos
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		displayText(-60, 70, 0, 0, 0, "High Score: ");
		displayText(-75, 40, 0, 0, 0, "Press UP to restart ");
		displayText(23, 69, 0, 0, 0, highScoreDisplayable);
		glutSwapBuffers();
		glFlush();
	}
}



void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-300.0, 300.0, -250.0, 250.0);

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
	glutCreateWindow("Highway Racer");

	init();

	glutDisplayFunc(desenDrum);
	glutReshapeFunc(winReshapeFcn);

	// Loop-uri initiale pentru functii care sunt apelate o data la un timp fix
	glutTimerFunc(5000, Obstacole, 0);
	glutTimerFunc(100, increaseScore, 0);
	glutTimerFunc(100, increaseSpeed, 0);

	glutSpecialFunc(keyPressed);
	glutSpecialUpFunc(keyUp);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();

}