#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/freeglut.h>
#endif

class Tile {

public:
	float pos[2] = { 0.0f, 0.0f };

private:
	int id;
	float color[3] = { 1.0f, 1.0f, 1.0f };

public:
	Tile(int numID, float x, float y);
	Tile(int numID, float c1, float c2, float c3);
	Tile(int numID, float x, float y, float c1, float c2, float c3);
	int getID();
	float getColor(int rgb);
	void setColor(float c1, float c2, float c3);
	void setPosition(float x, float y);
	void draw();

private:

};