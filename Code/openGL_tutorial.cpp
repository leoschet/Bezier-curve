/*
-----------------------------------------------------------------------------
OpenGL Tutorial
VOXAR Labs
Computer Science Center - CIn
Federal University of Pernambuco - UFPE
http://www.cin.ufpe.br/~voxarlabs

-----------------------------------------------------------------------------
*/

#include "openGL_tutorial.h"

/////////////////////////////////////////////////////////////////////////////
/*
http://www.inf.pucrs.br/~manssour/OpenGL/Tutorial.html
esse link!!!!
nãoo vou parar aquui pra mostrar mais coisas de C/C++ hj..
*/
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <vector>

class Ponto
{
public:
	float x, y;
	Ponto(float x, float y) : x(x), y(y) {};

};

// variables
std::vector <Ponto> pontos;
std::vector <Ponto> bezierCurve;
int dotIndex = -1;

const int WINDOW_W = 500;
const int WINDOW_H = 500;
const float _POINT_SIZE = 5.0f;

// auxiliar
bool insideDot(int clickedX, int clickedY, int dotX, int dotY)
{
	return dotX >= clickedX - _POINT_SIZE && dotX <= clickedX + _POINT_SIZE && dotY >= clickedY - _POINT_SIZE && dotY <= clickedY + _POINT_SIZE;
}

int getDot(int x, int y)
{
	int dotX;
	int dotY;
	int aux = 0;
	int maxSize = pontos.size();

	while (aux < maxSize)
	{
		dotX = pontos[aux].x;
		dotY = pontos[aux].y;

		if (insideDot(x, y, dotX, dotY))
			return aux;

		aux += 1;
	}

	return -1;
}

// bezier
unsigned long long fact(int number)
{
	// 1 e o numero 'irrelevante' da multiplicacao 
	unsigned long long result = 1;

	for (int i = number; i > 1; i -= 1){
		result *= i;
	}

	return result;
}

unsigned long long optimizedFact(int number, int limit)
{
	// 1 e o numero 'irrelevante' da multiplicacao 
	unsigned long long result = 1;

	for (int i = number; i > limit; i -= 1){
		result *= i;
	}

	return result;
}

int getMax(int numberOne, int numberTwo)
{
	return numberOne > numberTwo ? numberOne : numberTwo;
}

int getMin(int numberOne, int numberTwo)
{
	return numberOne < numberTwo ? numberOne : numberTwo;
}

unsigned long long comb(int n, int i)
{
	int limit = getMax(i, n - i);
	int minimum = getMin(i, n - i);

	unsigned long long combResult = optimizedFact(n, limit) / (fact(minimum));

	return combResult;
}

void bezier()
{
	bezierCurve.clear();

	for (float t = 0; t <= 1; t += 0.01)
	{
		float x = 0;
		float y = 0;
		int maxSize = pontos.size() - 1;

		for (int i = 0; i < pontos.size(); i += 1)
		{
			x += comb(maxSize, i) * pow((1 - t), (maxSize - i)) * pow(t, i) * pontos[i].x;
			y += comb(maxSize, i) * pow((1 - t), (maxSize - i)) * pow(t, i) * pontos[i].y;
		}

		bezierCurve.push_back(Ponto(x, y));
	}
}
// bezier end

// display event
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (pontos.size() > 0)
	{
		glPointSize(_POINT_SIZE);
		glBegin(GL_POINTS);
		float colorCoef = 1.0f;

		for (int i = 0; i < pontos.size(); i++)
		{
			if (i < 20)
				glColor3f(0.0f, 1.0f, 0.0f);
			else if (i < 29)
				glColor3f(1.0f, 1.0f, 0.0f);
			else
				glColor3f(1.0f, 0.0f, 0.0f);

			glVertex2d(pontos.at(i).x, pontos.at(i).y);

			colorCoef -= 0.1f;
		}

		glEnd();

		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < pontos.size(); i++)
			glVertex2d(pontos.at(i).x, pontos.at(i).y);
		glEnd();
	}

	if (bezierCurve.size() > 2)
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0.5f, 0.0f, 1.0f);
		for (int i = 0; i < bezierCurve.size(); i++)
			glVertex2d(bezierCurve.at(i).x, bezierCurve.at(i).y);
		glEnd();
	}

	glFlush();
}

// reshape window event
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, WINDOW_W, WINDOW_H, 0.0f, -5.0, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// key press event
void handleKeypress(unsigned char key, int x, int y)
{
	static int x1 = 0;
	switch (key){
	case 27: // ESC
		exit(0);
		break;
	case 119:
		glLineWidth(5.0f);
		pontos.push_back(Ponto(x, y));
		glutPostRedisplay();
		x1++;
		break;
	case 99:
		pontos.clear();
		bezierCurve.clear();
		glutPostRedisplay();
	}

}

// mouse pressed and moving event
void handlePressedAndMovingMouse(int x, int y)
{
	if (dotIndex != -1)
	{
		pontos.at(dotIndex).x = x;
		pontos.at(dotIndex).y = y;
		glutPostRedisplay();

		bezier();
	}
}

// mouse click event
void handleMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
		{
			dotIndex = getDot(x, y);
		}
		else
		{
			dotIndex = -1;
			bezier();
		}
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
		{
			// se 'getDot' for diferente de -1, ja existe um ponto naquele local
			if (getDot(x, y) == -1)
				pontos.push_back(Ponto(x, y));

			bezier();
		}
		if (button == GLUT_MIDDLE_BUTTON)
			if (state == GLUT_DOWN)
			{
				int index = getDot(x, y);

				if (index != -1)
					pontos.erase(pontos.begin() + index);

				bezier();
			}

	glutPostRedisplay();  // avisa que a janela atual deve ser reimpressa
}

// main
int main(int argc, char ** argv)
{
	printf("Aperte 'c' para limpar a tela.\n");
	printf("O limite do programa sao 29 ou 30 pontos.\n");
	printf("Os pontos ficarao vermelhos quando existirem 29 pontos na tela.");

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0); // a janela irá iniciar to topo esquerdo
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutCreateWindow("Exemplo OpenGL");
	glMatrixMode(GL_MODELVIEW); // estou alterando a matrix de do modelo da cena
	glLoadIdentity();

	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(handleMouseClick);
	glutMotionFunc(handlePressedAndMovingMouse);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}