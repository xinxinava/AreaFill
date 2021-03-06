#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <gl/glut.h>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

class Point
{
public:
	int x;
	int y;
	Point() {}
	Point(int xa, int ya) :x(xa), y(ya) {}
};

class AE
{
public:
	double x;
	double ymax;
	double dx;
	AE() {}
	AE(double xa, double ya, double k) :x(xa), ymax(ya), dx(k) {}
};

vector<Point> V;

void p2p(Point a, Point b)
{			
	glBegin(GL_LINES);
	glVertex2i(a.x, a.y);
	glVertex2i(b.x, b.y);
	glEnd();
	glFlush();
}


double distance(Point &a, Point &b)
{
	cout << "dis = " << sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)) << endl;
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void insert(vector<AE> &v, Point &a, Point &b)
{
	Point p1, p2;
	if (a.y >= b.y) p1 = b, p2 = a;
	else p1 = a, p2 = b;
	AE x = AE(p1.x, p2.y, double(p1.x - p2.x) / double(p1.y - p2.y));
	v.push_back(x);
}

bool cmp(AE &a, AE &b)
{
	if (a.x != b.x)
		return a.x < b.x;
	return a.dx < b.dx;
}

void mouseClick(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Point pp(x, y);
		if (!V.empty() && distance(V.front(), pp) <= 20)
		{
			p2p(V.back(), V.front());
			
			map<int, vector<AE> > ET;
			int y_max = V.front().y, y_min = V.front().y;

			insert(ET[min(V.front().y, V.back().y)], V.front(), V.back());
			for (vector<Point>::iterator i = V.begin() + 1; i != V.end(); i++)
			{
				y_max = max(y_max, i->y);
				y_min = min(y_min, i->y);

				insert(ET[min((i - 1)->y, i->y)], *i, *(i - 1));
			}

			cout << "***************ET*****************start" << endl;

			for (int i = y_min; i < y_max; i++)
			{
				if (ET[i].empty()) continue;
				cout << "y = " << i;
				sort(ET[i].begin(), ET[i].end(), cmp);
				for (vector<AE>::iterator j = ET[i].begin(); j != ET[i].end(); j++)
					cout << "->[" << j->x << "," << j->ymax << "," << j->dx << "]";
				cout << endl;
			}
			cout << "****************ET******************end" << endl;

			vector<AE> AET;
			for (int i = y_min; i < y_max; i++)
			{
				bool draw = false;
				for (vector<AE>::iterator r = AET.begin(); r != AET.end(); r++)
				{
					if (draw)
						p2p(Point(round(r->x), i), Point(round((r - 1)->x), i));
					draw = !draw;
				}

				for (vector<AE>::iterator r = AET.begin(); r != AET.end();)
				{
					r->x += r->dx;
					if (r->ymax == i)
						r = AET.erase(r);
					else
						r++;
				}

				if (!ET[i].empty())
				{
					cout << "ET[" << i << "]";
					for (vector<AE>::iterator r = ET[i].begin(); r != ET[i].end(); r++)
						cout << "->(" << r->x << "," << r->ymax << "," << r->dx << ")";
					cout << endl;

					for (vector<AE>::iterator r = ET[i].begin(); r != ET[i].end(); r++)
					{
						cout << "r={" << r->x << "," << r->ymax << "," << r->dx << "}" << endl;
						AET.push_back(*r);
					}
				}

				sort(AET.begin(), AET.end(), cmp);

				cout << "*******************AET***************start" << endl;
				for (vector<AE>::iterator r = AET.begin(); r != AET.end(); r++)
					cout << "[" << r->x << "," << r->ymax << "," << r->dx << "]";
				cout << endl;
				cout << "*******************AET**************end" << endl;
			}
			V.clear();
			return;
		}
		if(!V.empty())
			p2p(V.back(), pp);
		V.push_back(pp);
	}
 }

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
}

void KeyBoards(unsigned char key, int x, int y)
{
	cout << key << endl;
	switch (key)
	{
		case 'c':
			cout << "clear" << endl;
			glClear(GL_COLOR_BUFFER_BIT);
			glColor3f(0.0, 0.0, 0.0);
			glFlush();
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(50, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("AreaFill");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	gluOrtho2D(0, 500, 500, 0);
	glLineWidth(2);
	glutDisplayFunc(&display);
	glutMouseFunc(mouseClick);
	glutKeyboardFunc(&KeyBoards);
	glutMainLoop();
	return 0; 
}
