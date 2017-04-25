#ifndef SHADER_H
#define SHADER_H

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

unsigned int loadShaders(char *vertexShaderName, char *fragmentShaderName) 
{
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

#endif