#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>

struct coord
{
	int x,y;
};

std::vector<coord> snake;

int x,y;
int xFood, yFood;
int timerFreq=1000;
const int SIZE=25;
bool addNew=false;

char activeKey;

void loseOutput()
{
	std::cout<<"You lose!"<<std::endl<<"Your score "<<snake.size()-1<<std::endl;
	exit(0);
}

void genFood()
{
	xFood=std::rand()%750+SIZE;
	xFood=xFood/(SIZE*2)*SIZE*2;
	if(xFood==0) xFood=25;
	yFood=std::rand()%550+SIZE;
	yFood=yFood/(SIZE*2)*SIZE*2;
	if(yFood==0) yFood=25;
}

void drawSnake()
{
	glLoadIdentity();
	for(uint16_t i=0;i<snake.size();i++)
	{
		glBegin(GL_QUADS);
			glColor3f(1,0,0);
			glVertex2i(snake.at(i).x-SIZE,snake.at(i).y+SIZE);
			glVertex2i(snake.at(i).x-SIZE,snake.at(i).y-SIZE);
			glVertex2i(snake.at(i).x+SIZE,snake.at(i).y-SIZE);
			glVertex2i(snake.at(i).x+SIZE,snake.at(i).y+SIZE);
		glEnd();
	}
}

void drawFood()
{
	glLoadIdentity();
	glBegin(GL_QUADS);
		glColor3f(0,1,0);
		glVertex2i(xFood-SIZE,yFood+SIZE);
		glVertex2i(xFood-SIZE,yFood-SIZE);
		glVertex2i(xFood+SIZE,yFood-SIZE);
		glVertex2i(xFood+SIZE,yFood+SIZE);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	drawSnake();
	drawFood();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,w,0,h,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void timer(int)
{
	if(addNew)
	{
		if(snake.size()%5==0)
			timerFreq=timerFreq-100;
		snake.push_back({snake.back().x, snake.back().y});
	}
	for(uint16_t i=snake.size()-1;i>0;i--)
		snake.at(i)=snake.at(i-1);
	if(activeKey=='u') snake.at(0).y+=SIZE*2;
	else if(activeKey=='d') snake.at(0).y-=SIZE*2;
	else if(activeKey=='l') snake.at(0).x-=SIZE*2;
	else if(activeKey=='r') snake.at(0).x+=SIZE*2;
	if((snake.at(0).x<0)||(snake.at(0).x>800)||(snake.at(0).y<0)||(snake.at(0).y>600))
	//	exit(0);
		loseOutput();
	for(uint16_t i=2;i<snake.size();i++)
		if(((snake.at(0).x==snake.at(i).x)&&(snake.at(0).y==snake.at(i).y)))
			//exit(0);
			loseOutput();
	addNew=false;
	glutTimerFunc(timerFreq, timer, 0);
}

void idle()
{
	glutPostRedisplay();
	if(snake.at(0).x==xFood&&snake.at(0).y==yFood)
	{
		genFood();
		addNew=true;
	}
}

void onArrowPress(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		if(activeKey!='d')
			activeKey='u';
		break;
	case GLUT_KEY_DOWN:
		if(activeKey!='u')
			activeKey='d';
		break;
	case GLUT_KEY_LEFT:
		if(activeKey!='r')
			activeKey='l';
		break;
	case GLUT_KEY_RIGHT:
		if(activeKey!='l')
			activeKey='r';
		break;
	}
}

int main(int argc, char *argv[])
{
	snake.push_back({400, 300});
	std::srand(unsigned(std::time(0)));
	genFood();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(800,600);
	glutCreateWindow("Snake");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(onArrowPress);
	glutIdleFunc(idle);
	glutTimerFunc(timerFreq, timer, 0);
	glClearColor(0,0,0,0);
	glutMainLoop();
	return 0;
}
