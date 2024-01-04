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

bool Tile::isMoving() {
	return moving;
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

void Tile::setMousePosition(float x, float y) {

}

bool Tile::checkCollision(float x, float y) {
	if (x > pos[0] && x < pos[0] + 1 &&
		y > pos[1] && y < pos[1] + 1) {
		containsMouse = true;
		return true;
	}
	else {
		containsMouse = false;
		return false;
	}
}

void Tile::toggleMovement(int button, int state, float x, float y) {
	if (checkCollision(x, y)) {
		if (button == GLUT_LEFT_BUTTON) {
			if (!moving && state == GLUT_DOWN) {
				rootMovPos[0] = pos[0];
				rootMovPos[1] = pos[1];
				prevMousePos[0] = x;
				prevMousePos[1] = y;
				moving = true;
			}
			else if (moving && state == GLUT_DOWN) {
				move(x, y);
			}
			else if (moving && state == GLUT_UP) {
				moving = false;
			}
		}
	}
}

void Tile::move(float x, float y) {
	checkCollision(x, y);
	if (moving) {
		pos[0] = rootMovPos[0] + (x - prevMousePos[0]);
		pos[1] = rootMovPos[1] + (y - prevMousePos[1]);
	}
	else {
		prevMousePos[0] = x;
		prevMousePos[1] = y;
	}
}

void Tile::idleUpdate() {

}

void Tile::drawOutline() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[0] + 1, pos[1]);
	glVertex2f(pos[0] + 1, pos[1] + 1);
	glVertex2f(pos[0], pos[1] + 1);
	glEnd();
}

void Tile::draw(bool outline) {
	glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex2f(pos[0], pos[1]);
	glVertex2f(pos[0] + 1, pos[1]);
	glVertex2f(pos[0] + 1, pos[1] + 1);
	glVertex2f(pos[0], pos[1] + 1);
	glEnd();

	if (outline) {
		drawOutline();
	}
}