#include "Tile.h"

Tile::Tile(int numID, float x, float y) {
	id = numID;
	pos[0] = x;
	pos[1] = y;
}

Tile::Tile(int numID, float c1, float c2, float c3) {
	id = numID;
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
}

Tile::Tile(int numID, float x, float y, float c1, float c2, float c3) {
	id = numID;
	pos[0] = x;
	pos[1] = y;
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
}

int Tile::getID() {
	return id;
}

float Tile::getColor(int rgb) {
	if (rgb == 0) return color[0];
	else if (rgb == 1) return color[1];
	else return color[2];
}

void Tile::setColor(float c1, float c2, float c3) {
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
}

void Tile::setPosition(float x, float y) {
	pos[0] = x;
	pos[1] = y;
}

void Tile::draw() {
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[0] + 1, pos[1]);
	glVertex2f(pos[0] + 1, pos[1] + 1);
	glVertex2f(pos[0], pos[1] + 1);
	glEnd();
}