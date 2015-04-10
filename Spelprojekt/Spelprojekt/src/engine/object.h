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
	GLuint vertexData;
	GLuint indexBuffer;
	GLuint vertexAttribute;
	GLuint textureId;
	int TEXTUREINDEXOFFSET = 6;
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
	Object(std::string pathVert, std::string pathTex);
	~Object();
	void bind();
	int getFaces();
};

#endif