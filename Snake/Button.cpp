#include "Button.h";

void Button::setColor(float c1, float c2, float c3) {
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
}

void Button::setTextColor(float c1, float c2, float c3) {
	textColor[0] = c1;
	textColor[1] = c2;
	textColor[2] = c3;
}

void Button::checkCollision(float x, float y) {
	if (x >= position[0] && x <= position[0] + size[0] &&
		y >= position[1] && y <= position[1] + size[1]) {
		mouseIntersecting = true;
	}
	else {
		mouseIntersecting = false;
	}
}

void Button::click(int button, int state) {
	if (mouseIntersecting) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			mouseIntersecting = false;
			function();
		}
	}
}

void Button::drawText(string text, int gridSize) {
	int textSize = ((text.length() - 1) * 7) + 24;
	float x = position[0] + (size[0] * 0.5) - (0.5 * (textSize / (float)gridSize));
	float y = position[1] + (size[1] * 0.5) - (0.25 * (24.0f / (float)gridSize));
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void Button::draw(int gridSize) {

	// draw button base
	if (mouseIntersecting)
		glColor3f(hoverColor[0], hoverColor[1], hoverColor[2]);
	else
		glColor3f(color[0], color[1], color[2]);
	glBegin(GL_POLYGON);
	glVertex2f(position[0], position[1]);
	glVertex2f(position[0] + size[0], position[1]);
	glVertex2f(position[0] + size[0], position[1] + size[1]);
	glVertex2f(position[0], position[1] + size[1]);
	glEnd();

	// draw text
	glColor3f(textColor[0], textColor[1], textColor[2]);
	drawText(text, gridSize);
}