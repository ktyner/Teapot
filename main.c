#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void update()
{

}

void input(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q':
			exit(0);
		default:
			break;
	}
}

void loadTeapot(const char *objFile)
{
	FILE *file = fopen(objFile, "r");
	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		char *data;
		data = strtok(line, " ");
		if (strstr(data, "#")) 
		{
			printf("%s", line);
		}
	}
	fclose(file);
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);

	loadTeapot("teapot.605.obj");
	
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}
