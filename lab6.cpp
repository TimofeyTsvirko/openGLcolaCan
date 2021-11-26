#include <GL/glut.h>
#include <GL/gl.h>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
double PI = 3.14159265;

GLuint textures[3];
int count = 0;

void LoadImages()
{
	int width[3], height[3], channels[3];

	unsigned char* topData = stbi_load("img/top.png", &width[0], &height[0], &channels[0], 0);
	unsigned char* sideData = stbi_load("img/sides.png", &width[1], &height[1], &channels[1], 0);
	unsigned char* bottomData = stbi_load("img/bottom.png", &width[2], &height[2], &channels[2], 0);

	glGenTextures(3, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[0], height[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, topData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[1], height[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, sideData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[2], height[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, bottomData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(topData);
	stbi_image_free(sideData);
	stbi_image_free(bottomData);
}

void DisplayTopBottom(double r, double angleOffset, double h, int tID = 0)
{
	glBindTexture(GL_TEXTURE_2D, textures[tID]);

	glBegin(GL_POLYGON);
		for (double angle = 0; angle < PI * 2; angle += angleOffset) {
			glTexCoord2f((cos(angle) + 0.5) / 2 + 0.25, (sin(angle) + 0.5) / 2 + 0.25);
			glVertex3f(cos(angle) * r, sin(angle) * r, h);
		}
	glEnd();
}

void DisplaySide(double r, double angleOffset, double h)
{
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	double x1, x2, y1, y2;
	double t1, t2;
	for (double angle = 0; angle < PI * 2; angle += angleOffset) {
		x1 = cos(angle) * r;
		y1 = sin(angle) * r;
		x2 = cos(angle + angleOffset) * r;
		y2 = sin(angle + angleOffset) * r;

		t1 = (angle) / (2 * PI);
		t2 = (angle + angleOffset) / (2 * PI);

		glBegin(GL_POLYGON);
			glTexCoord2f(t1, 0);
			glVertex3f(x1, y1, 0);
			glTexCoord2f(t2, 0);
			glVertex3f(x2, y2, 0);
			glTexCoord2f(t2, 1);
			glVertex3f(x2, y2, h);
			glTexCoord2f(t1, 1);
			glVertex3f(x1, y1, h);
		glEnd();
	}
}

void Display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glRotatef(120, 1, 0, 0);
	glRotatef(180, 0, 0, 1);

	LoadImages();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	double radius = 0.25;
	double height = 0.5;
	double angleOffset = PI/50;

	DisplayTopBottom(radius, angleOffset, 0);
	DisplaySide(radius, angleOffset, height);
	DisplayTopBottom(radius, angleOffset, height, 2);

	glDeleteTextures(2, textures);
	glutSwapBuffers();
}

int main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Lab6");
	glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}