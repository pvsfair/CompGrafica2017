#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "Dependencies\freeglut\glut.h"
#include "FrameBuffer.h"
#include "DrawLib.h"

using namespace std;

void displayCB(void)		/* function called whenever redisplay needed */
{
	glClear(GL_COLOR_BUFFER_BIT);		/* clear the display */
	FrameBuffer *fb = FrameBuffer::getInstance();
	for (int i = 0; i < FrameBuffer::nLinhas; i++) {
		for (int j = 0; j < FrameBuffer::nCol; j++) {
			Color c = fb->getPixel(i, j);
			glColor3ub(c.r, c.g, c.b);

			glBegin(GL_QUADS);
			glVertex2i(i*fb->pixelSize, j*fb->pixelSize);
			glVertex2i(i*fb->pixelSize, j*fb->pixelSize + fb->pixelSize);
			glVertex2i(i*fb->pixelSize + fb->pixelSize, j*fb->pixelSize + fb->pixelSize);
			glVertex2i(i*fb->pixelSize + fb->pixelSize, j*fb->pixelSize);
			glEnd();
		}
	}
	glFlush();				/* Complete any pending operations */
}

void keyCB(unsigned char key, int x, int y)	/* called on key press */
{
	if (key == 'q') exit(0);
}

//Called when the window is resized
void handleResize(int w, int h) {

}
int main(int argc, char *argv[])
{
	FrameBuffer *fb = FrameBuffer::getInstance();
	fb->setPixel(3, 4, Color(255, 10, 40));
	DrawLib::printCirculo(50, 50, 47);
	DrawLib::floodFill(50, 50, Color(255, 255, 0));
	DrawLib::printCirculo(30, 30, 12);
	DrawLib::printCirculo(70, 30, 12);
	DrawLib::printCirculo(50, 50, 5);

	DrawLib::printElipse(50, 75, 28, 7);

	DrawLib::printLinha(23, 75, 77, 75);
	DrawLib::printLinha(50, 81, 50, 69);
	DrawLib::printLinha(42, 81, 42, 69);
	DrawLib::printLinha(58, 81, 58, 69);
	DrawLib::printLinha(34, 80, 34, 70);
	DrawLib::printLinha(66, 80, 66, 70);
	DrawLib::printLinha(26, 78, 26, 72);
	DrawLib::printLinha(74, 78, 74, 72);
	/*
	DrawLib::printLinha(0, 10, 31, 12);

	std::vector<std::pair<int, int>> poli1;
	poli1.emplace_back(0, 50);
	poli1.emplace_back(20, 90);
	poli1.emplace_back(50, 30);
	poli1.emplace_back(80, 90);
	poli1.emplace_back(100, 50);
	poli1.emplace_back(80, 10);
	poli1.emplace_back(50, 70);
	poli1.emplace_back(20, 10);
	DrawLib::printPoligono(poli1, true);
	*/

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