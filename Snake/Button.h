#pragma once
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <string>

using namespace std;

class Button {

public:
	float position[2];
private:
	string text;
	float color[3] = { 0.5f, 0.5f, 0.5f };
	float textColor[3] = { 1.0f, 1.0f, 1.0f };
	float hoverColor[3];
	float size[2];

	bool mouseIntersecting = false;

	void (*function)();

public:
	Button(string t, float x, float y, float width, float height, void (*func)()) {
		text = t;
		position[0] = x;
		position[1] = y;
		size[0] = width;
		size[1] = height;
		function = func;

		float hoverDifference = 0.2f;
		if (color[0] >= hoverDifference) hoverColor[0] = color[0] - hoverDifference;
		if (color[1] >= hoverDifference) hoverColor[1] = color[1] - hoverDifference;
		if (color[2] >= hoverDifference) hoverColor[2] = color[2] - hoverDifference;
	}
	void setColor(float c1, float c2, float c3);
	void setTextColor(float c1, float c2, float c3);
	void checkCollision(float x, float y);
	void click(int button, int state);
	void draw(int gridSize);
private:
	void drawText(string text, int gridSize);
};