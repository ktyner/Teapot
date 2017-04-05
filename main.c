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

int vertexCount, texCount, faceCount;
GLuint *faces;
int boxShaderProgram;

int vertexVBOID[2], indexVBOID[2];

int createViewVolume() {
  glEnable(GL_DEPTH_TEST);

  // Specify shape and size of the view volume.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); //load identity matrix into GL_PROJECTION
  gluPerspective(45.0, 1.0, 0.1, 20.0);

  // Specify the position for the view volume
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /*gluLookAt(0.0, 1.5, -3.0,  // eye xyz,
            0.0, 1.5, 3.0,  // view xyz,
            0.0, 1.0, 0.0); // up xyz
	*/

	gluLookAt(0.0, 0.0, -4.0,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
  return 0;
}

int createLights() {
  // Fill light
  float light0_ambient[] = { 0.3, 0.3, 0.3, 0.0 };
  float light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  float light0_specular[] = { 1.0, 0.4, 1.3, 1.0 };
  float light0_position[] = { 0.0, 1.5, 0.0, 1.0 };
  float light0_direction[] = { 0.0, 0.0, 0.0, 1.0 };

  // Turn off scene default ambient.
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

  // Make specular correct for spots.
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

  glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
  glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
  glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
  glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.001);
  glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0001);
  glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
  glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

  glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);
  return 0;
}

void set_material()
{
	float mat_diffuse[] = {1.0, 1.0, 1.0,1.0};
	float mat_specular[] = {0.9,0.9,0.9,1.0};
	float mat_shininess[] = {85.0};

	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

char *readShader(char *filename)
{
	char *content = NULL;
	int fd, count;
	fd = open(filename, O_RDONLY);
	count = lseek(fd, 0, SEEK_END);
	content = (char *)calloc(1, (count+1));
	lseek(fd, 0, SEEK_SET);
	count = read(fd, content, count*sizeof(char));
	content[count] = '\0';
	close(fd);
	return content;
}

// Take this out before turning the assignment in
void debugShaderProgram(int id)
{
	int linkStatus = -1;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);

	int compileStatus = -1;
	glGetProgramiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (linkStatus == GL_FALSE || compileStatus == GL_FALSE)
	{
		int size;
		char *c;
		glGetProgramInfoLog(id, 1024, &size, &c[0]);
		printf("%s\n", c);
	} else {
		printf("Shaders loaded correctly.\n");
	}
}

unsigned int loadShaders(char *vertexShaderName, char *fragmentShaderName) 
{
	int result = -1;
	char *vs, *fs;
	GLuint p;
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	vs = readShader(vertexShaderName);
	fs = readShader(fragmentShaderName);

	glShaderSource(vertexShaderID, 1, (const char **)&vs, NULL);
	glShaderSource(fragmentShaderID, 1, (const char **)&fs, NULL);
	free(vs);
	free(fs);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	p = glCreateProgram();
	glAttachShader(p, fragmentShaderID);
	glAttachShader(p, vertexShaderID);
	glLinkProgram(p);
	glUseProgram(p);

	// Take this out before turning the assignment in
	debugShaderProgram(p);
	
	return p;
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID[0]);
	//glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, NULL+0);
	//printf("Bind buffer...\n");
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID[1]);
	//glDrawElements(GL_QUADS, faceCount*12, GL_UNSIGNED_INT, NULL+0);
	glEnableClientState(GL_COLOR_ARRAY);
	printf("Drawing elements...\n");
	glDisableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_QUADS, faceCount*3, GL_UNSIGNED_INT, faces);
	printf("Elements should be drawn\n");
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

void loadBox()
{
	GLfloat vertices[] =
	{
		// Positions		// Colors			// Normals
		//Back face
		-1.5, 3.0, 3.0, 	1.0, 1.0, 1.0,		0.0, 0.0, -1.0,
		1.5, 3.0, 3.0, 		1.0, 1.0, 1.0,		0.0, 0.0, -1.0,
		1.5, 0.0, 3.0, 		1.0, 1.0, 1.0,		0.0, 0.0, -1.0,
		-1.5, 0.0, 3.0, 	1.0, 1.0, 1.0,		0.0, 0.0, -1.0,

		//Right face
		-1.5, 3.0, -3.0, 	0.0, 1.0, 0.0,		1.0, 0.0, 0.0,
		-1.5, 3.0, 3.0, 	0.0, 1.0, 0.0,		1.0, 0.0, 0.0,
		-1.5, 0.0, 3.0, 	0.0, 1.0, 0.0,		1.0, 0.0, 0.0,
		-1.5, 0.0, -3.0, 	0.0, 1.0, 0.0,		1.0, 0.0, 0.0,
		
		//Left face
		1.5, 3.0, -3.0, 	1.0, 0.0, 0.0,		-1.0, 0.0, 0.0,
		1.5, 3.0, 3.0, 		1.0, 0.0, 0.0,		-1.0, 0.0, 0.0,
		1.5, 0.0, 3.0, 		1.0, 0.0, 0.0,		-1.0, 0.0, 0.0,
		1.5, 0.0, -3.0, 	1.0, 0.0, 0.0,		-1.0, 0.0, 0.0,

		//Top face
		-1.5, 3.0, 3.0, 	1.0, 1.0, 1.0,		0.0, -1.0, 0.0,
		1.5, 3.0, 3.0, 		1.0, 1.0, 1.0,		0.0, -1.0, 0.0,
		1.5, 3.0, -3.0, 	1.0, 1.0, 1.0,		0.0, -1.0, 0.0,
		-1.5, 3.0, -3.0, 	1.0, 1.0, 1.0,		0.0, -1.0, 0.0,

		//Bottom face
		-1.5, 0.0, 3.0, 	1.0, 1.0, 1.0,		0.0, 1.0, 0.0,
		1.5, 0.0, 3.0, 		1.0, 1.0, 1.0,		0.0, 1.0, 0.0,
		1.5, 0.0, -3.0, 	1.0, 1.0, 1.0,		0.0, 1.0, 0.0,
		-1.5, 0.0, -3.0, 	1.0, 1.0, 1.0,		0.0, 1.0, 0.0
	};

	GLubyte face[] =
	{
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 10, 11,
		12, 13, 14, 15,
		16, 17, 18, 19
	};

	glGenBuffers(1, &vertexVBOID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexPointer(3, GL_FLOAT, 9*sizeof(GLfloat), NULL+0);
	glColorPointer(3, GL_FLOAT, 9*sizeof(GLfloat), NULL+12);
	glNormalPointer(GL_FLOAT, 9*sizeof(GLfloat), NULL+24);

	glGenBuffers(1, &indexVBOID[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);
}

void loadTeapot(const char *objFile)
{
	GLfloat *vertices, *normals, *tangents, *bitangents, *textureCoords;

	char *data;

	FILE *file = fopen(objFile, "r");
	char line[256];

	// Obtain vertexCount and faceCount
	while (fgets(line, sizeof(line), file))
	{
		if (line[0]+line[1] == 'v'+' ')
		{
			vertexCount++;
		}
		else if (line[0]+line[1] == 'v'+'t') 
		{
			texCount++;
		} else if (line[0] == 'f') {
			faceCount++;
		}
	}
	printf("Rewinding File...\n");
	rewind(file);

	// Allocate memory for storing all data from file
	vertices = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	normals = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	tangents = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	bitangents = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	faces = (GLuint *)malloc(sizeof(GLuint) * faceCount * 12); // |faces| == |vertices|
	textureCoords = (GLfloat *)malloc(sizeof(GLfloat) * texCount * 2);

	// Reset to use as index
	int currentVertex = 0, currentTexCoord = 0, currentNormal = 0;
	int currentTangent = 0, currentBiTangent = 0, currentFace = 0;

	int maxIndex = 0;
	
	// Parse File
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == '#') continue;

		if (line[0] == 'v')
		{
			switch (line[1])
			{
				case 't':
					data = strtok(line, " ");
					textureCoords[currentTexCoord++] = atof(strtok(NULL, " "));
					textureCoords[currentTexCoord++] = atof(strtok(NULL, " "));
					break;
				case 'x':
					data = strtok(line, " ");
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					break;
				case 'y':
					data = strtok(line, " ");
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					break;
				case 'n':
					data = strtok(line, " ");
					normals[currentNormal++] = atof(strtok(NULL, " "));
					normals[currentNormal++] = atof(strtok(NULL, " "));
					normals[currentNormal++] = atof(strtok(NULL, " "));
					break;
				case ' ':
					data = strtok(line, " ");
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					break;
				default:
					break;
			}
		} 
		else if (line[0] == 'f') 
		{
			strtok(line, " ");
			int i = 0;			
			while (i < 12)
			{
				data = strtok(NULL, " /");
				int element = atoi(data);
				if (element > maxIndex) {
					maxIndex = element;
				}
				faces[currentFace++] = atoi(data);
				i++;
			}
		}
		else
		{
			char *mtlFile = strtok(line, " ");
			if (strstr(mtlFile, "mtllib"))
			{
				mtlFile = strtok(NULL, " \n");
				
				// Process mtlFile here
			}
		}
	}

	printf("%d\n", maxIndex);

	printf("Finished. Closing file...\n");
	fclose(file);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBOID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), NULL+0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	*/
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
	printf("Put data in GPU\n");
}

void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glGenBuffers(2, &indexVBOID[0]);
	glGenBuffers(2, &vertexVBOID[0]);
}

int main(int argc, char **argv) 
{
	init(argc, argv);
	
	printf("Creating view volume\n");
	createViewVolume();

	printf("Loading teapot\n");
	loadTeapot("teapot.605.obj");

	//printf("Creating box\n");
	//loadBox();

	printf("Creating Lighting\n");
	createLights();

	printf("Creating materials\n");
	set_material();

	printf("Creating shaders\n");
	boxShaderProgram = loadShaders("boxShader.vert", "boxShader.frag");
	
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}

