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
#include <math.h>

#include "global.h"
#include "shader.h"
#include "load_geom.h"
#include "halton.h"

#define XRES 1280
#define YRES 1024


void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE|GLUT_ACCUM|GLUT_DOUBLE);
	glutInitWindowSize(XRES, YRES);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	//glClearColor(1.0, 0.0, 0.7, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

}

float myrand() { return ((float)(rand() % 100000)/100000.0 - 0.5); }

void display() 
{
	Vector3f lightJitter = {myrand()*0.4, myrand()*0.07, myrand()*0.07};
	Vector3f aaJitter = {myrand(), myrand(), myrand()};
	aaJitter = mul(aaJitter, 0.003);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Render Light
	glDisableClientState(GL_NORMAL_ARRAY);
		glUseProgram(0);
		glVertexPointer(3, GL_FLOAT, 0, sceneData.toplight.vertices);
		glPushMatrix();
			glTranslatef(lightJitter.y, 0.9999, lightJitter.z);
			glScalef(1.0-lightJitter.x, 1.0-lightJitter.x, 1.0-lightJitter.x);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, sceneData.toplight.indices);
		glPopMatrix();
	glEnableClientState(GL_NORMAL_ARRAY);

	// Render Box
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glUseProgram(boxShaderProgramID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTextureID);
			glVertexPointer(3, GL_FLOAT, 0, sceneData.box.vertices);
			glColorPointer(3, GL_FLOAT, 0, sceneData.box.colors);
			glNormalPointer(GL_FLOAT, 0, sceneData.box.normals);
			glTexCoordPointer(2, GL_FLOAT, 0, sceneData.box.texCoords);
			glEnableClientState(GL_COLOR_ARRAY);
				glPushMatrix();
					glTranslatef(aaJitter.x, aaJitter.y, aaJitter.z);
					glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, sceneData.box.indices);
				glPopMatrix();
			glDisableClientState(GL_COLOR_ARRAY);
		glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Render Teapot
	glUseProgram(teapotShaderProgramID);
	glVertexPointer(3, GL_FLOAT, 0, sceneData.teapot.vertices);
	glNormalPointer(GL_FLOAT, 0, sceneData.teapot.normals);
	glDisableClientState(GL_COLOR_ARRAY);
	glPushMatrix();
		glTranslatef(aaJitter.x, aaJitter.y-1.0, aaJitter.z);
		glRotatef(50.0, 0.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glDrawElements(GL_QUADS, sceneData.teapot.curIndex, GL_UNSIGNED_INT, sceneData.teapot.indices);
	glPopMatrix();
	glFlush();
}

void input(unsigned char key, int x, int y) { if (key=='q') exit(0); }

void createLights(Vector3f pos, Vector3f color) {
	light0_diffuse[0] = color.x, light0_diffuse[1] = color.y, light0_diffuse[2] = color.z, light0_diffuse[3] = 1.0;
	light0_position[0] = pos.x,	light0_position[1] = pos.y, light0_position[2] = pos.z,	light0_position[3] = 1.0;

	float light0_specular[] = { color.x, color.y, color.z, 1.0 };

	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.001);
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0001);
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

	glEnable(GL_LIGHT0);
}

void createViewVolume(Vector3f ep, Vector3f vp) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)(XRES)/(float)(YRES), 0.1, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ep.x, ep.y, ep.z, vp.x, vp.y, vp.z, 0.0, 1.0, 0.0);
}

void save_matrix(float *ep, float *vp)
{
	glMatrixMode(GL_TEXTURE); 
	glActiveTexture(GL_TEXTURE7);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-0.005);
	glScalef(0.5,0.5,0.5);
	glTranslatef(1.0,1.0,1.0);
	gluPerspective(45.0,(float)(XRES)/(float)(YRES),0.1,20.0);
	gluLookAt(ep[0], ep[1], ep[2], vp[0], vp[1], vp[2], 0.0,1.0,0.0);
}

void renderWithShadows(Vector3f lightPosition, Vector3f lightColor)
{
	float eyepoint[3], viewpoint[3];
	int k;

	// Fill the framebuffer with depth data from the point of view of the light
	glBindFramebufferEXT(GL_FRAMEBUFFER, 1);
		glUseProgram(0);

		light0_position[0] = lightPosition.x, light0_position[1] = lightPosition.y, light0_position[2] = lightPosition.z;

		// Point the light at the teapot, wherever the light is
		light0_direction[0] = -light0_position[0];
		light0_direction[1] = -1.0-light0_position[1];
		light0_direction[2] = -light0_position[2];

		for (k = 0; k < 3; k++) {
			eyepoint[k] = light0_position[k];
			viewpoint[k] = light0_position[k] + light0_direction[k];
		}

		Vector3f ep = {eyepoint[0], eyepoint[1], eyepoint[2]};
		Vector3f vp = {viewpoint[0], viewpoint[1], viewpoint[2]};
		createViewVolume(ep, vp);

		Vector3f l0_pos = {light0_position[0], light0_position[1], light0_position[2]};
		createLights(l0_pos, lightColor);
		display();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	// Store this viewpoint matrix into texture matrix 7
	save_matrix(eyepoint, viewpoint);

	glUseProgram(boxShaderProgramID);

	int location;
	location = glGetUniformLocation(boxShaderProgramID, "depthTexture");
	glUniform1i(location, 6);
	location = glGetUniformLocation(teapotShaderProgramID, "depthTexture");
	glUniform1i(location, 6);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);

	eyepoint[0] = 0.0, eyepoint[1] = 0.0, eyepoint[2] = -3.41;
	viewpoint[0] = 0.0, viewpoint[1] = 0.0, viewpoint[2] = 0.0;

	ep.x = eyepoint[0], ep.y = eyepoint[1], ep.z = eyepoint[2];
	vp.x = viewpoint[0], vp.y = viewpoint[1], vp.z = viewpoint[2];
	createViewVolume(ep, vp);
	createLights(l0_pos, lightColor);
	display();
}

int trace(int j, int N, Vector3f currentRay, Vector3f v0, Vector3f v1, Vector3f v2)
{
	Vector3f color;
	Vector3f source = {0.0, 0.9, 0.0};
	Vector3f lightPosition = getRayTriangleIntersectionPoint(source, currentRay, v0, v1, v2);
	if (lightPosition.x != RAYNOTHIT)
	{
		lightPosition = mul(lightPosition, 0.8);
		// Move low light closer to the middle
		if (lightPosition.y <= 0.0) lightPosition.y = 0.5 + myrand()*0.3;
		color = mul(sceneData.box.colors[j], 0.5);
		
		renderWithShadows(lightPosition, color);
		glAccum(GL_ACCUM, 1.0/(float)N);
		return 1;
	}
	return 0;
}

void renderScene()
{
	int N = 1000;
	glClear(GL_ACCUM_BUFFER_BIT);

	int i;
	for (i = 0; i < N; i++)
	{
		Vector3f currentRay = normalize(halton(i));
		Vector3f v0, v1, v2;

		// Only check collisions w/ box. 
		// Checking collisions w/ teapot gives negligible results.
		int j;
		for (j = 0; j < 20; j+=4)
		{
			v0 = sceneData.box.vertices[j];
			v1 = sceneData.box.vertices[j+1];
			v2 = sceneData.box.vertices[j+2];
			trace(j, N, currentRay, v0, v1, v2);

			v0 = sceneData.box.vertices[j+2];
			v1 = sceneData.box.vertices[j+3];
			v2 = sceneData.box.vertices[j];
			trace(j, N, currentRay, v0, v1, v2);
		}
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
}

void prepareFramebuffer()
{
	glGenTextures(1, &depthTextureID);
	// Set properties of texture id #1.
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, XRES, YRES, 0, 
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER,frameBufferID);
	glDrawBuffer(GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,depthTextureID,0);
	glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}

int loadTexture(GLenum activeTexture, const char *filename, int textureID) {
	FILE *fopen(), *fptr;
	char buf[512];
	int im_size, im_width, im_height, max_color;
	unsigned char *texture_bytes, *parse;

	fptr=fopen(filename,"r");
	if (!fptr) return -1;
	fgets(buf,512,fptr);
	do{
	fgets(buf,512,fptr);
	} while(buf[0]=='#');

	parse = strtok(buf," \t");
	im_width = atoi(parse);

	parse = strtok(NULL," \n");
	im_height = atoi(parse);

	fgets(buf,512,fptr);
	parse = strtok(buf," \n");
	max_color = atoi(parse);

	im_size = im_width*im_height;
	texture_bytes = (unsigned char *)calloc(3,im_size);
	fread(texture_bytes,3,im_size,fptr);
	fclose(fptr);
	
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,
	           GL_UNSIGNED_BYTE,texture_bytes);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	free(texture_bytes);

	return 0;
}

int main(int argc, char **argv) 
{
	init(argc, argv);

	printf("Loading textures...\n");
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &floorTextureID);
	loadTexture(GL_TEXTURE0, "floor.ppm", floorTextureID);

	int location = glGetUniformLocation(boxShaderProgramID, "floorTexture");
	glUniform1i(location, 0);

	printf("Preparing Framebuffer...\n");
	prepareFramebuffer();

	printf("Loading environment...\n");
	loadTopLight(2.0, 2.0);
	loadTeapot("teapot.605.obj");
	loadBox(2.5, 2.0, 2.0);

	printf("Loading Shaders...\n");
	boxShaderProgramID = loadShaders("boxShader.vert", "boxShader.frag");
	teapotShaderProgramID = loadShaders("teapotShader.vert", "teapotShader.frag");

	glutDisplayFunc(renderScene);
	glutKeyboardFunc(input);

	glutMainLoop();
}

