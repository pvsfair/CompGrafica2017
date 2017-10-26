#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "Dependencies\freeglut\glut.h"
#include "FrameBuffer.h"
#include "DrawLib.h"
#include "Math.h"

using namespace std;

void text()
{
	string menu = FrameBuffer::getInstance()->buttonSelected;
	int len;
	len = menu.size();

	glColor3f(0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0, 600, 0, 600);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glRasterPos2i(0, 590);


	for (int i = 0; i < len; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, menu[i]);
	}

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void displayCB(void)		/* function called whenever redisplay needed */
{
	glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
	FrameBuffer *fb = FrameBuffer::getInstance();
	Color c;
	for (int i = 0; i < FrameBuffer::nLinhas; i++) {
		for (int j = 0; j < FrameBuffer::nCol; j++) {
			if (fb->isTempBuffer() && fb->getTempPixel(i, j) != Color(-1, -1, -1)) {
				c = fb->getTempPixel(i, j);
				cout << "pintando com temp" << endl;
			}
			else {
				c = fb->getPixel(i, j);
			}
			glColor3ub(c.r, c.g, c.b);

			glBegin(GL_QUADS);
			glVertex2i(i*fb->pixelSize, j*fb->pixelSize);
			glVertex2i(i*fb->pixelSize, j*fb->pixelSize + fb->pixelSize);
			glVertex2i(i*fb->pixelSize + fb->pixelSize, j*fb->pixelSize + fb->pixelSize);
			glVertex2i(i*fb->pixelSize + fb->pixelSize, j*fb->pixelSize);
			glEnd();
		}
	}
	text();
	glFlush();				/* Complete any pending operations */
}

pair<int, int> click = pair<int, int>(-1, -1);

void keyCB(unsigned char key, int x, int y)	/* called on key press */
{
	if (key == 'q') exit(0);
	else if (key == '1') {
		FrameBuffer::getInstance()->toolSelected = 1;
		click = pair<int, int>(-1, -1);
		FrameBuffer::getInstance()->buttonSelected = "Ferramenta: Linha";
	}
	else if (key == '2') {
		FrameBuffer::getInstance()->toolSelected = 2;
		click = pair<int, int>(-1, -1);
		FrameBuffer::getInstance()->buttonSelected = "Ferramenta: Círculo";
	}
	else if (key == '3') {
		FrameBuffer::getInstance()->toolSelected = 3;
		click = pair<int, int>(-1, -1);
		FrameBuffer::getInstance()->buttonSelected = "Ferramenta: Elipse";
	}
	else if (key == '4') {
		FrameBuffer::getInstance()->toolSelected = 4;
		FrameBuffer::getInstance()->buttonSelected = "Ferramenta: Baldinho";
	}
	else if (key == '5') {
		FrameBuffer::getInstance()->toolSelected = 5;
		FrameBuffer::getInstance()->buttonSelected = "Ferramenta: Pintar Ponto";
	}
	glutPostRedisplay();
	//std::cout << key << endl;
}

//Called when the window is resized
void handleResize(int w, int h) {

}

FrameBuffer *temp = nullptr;

void mouseFunc(int button, int state, int x, int y) {//Called on mouseKeyUp or mouseKeyDown
	if (state == GLUT_DOWN) {
		int pixelSize = FrameBuffer::getInstance()->pixelSize;
		int xScreen = x / pixelSize;
		int yScreen = y / pixelSize;
		click = pair<int, int>(xScreen, yScreen);
		temp = FrameBuffer::getInstance();
	}
	else if (state == GLUT_UP) {
		temp = nullptr;
		click = pair<int, int>(-1, -1);
		FrameBuffer::getInstance()->copyTempToFinalBuffer();
	}
	/*if (state == GLUT_UP) {
		int pixelSize = FrameBuffer::getInstance()->pixelSize;
		int xScreen = x / pixelSize;
		int yScreen = y / pixelSize;
		std::cout << xScreen << ',' << yScreen << endl;
		switch (FrameBuffer::getInstance()->toolSelected)
		{
		case 1:
			if (click.first < 0) {
				click = pair<int, int>(xScreen, yScreen);
			}
			else {
				DrawLib::printLinha(click, pair<int, int>(xScreen, yScreen), Color(0, 0, 0));
				click = pair<int, int>(-1, -1);
			}
			break;
		case 2:
			if (click.first < 0) {
				click = pair<int, int>(xScreen, yScreen);
			}
			else {
				int raio = Math::distanceBtwPoints(click, pair<int, int>(xScreen, yScreen));
				DrawLib::printCirculo(click, raio);
				click = pair<int, int>(-1, -1);
			}
			break;
		case 3:
			if (click.first < 0) {
				click = pair<int, int>(xScreen, yScreen);
			}
			else {
				int w = Math::distanceBtwPoints(pair<int, int>(click.first, yScreen), pair<int, int>(xScreen, yScreen));
				int h = Math::distanceBtwPoints(pair<int, int>(click.first, yScreen), click);
				DrawLib::printElipse(click.first, yScreen, w, h);
				click = pair<int, int>(-1, -1);
			}
			break;
		case 4:
			DrawLib::floodFill(xScreen, yScreen, Color(0, 0, 255));
			break;
		case 5:
			FrameBuffer::getInstance()->setPixel(xScreen, yScreen, Color(100, 15, 100));
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}*/
}

void motionFunc(int x, int y) {
	int pixelSize = FrameBuffer::getInstance()->pixelSize;
	int xScreen = x / pixelSize;
	int yScreen = y / pixelSize;
	pair<int, int> ponto = pair<int, int>(xScreen, yScreen);
	if (click.first >= 0) {
		std::cout << xScreen << ',' << yScreen << endl;	

		DrawLib::printLinha(click, ponto, Color(23, 42, 148), true);

		glutPostRedisplay();
	}
}

int main(int argc, char *argv[])
{
	FrameBuffer *fb = FrameBuffer::getInstance();
	/*
	fb->setPixel(3, 4, Color(255, 10, 40));
	DrawLib::printCirculo(50, 50, 47);
	DrawLib::printCirculo(30, 30, 12);
	DrawLib::printCirculo(70, 30, 12);
	DrawLib::floodFill(50, 50, Color(255, 255, 0));
	DrawLib::printCirculo(50, 50, 5);
	DrawLib::floodFill(50, 50, Color(255, 0, 0));

	DrawLib::printElipse(50, 75, 28, 7);
	DrawLib::floodFill(50, 75, Color(150, 150, 150));
	DrawLib::printLinha(23, 75, 77, 75);
	DrawLib::printLinha(50, 81, 50, 69);
	DrawLib::printLinha(42, 81, 42, 69);
	DrawLib::printLinha(58, 81, 58, 69);
	DrawLib::printLinha(34, 80, 34, 70);
	DrawLib::printLinha(66, 80, 66, 70);
	DrawLib::printLinha(26, 78, 26, 72);
	DrawLib::printLinha(74, 78, 74, 72);
	*/
	/*

	*/
	std::vector<std::pair<int, int>> poli1;
	/*poli1.emplace_back(0, 50);
	poli1.emplace_back(20, 90);
	poli1.emplace_back(50, 30);
	poli1.emplace_back(80, 90);
	poli1.emplace_back(100, 50);
	poli1.emplace_back(80, 10);
	poli1.emplace_back(50, 70);
	poli1.emplace_back(20, 10);*/
	poli1.emplace_back(20, 20);
	poli1.emplace_back(20, 50);
	poli1.emplace_back(50, 50);
	poli1.emplace_back(50, 20);
	DrawLib::printPoligono(poli1, true);

	int win;

	glutInit(&argc, argv);		/* initialize GLUT system */

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(1000, 1000);		/* width=400pixels height=500pixels */
	win = glutCreateWindow("Triangle");	/* create window */

										/* from this point on the current window is win */

	glClearColor(1.0, 1.0, 1.0, 1.0);	/* set background to black */
	gluOrtho2D(0, 1000, 1000, 0);		/* how object is mapped to window */
	glutDisplayFunc(displayCB);		/* set window's display callback */
	glutKeyboardFunc(keyCB);		/* set window's key callback */
	
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	glutReshapeFunc(handleResize);

	glutMainLoop();			/* start processing events... */

							/* execution never reaches this point */

	return 0;
}
/*
int main(int argc, char **argv) {
	std::cout << "dsdsd";
	getchar();
	return 0;
}*/