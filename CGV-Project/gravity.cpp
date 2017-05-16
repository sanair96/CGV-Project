#include<stdlib.h>
#include<vector>
#include<gl\glut.h>
#include<math.h>
#define PI 22/7
using namespace std;
struct particle {
	float x;
	float y;
	float r;
	float vx;
	float vy;
	float m;
	float color[3];
};
struct line {
	float x1;
	float y1;
	float x2;
	float y2;
} line;

vector<particle> particles;
int mx ,my;
int left_press = 0, right_press = 0, middle_press = 0, shoot = 0;



void repeat(int);
void display();
void mouse(int, int, int, int);
void key(unsigned char, int, int);
void move(int, int);
void addPart(float,float,int,float,float);


void repeat(int) {
	display();
	if ((left_press == 1) && shoot == 0) {
		addPart(10,3,1,0,0);
		left_press = 0;
	}
	if (right_press == 1) {
		addPart(10000, 15, 2, 0, 0);	
		right_press = 0;
	}
	if (middle_press) {
		addPart(1000,10,3,0,0);
		middle_press = 0;
	}

	for (int i = 0; i < particles.size(); i++) {
		particle &temp = particles[i];
		int collide = 0;
		for (int j = 0; j < particles.size(); j++) {
			if ((i == j) || (temp.m == 10000)) {
				continue;
			}
			const particle &j1 = particles[j];
			
			float d = sqrt((temp.x - j1.x)*(temp.x - j1.x) + (temp.y - j1.y)*(temp.y - j1.y));
			if (d > j1.r) {
				temp.vx += .03*j1.m / (d*d)*(j1.x - temp.x) / d;
				temp.vy += .03*j1.m / (d*d)*(j1.y - temp.y) / d;
			}
			else {
				collide = 1;
			}

		}
		if (collide == 0) {
			temp.x += temp.vx;
			temp.y += temp.vy;
		}
		else particles.erase(particles.begin() + i);
	}
	glutTimerFunc(1, repeat, 0);

}




void mouse(int but, int state, int x, int y) {
	mx = x - 250;
	my = y - 250;
	if (shoot == 1||right_press==1) {
		if ((line.x2 != 0) && (line.y2 != 0) && state == GLUT_UP && left_press == 1) {
			addPart(100, 5, 1, line.x1 - line.x2, line.y1 - line.y2);
		}
		else{
			line.x1 = line.x2 = mx;
			line.y1 = line.y2 = my;
		}
		
	}
	if (but == GLUT_LEFT_BUTTON && state == GLUT_DOWN) left_press = 1;
	else if (but == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) right_press = 1;
	else if (but == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) middle_press = 1;
}

void key(unsigned char key, int x, int y) {
	switch (key)
	{
	case 's':
	case 'S':
		if (shoot == 0) shoot = 1;
		else shoot = 0;
		break;
	case 'c':
	case 'C':
	{
		int size = particles.size();
		for (int i = 0; i < size; i++) {
			particles.pop_back();
		}
		line.x1 = 0;
		line.x2 = line.y1 = line.y2 = 0;
		break; }
	case 'E':
	case 'e':
		exit(0);
	}
}

void move(int x, int y) {
	mx = x - 250;
	my = y - 250;
	if ((shoot == 1)&&(left_press==1)) {
		line.x2 = mx;
		line.y2 = my;
	}
}
void addPart(float m, float r, int col, float vx, float vy) {
	particle temp;
	temp.x = mx;
	temp.y = my;
	temp.vx = vx/30;
	temp.vy = vy/30;
	temp.m = m;
	temp.r = r;
	if (col == 1) {
		temp.color[0] = 0;
		temp.color[1] = 0;
		temp.color[2] = 1;
	}
	else {
		temp.color[0] = 1;
		temp.color[1] = .3;
		temp.color[2] = 0;
	}
	particles.push_back(temp);
	if (line.x1 != 0) {
		line.x1 = 0;
		line.x2 = 0;
		line.y1 = 0;
		line.y2 = 0;
	}
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex2f(line.x1,line.y1);
	glVertex2f(line.x2, line.y2);
	glEnd();
	for (int i = 0; i < particles.size(); i++) {
		particle &temp = particles[i];
		glColor3f(temp.color[0], temp.color[1], temp.color[2]);
		glBegin(GL_POLYGON);
		for (float j = 0; j < 2 * PI; j += .02) {
			glVertex2f(temp.r*cos(j)+temp.x,temp.r*sin(j)+temp.y);
		}
		glEnd();
	}
	glFlush();
	glutSwapBuffers();

}

int main() {
	particle p;
	p.x = 0;
	p.y = 0;
	p.vx = p.vy = 0;
	p.r = 10;
	p.m = 100000;
	p.color[0] = 1;
	p.color[1] = .3;
	p.color[2] = 0;
	particles.push_back(p);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutCreateWindow("Gravity!");
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-250, 250, 250, -250, 0, 1);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(key);
	glutMotionFunc(move);
	repeat(0);
	glutMainLoop();
	return 0;
}