#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/freeglut.h>
#endif

#include <stdlib.h>
#include <time.h>

#include "Tile.h"

using namespace std;


// = = = = = Variables = = = = =

enum GameState {
    Game,
    GameEnd
};

float gridSize = 100;
int gridDimensions[] = { 5, 5 };

int w = gridDimensions[0];
int h = gridDimensions[1];

int* grid;

GameState gameState = Game;

float cornerColors[] = {
    0.79f, 1.0f, 0.24f,
    0.68f, 0.0f, 0.05f,
    1.0f, 0.23f, 0.52f,
    0.0f, 0.72f, 1.0f
};

Tile** tiles;
int activeTile = 0;
int markedTile = -1;

float prevMousePos[2] = { -1.0f, -1.0f };

// = = = = = Helper Functions = = = = =

void initTiles() {
    // initialize array
    tiles = new Tile * [w * h];

    // initialize corner tiles
    tiles[0] = new Tile(0, 0, 0, cornerColors[0], cornerColors[1], cornerColors[2]);
    tiles[w * h - h] = new Tile(w * h - h, w - 1, 0, cornerColors[3], cornerColors[4], cornerColors[5]);
    tiles[w * h - 1] = new Tile(w * h - 1, w - 1, h - 1, cornerColors[6], cornerColors[7], cornerColors[8]);
    tiles[h - 1] = new Tile(h - 1, 0, h - 1, cornerColors[9], cornerColors[10], cornerColors[11]);

    // initialize bottom edge
    float colorDif[3];
    colorDif[0] = (cornerColors[0] - cornerColors[3]) / w;
    colorDif[1] = (cornerColors[1] - cornerColors[4]) / w;
    colorDif[2] = (cornerColors[2] - cornerColors[5]) / w;
    for (int i = 1; i < w - 1; i++){
        tiles[i * h] = new Tile(i * h, cornerColors[0] - i*colorDif[0], cornerColors[1] - i*colorDif[1], cornerColors[2] - i*colorDif[2]);
        tiles[i * h]->setPosition(i, 0);
    }

    // initialize the right edge
    colorDif[0] = (cornerColors[3] - cornerColors[6]) / h;
    colorDif[1] = (cornerColors[4] - cornerColors[7]) / h;
    colorDif[2] = (cornerColors[5] - cornerColors[8]) / h;
    for (int i = 1; i < h - 1; i++) {
        tiles[(w - 1) * h + i] = new Tile((w - 1) * h + i, cornerColors[3] - i * colorDif[0], cornerColors[4] - i * colorDif[1], cornerColors[5] - i * colorDif[2]);
        tiles[(w - 1) * h + i]->setPosition(w - 1, i);
    }

    // initialize the top edge
    colorDif[0] = (cornerColors[9] - cornerColors[6]) / w;
    colorDif[1] = (cornerColors[10] - cornerColors[7]) / w;
    colorDif[2] = (cornerColors[11] - cornerColors[8]) / w;
    for (int i = 1; i < w - 1; i++) {
        tiles[i * h + h - 1] = new Tile(i * h + h - 1, cornerColors[9] - i * colorDif[0], cornerColors[10] - i * colorDif[1], cornerColors[11] - i * colorDif[2]);
        tiles[i * h + h - 1]->setPosition(i, h - 1);
    }

    // initialize the left edge
    colorDif[0] = (cornerColors[0] - cornerColors[9]) / h;
    colorDif[1] = (cornerColors[1] - cornerColors[10]) / h;
    colorDif[2] = (cornerColors[2] - cornerColors[11]) / h;
    for (int i = 1; i < h - 1; i++) {
        tiles[i] = new Tile(i, cornerColors[0] - i * colorDif[0], cornerColors[1] - i * colorDif[1], cornerColors[2] - i * colorDif[2]);
        tiles[i]->setPosition(0, i);
    }

    // fill in the middle
    for (int i = 1; i < w - 1; i++) {
        colorDif[0] = (tiles[i * h]->getColor(0) - tiles[i * h + h - 1]->getColor(0)) / h;
        colorDif[1] = (tiles[i * h]->getColor(1) - tiles[i * h + h - 1]->getColor(1)) / h;
        colorDif[2] = (tiles[i * h]->getColor(2) - tiles[i * h + h - 1]->getColor(2)) / h;
        int counter = 1;
        for (int j = i * h + 1; j < (i * h) + h - 1; j++) {
            tiles[j] = new Tile(j, i, counter, 
                tiles[i * h]->getColor(0) - counter * colorDif[0], 
                tiles[i * h]->getColor(1) - counter * colorDif[1], 
                tiles[i * h]->getColor(2) - counter * colorDif[2]);
            counter++;
        }
    }
}


void swapTiles(int t1, int t2) {
    float tempPos[2] = { tiles[grid[t2]]->rootMovPos[0], tiles[grid[t2]]->rootMovPos[1] };
    tiles[grid[t2]]->setPosition(tiles[grid[t1]]->rootMovPos[0], tiles[grid[t1]]->rootMovPos[1]);
    tiles[grid[t1]]->setPosition(tempPos[0], tempPos[1]);
    tiles[grid[t2]]->stopMoving();
    tiles[grid[t1]]->stopMoving();
    int temp = grid[t2];
    grid[t2] = grid[t1];
    grid[t1] = temp;
}

// = = = = = Callback Functions = = = = =

void init(void)
{
    // initialize grid
    grid = new int [w * h];
    for (int i = 0; i < w * h; i++) grid[i] = -1;

    // initialize tiles
    initTiles();
    srand((int)time(NULL));
    grid[0] = 0;
    grid[w * h - h] = w * h - h;
    grid[w * h - 1] = w * h - 1;
    grid[h - 1] = h - 1;
    for (int i = 0; i < w * h; i++) {
        if (i != 0 && i != w * h - h && i != w * h - 1 && i != h - 1) {
            int pos = rand() % (w * h);
            while (grid[pos] != -1) {
                pos = (pos + 1) % (w * h);
            }
            grid[pos] = i;
            tiles[i]->setPosition((int)(pos / h), pos % h);
        }
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && tiles[grid[activeTile]]->isMoving()) {
        int swapTile = ((int)(x / gridSize) * h) + (int)(((h * gridSize) - y) / gridSize);
        swapTiles(activeTile, swapTile);
    }
    for (int i = 0; i < w * h; i++) {
        tiles[i]->toggleMovement(button, state, x / gridSize, ((h * gridSize) - y) / gridSize);
    }
    prevMousePos[0] = x / gridSize;
    prevMousePos[1] = ((h * gridSize) - y) / gridSize;
}

void mouseMove(int x, int y) {
    float mouseX = x / gridSize;
    float mouseY = ((h * gridSize) - y) / gridSize;
    markedTile = -1;
    if (prevMousePos[0] != -1) {
        if (mouseX + (mouseX - prevMousePos[0]) <= 0 ||
            mouseX + (mouseX - prevMousePos[0]) >= w ||
            mouseY + (mouseY - prevMousePos[1]) <= 0 ||
            mouseY + (mouseY - prevMousePos[1]) >= h ) {
            for (int i = 0; i < w * h; i++) {
                tiles[i]->containsMouse = false;
                if (tiles[i]->isMoving()) {
                    swapTiles(activeTile, ((int)mouseX * h) + (int)mouseY);
                }
            }
        }
        else {
            for (int i = 0; i < w * h; i++) {
                tiles[i]->move(x / gridSize, ((h * gridSize) - y) / gridSize);
            }
        }
    }
    prevMousePos[0] = mouseX;
    prevMousePos[1] = mouseY;
}

void keyboard(unsigned char key, int x, int y) {
    switch (gameState) {
    case Game:

        // Use the arrow keys to change selected tile
        if (key == 'w') {
            if ((activeTile + 1) % h != 0) {
                activeTile += 1;
            }
        }
        else if (key == 's') {
            if (activeTile % h != 0) {
                activeTile -= 1;
            }
        }
        else if (key == 'd') {
            if (activeTile + h < w * h) {
                activeTile += h;
            }
        }
        else if (key == 'a') {
            if (activeTile - h >= 0) {
                activeTile -= h;
            }
        }
        // Use the spacebar to select tiles to swap
        else if (key == 32) {
            if (markedTile == -1) {
                markedTile = activeTile;
            }
            else {
                swapTiles(markedTile, activeTile);
                markedTile = -1;
            }
        }

        break;
    case GameEnd:
        break;
    }

}


void update() {

    
    
    glutPostRedisplay();
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    bool movingTile = false;
    for (int i = 0; i < w * h; i++) {
        if (!movingTile && tiles[grid[i]]->isMoving()) {
            movingTile = true;
            tiles[grid[activeTile]]->draw(false);
            activeTile = i;
        }
        else if (!movingTile && tiles[grid[i]]->containsMouse) {
            tiles[grid[activeTile]]->draw(false);
            activeTile = i;
        } 
        else if (i != markedTile) {
            tiles[grid[i]]->draw(false);
        }
    }
    if (markedTile != -1)
        tiles[grid[markedTile]]->draw(!movingTile);
    tiles[grid[activeTile]]->draw(!movingTile);

    glutSwapBuffers();
}


void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, gridDimensions[0], 0.0, gridDimensions[1]);
    //gluOrtho2D(-5.0, 5.0, -5.0, 5.0);

    glViewport(0, 0, (GLsizei)(gridSize * gridDimensions[0]), (GLsizei)(gridSize * gridDimensions[1]));
    //glViewport((GLsizei) width/2, (GLsizei) height/2, (GLsizei) width, (GLsizei) height);

    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize((int)(gridSize * gridDimensions[0]), (int)(gridSize * gridDimensions[1]));

    glutCreateWindow("Color Game");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMove);
    glutIdleFunc(update);

    glutMainLoop();

    return 0;
}