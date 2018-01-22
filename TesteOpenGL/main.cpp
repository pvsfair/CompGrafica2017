#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "Dependencies\freeglut\glut.h"
#include "FrameBuffer.h"
#include "DrawLib.h"
#include "Math.h"
#include "ColorPicker.h"
#include "main.h"

using namespace std;

void update(int a) {
	glutPostRedisplay();
}

void text()
{
	string menu = FrameBuffer::getInstance()->buttonSelected;
	menu += (FrameBuffer::getInstance()->fillFigure) ? " Com preenchimento" : "";
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

void drawColorPicker() {

	auto cp = ColorPicker::getInstance();
	Color c;
	for (int y = 0; y < 330; y++) {
		for (int x = 1000; x < 1300; x++) {
			//cout << x << ',' << y << endl;
			c = cp->getPixel(x - 1000, y);
			glColor3ub(c.r, c.g, c.b);
			glBegin(GL_POINTS);
			glVertex2i(x, y);
			glEnd();
		}
	}
	c = cp->getColor();
	glColor3ub(c.r, c.g, c.b);
	glBegin(GL_QUADS);
	glVertex2i(1100, 430);
	glVertex2i(1100, 530);
	glVertex2i(1200, 530);
	glVertex2i(1200, 430);
	glEnd();

	glLineWidth(1);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(999, 0);
	glVertex2i(999, 1000);
	glEnd();
}

void displayCB(void)		/* function called whenever redisplay needed */
{
	glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
	FrameBuffer *fb = FrameBuffer::getInstance();
	Color c;
	for (int i = 0; i < FrameBuffer::nLinhas; i++) {
		for (int j = 0; j < FrameBuffer::nCol; j++) {
			if ((c = fb->getPolisPixel(i, j)) != Color(-1, -1, -1)) {	
			}
			else if (fb->isTempBuffer() && (c = fb->getTempPixel(i, j)) != Color(-1, -1, -1)) {
				//c = fb->getTempPixel(i, j);
				//cout << "pintando com temp" << endl;
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
			/*glBegin(GL_POINTS);
			glVertex2i(i, j);
			glEnd();*/
		}
	}
	drawColorPicker();
	text();
	glFlush();				/* Complete any pending operations */
	glutTimerFunc(35, update, 0);
}

pair<int, int> click = pair<int, int>(-1, -1);

void keyCB(unsigned char key, int x, int y)	/* called on key press */
{
	FrameBuffer* fb = FrameBuffer::getInstance();
	if (key == 'q') exit(0);
	else if (key == '1') {
		fb->toolSelected = 1;
		click = pair<int, int>(-1, -1);
		fb->buttonSelected = "Ferramenta: Linha";
		fb->fillFigure = false;
	}
	else if (key == '2') {
		fb->toolSelected = 2;
		click = pair<int, int>(-1, -1);
		fb->buttonSelected = "Ferramenta: Círculo";
	}
	else if (key == '3') {
		fb->toolSelected = 3;
		click = pair<int, int>(-1, -1);
		fb->buttonSelected = "Ferramenta: Elipse";
	}
	else if (key == '4') {
		fb->toolSelected = 4;
		fb->buttonSelected = "Ferramenta: Baldinho";
	}
	else if (key == '5') {
		fb->toolSelected = 5;
		fb->buttonSelected = "Ferramenta: Pintar Ponto";
		fb->fillFigure = false;
	}
	else if (key == '6') {
		fb->toolSelected = 6;
		fb->buttonSelected = "Ferramenta: Poligono";
		DrawLib::clearPoligonoDrawing();
	}
	else if (key == '7') {
		fb->toolSelected = 7;
		fb->buttonSelected = "Ferramenta: Janela de recorte";
		fb->fillFigure = false;
		fb->clearFrameBuffer();
	}
	else if (key == 'f' || key == 'F') {
		if (fb->toolSelected != 7 && fb->toolSelected != 5 && fb->toolSelected != 1)
			fb->fillFigure = !fb->fillFigure;
	}
	else if (key == 'c' || key == 'C') {
		fb->clearFrameBuffer();
	}
	else if (key == 'w' || key == 'W')
	{
		Poligono3D* poli3d = (*fb->getPoligonos3D())[0];
		poli3d->rotacao(0, 2);
		fb->redrawPolisBuffer();
	}
	else if (key == 'e' || key == 'E')
	{
		Poligono3D* poli3d = (*fb->getPoligonos3D())[0];
		poli3d->rotacao(1, 2);
		fb->redrawPolisBuffer();
	}
	else if (key == 'r' || key == 'R')
	{
		Poligono3D* poli3d = (*fb->getPoligonos3D())[0];
		poli3d->rotacao(2, 2);
		fb->redrawPolisBuffer();
	}
	//glutPostRedisplay();
	//std::cout << key << endl;
}

//Called when the window is resized
void handleResize(int w, int h) {

}

FrameBuffer *temp = nullptr;
ColorPicker *cp = nullptr;

void mouseFunc(int button, int state, int x, int y) {//Called on mouseKeyUp or mouseKeyDown
	if (state == GLUT_UP)
		cp = nullptr;
	if (x > 1000) {
		if (FrameBuffer::getInstance()->toolSelected == 1 && temp != nullptr) {
			if (state == GLUT_UP) {
				temp = nullptr;
				click = pair<int, int>(-1, -1);
				FrameBuffer::getInstance()->copyTempToFinalBuffer();
			}
		}
		else {
			if (state == GLUT_DOWN) {
				cp = ColorPicker::getInstance();
				if (y >= 0 && y < 300) {
					cp->setHV((x - 1000)*1.2f, 1 - y / 300.0f);
				}
				else if (y >= 300 && y < 330) {
					cp->setS((x - 1000) / 300.0f);
				}
			}
		}
	}
	else {
		int pixelSize = FrameBuffer::getInstance()->pixelSize;
		int xScreen = x / pixelSize;
		int yScreen = y / pixelSize;
		switch (FrameBuffer::getInstance()->toolSelected)
		{
		case 1:// Linha
			if (state == GLUT_DOWN) {
				click = pair<int, int>(xScreen, yScreen);
				temp = FrameBuffer::getInstance();
			}
			else if (state == GLUT_UP) {
				temp = nullptr;
				click = pair<int, int>(-1, -1);
				FrameBuffer::getInstance()->copyTempToFinalBuffer();
			}
			break;
		case 2:// Círculo
			if (state == GLUT_DOWN) {
				click = pair<int, int>(xScreen, yScreen);
				temp = FrameBuffer::getInstance();
			}
			else if (state == GLUT_UP) {
				temp = nullptr;
				click = pair<int, int>(-1, -1);
				FrameBuffer::getInstance()->copyTempToFinalBuffer();
			}
			break;
		case 3:// Elipse
			if (state == GLUT_DOWN) {
				click = pair<int, int>(xScreen, yScreen);
				temp = FrameBuffer::getInstance();
			}
			else if (state == GLUT_UP) {
				temp = nullptr;
				click = pair<int, int>(-1, -1);
				FrameBuffer::getInstance()->copyTempToFinalBuffer();
			}
			break;
		case 7:// Polígono Recorte
			if (state == GLUT_DOWN) {
				click = pair<int, int>(xScreen, yScreen);
				temp = FrameBuffer::getInstance();
			}
			else if (state == GLUT_UP) {
				temp = nullptr;
				click = pair<int, int>(-1, -1);
				FrameBuffer::getInstance()->copyTempToFinalBuffer();
			}
			break;
		default:
			if (state == GLUT_UP) {
				//std::cout << xScreen << ',' << yScreen << endl;
				switch (FrameBuffer::getInstance()->toolSelected)
				{
				case 4:// Baldinho
					DrawLib::floodFill(xScreen, yScreen, ColorPicker::brushColor());
					break;
				case 5:// Pinta Pixel
					FrameBuffer::getInstance()->setPixel(xScreen, yScreen, Color(100, 15, 100));
					break;
				case 6:
					DrawLib::printPoligonoPonto(std::pair<int,int>(xScreen, yScreen), ColorPicker::getInstance()->brushColor(), FrameBuffer::getInstance()->fillFigure);
					break;
				default:
					break;
				}
			}
		}
	}
	//glutPostRedisplay();
}

void motionFunc(int x, int y) {
	int pixelSize = FrameBuffer::getInstance()->pixelSize;
	int xScreen = x / pixelSize;
	int yScreen = y / pixelSize;
	pair<int, int> ponto = pair<int, int>(xScreen, yScreen);

	switch (FrameBuffer::getInstance()->toolSelected) {
	case 1:// Linha
		if (click.first >= 0) {
			//std::cout << xScreen << ',' << yScreen << endl;
			DrawLib::printLinha(click, ponto, ColorPicker::brushColor(), true);

		}
		break;
	case 2:// Círculo
		if (click.first >= 0) {
			int raio = Math::distanceBtwPoints(click, pair<int, int>(xScreen, yScreen));
			cout << "printando cilculo: ";
			DrawLib::printCirculo(click, raio, ColorPicker::brushColor(), FrameBuffer::getInstance()->fillFigure, true);
			cout << endl;
		}
		break;
	case 3:// Elipse
		if (click.first >= 0) {
			int w = Math::distanceBtwPoints(pair<int, int>(click.first, yScreen), pair<int, int>(xScreen, yScreen));
			int h = Math::distanceBtwPoints(pair<int, int>(click.first, yScreen), click);
			DrawLib::printElipse(click.first, yScreen, w, h, ColorPicker::brushColor(), true);
		}
		break;
	case 7:
		if (click.first >= 0) {
			DrawLib::drawPoligonoRecorte(click, ponto);
		}
		break;
	}
	if (cp != nullptr) {
		cp = ColorPicker::getInstance();
		if (y >= 0 && y < 300) {
			cp->setHV((x - 1000)*1.2f, 1 - y / 300.0f);
		}
		else if (y >= 300 && y < 330) {
			cp->setS((x - 1000) / 300.0f);
		}
	}
	//glutPostRedisplay();
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
	/* teste polígono 2D
	std::vector<std::pair<int, int>> poli1;
	poli1.emplace_back(0, 50);
	poli1.emplace_back(50, 50);
	poli1.emplace_back(50, 0);
	poli1.emplace_back(0, 0);
	//DrawLib::printPoligono(poli1);
	Poligono pol(&poli1, Color(0,0,0));
	//pol.redrawPoli();
	pol.translacao(32, 32);
	FrameBuffer::getInstance()->addPoligono(&pol);
	pol.escala(1.2, 1.2);
	FrameBuffer::getInstance()->addPoligono(&pol);
	FrameBuffer::getInstance()->redrawPolisBuffer();
	//pol.redrawPoli();
	*/

	std::vector<ponto3d> vertices;
	//vertices.emplace_back(1, 1, 1);
	//vertices.emplace_back(2, 2, 1);
	//vertices.emplace_back(1, 2, 1);
	vertices.emplace_back(10, 10, 10);
	vertices.emplace_back(20, 10, 10);
	vertices.emplace_back(20, 20, 10);
	vertices.emplace_back(10, 20, 10);
	vertices.emplace_back(10, 10, 20);
	vertices.emplace_back(20, 10, 20);
	vertices.emplace_back(20, 20, 20);
	vertices.emplace_back(10, 20, 20);
	std::vector<face3d> faces;
	//faces.emplace_back(0, 2, 1);
	faces.emplace_back(0, 4, 5);
	faces.emplace_back(0, 5, 1);
	faces.emplace_back(1, 6, 5);
	faces.emplace_back(1, 2, 6);
	faces.emplace_back(2, 6, 7);
	faces.emplace_back(2, 7, 3);
	faces.emplace_back(3, 4, 7);
	faces.emplace_back(3, 0, 4);
	faces.emplace_back(4, 5, 6);
	faces.emplace_back(4, 6, 7);
	faces.emplace_back(0, 3, 2);
	faces.emplace_back(0, 2, 1);
	Poligono3D *poli3d = new Poligono3D(vertices, faces);
	//poli3d->translacao(-9,-9,-9);
	//poli3d->escala(3, 3, 3);
	//Poligono3D *poli3d2 = new Poligono3D(vertices, faces);
	//poli3d.redrawPoli(50);
	FrameBuffer::getInstance()->addPoligono3d(poli3d);
	//poli3d.rotacao(0, 90);
	//poli3d2->translacao(13, 0, 0);
	//FrameBuffer::getInstance()->addPoligono3d(poli3d2);
	FrameBuffer::getInstance()->redrawPolisBuffer();


	int win;

	glutInit(&argc, argv);		/* initialize GLUT system */

	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1300, 1000);		/* width=400pixels height=500pixels */

	win = glutCreateWindow("Triangle");	/* create window */

										/* from this point on the current window is win */

	glClearColor(1.0, 1.0, 1.0, 1.0);	/* set background to black */
	gluOrtho2D(0, 1300, 1000, 0);		/* how object is mapped to window */
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