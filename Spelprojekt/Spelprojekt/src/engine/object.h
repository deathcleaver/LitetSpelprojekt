#ifndef OBJECT_H
#define OBJECT_H

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

class Object
{
private:
	GLuint vertexData = 0;
	GLuint indexBuffer = 0;
	GLuint vertexAttribute = 0;
	bool vertexHost = false;
	GLuint textureId = 0;
	bool textureHost = false;
	int TEXTUREINDEXOFFSET = 0;
	int faceCount = -1;

	struct TriangleVertex
	{
		float x, y, z;
		float u, v;
	};

	//temprary implementations
	bool loadVert(std::string path);
	bool loadBMP(std::string path); 

public:
	Object(std::string pathVert, std::string pathTex, Object* obj = 0, bool copyVert = false, bool copyTex = false);
	~Object();
	void bind();
	void bindTexOnly();
	void bindVertOnly();
	int getFaces();
};

#endif