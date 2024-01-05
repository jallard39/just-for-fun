#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/freeglut.h>
#endif

class Tile {

public:
	bool containsMouse = false;
	float rootMovPos[2] = { 0.0f, 0.0f };

private:
	int id;
	bool moving = false;
	float pos[2] = { 0.0f, 0.0f };
	float color[3] = { 1.0f, 1.0f, 1.0f };
	float prevMousePos[2] = { -1.0f, -1.0f };

public:
	Tile(int numID, float x, float y);
	Tile(int numID, float c1, float c2, float c3);
	Tile(int numID, float x, float y, float c1, float c2, float c3);
	int getID();
	bool isMoving();
	void stopMoving();
	bool checkCollision(float x, float y);
	float getColor(int rgb);
	void setColor(float c1, float c2, float c3);
	void setPosition(float x, float y);
	void setMousePosition(float x, float y);
	void toggleMovement(int button, int state, float x, float y);
	void move(float x, float y);
	void idleUpdate();
	void draw(bool outline);

private:
	void drawOutline();
};