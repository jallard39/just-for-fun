#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/freeglut.h> 
#endif

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "Button.h";

using namespace std;


// = = = = = Variables = = = = =

enum GameState {
    MainMenu,
    Game,
    GameOver
};

GameState gameState = MainMenu;

Button* buttonStart;
Button* buttonTryAgain;

void (*tryAgain)();

float gridSize = 30;
int gridDimensions[] = { 30, 20 };

int** grid;

int playerDirection;
int headPosition[2];
vector<int> bodyPosition;
int playerLength;

float playerSpeed = 10.0f; // movements per second

unsigned int curTime;
unsigned int preTime;
float deltaTime;
float playerTimer;

int foodPosition[] = { 0, 0 };


// = = = = = Init = = = = =

void startGame() {
    gameState = Game;
}

void init(void)
{
    // Set up buttons
    buttonStart = new Button("Start Game", (gridDimensions[0]*0.5) - 3, (gridDimensions[1]*0.35), 6, 2, startGame);
    buttonTryAgain = new Button("Play Again", (gridDimensions[0] * 0.5) - 3, (gridDimensions[1] * 0.35), 6, 2, tryAgain);
    
    // Set up game variables
    playerDirection = 0;
    playerLength = 1;
    headPosition[0] = 0.5 * gridDimensions[0];
    headPosition[1] = 0.35 * gridDimensions[1];
    curTime = 0;
    preTime = 0;
    deltaTime = 0;
    playerTimer = 0;
    foodPosition[0] = 0;
    foodPosition[1] = 0;

    // Set up grid
    grid = new int* [gridDimensions[0]];
    for (int i = 0; i < gridDimensions[0]; i++) {
        grid[i] = new int[gridDimensions[1]] {0};
    }

    // Set up player
    bodyPosition.push_back(headPosition[0]);
    bodyPosition.push_back(headPosition[1]);
    grid[headPosition[0]][headPosition[1]] = 1;

    // Set up food
    srand(time(NULL));
    do {
        foodPosition[0] = rand() % gridDimensions[0];
        foodPosition[1] = rand() % gridDimensions[1];
    } while (grid[foodPosition[0]][foodPosition[1]] == 1);
    grid[foodPosition[0]][foodPosition[1]] = 2;
}


// = = = = = Helper Functions = = = = =

void drawSquare(int x, int y, int size) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}

void newFoodPosition() {
    do {
        foodPosition[0] = rand() % gridDimensions[0];
        foodPosition[1] = rand() % gridDimensions[1];
    } while (grid[foodPosition[0]][foodPosition[1]] == 1);
    grid[foodPosition[0]][foodPosition[1]] = 2;
}

void restart() {
    for (int i = 0; i < gridDimensions[0]; i++)
        delete[] grid[i];
    delete[] grid;
    bodyPosition.clear();
    init();
    gameState = Game;
    glutMainLoop();
}

void drawText(string text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

float textOffsetX(string text) {
    float offset = 0;
    for (char c : text) {
        if (isupper(c))
            offset += 14;
        else
            offset += 9;
    }

    return (0.5 * offset / gridSize);
}


// = = = = = Callback Functions = = = = =

void update(void) {
    curTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (float)(curTime - preTime) / 1000.0f;

    switch (gameState) {
    case MainMenu:

        break;

    case Game:

        // Update player position
        playerTimer += deltaTime;
        if (playerTimer >= 1 / playerSpeed) {
            switch (playerDirection) {
            case 0:
                headPosition[1] += 1; break;
            case 1:
                headPosition[0] += 1; break;
            case 2:
                headPosition[1] -= 1; break;
            case 3:
                headPosition[0] -= 1; break;
            }

            if (headPosition[0] < 0 || headPosition[0] >= gridDimensions[0] ||
                headPosition[1] < 0 || headPosition[1] >= gridDimensions[1])
                gameState = GameOver;

            // update head position
            if (gameState != GameOver) {
                if (grid[headPosition[0]][headPosition[1]] == 1) {
                    gameState = GameOver;
                }
                else if (grid[headPosition[0]][headPosition[1]] == 2) {
                    playerLength += 1;
                    grid[headPosition[0]][headPosition[1]] = 1;
                    newFoodPosition();
                }
                else {
                    grid[headPosition[0]][headPosition[1]] = 1;
                    grid[bodyPosition[0]][bodyPosition[1]] = 0;
                    bodyPosition.erase(bodyPosition.begin(), bodyPosition.begin() + 2);
                }
                bodyPosition.push_back(headPosition[0]);
                bodyPosition.push_back(headPosition[1]);

                playerTimer = 0;
            }
        }

        break;

    case GameOver:


        break;
    }

    

    preTime = curTime;
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {

    switch (gameState) {
    case MainMenu:
        if (key == 32)
            startGame();
        break;
    case Game:

        switch (key) {
        case 'w':
            if (playerDirection != 0 && playerDirection != 2)
                playerDirection = 0;
            break;
        case 'd':
            if (playerDirection != 1 && playerDirection != 3)
                playerDirection = 1;
            break;
        case 's':
            if (playerDirection != 2 && playerDirection != 0)
                playerDirection = 2;
            break;
        case 'a':
            if (playerDirection != 3 && playerDirection != 1)
                playerDirection = 3;
            break;
        case 32:
            restart();
            break;
        }
        break;

    case GameOver:
        if (key == 32)
            restart();
        break;
    }

    glutPostRedisplay();
}


void mouseClick(int button, int state, int x, int y) {
    if (gameState == MainMenu)
        buttonStart->click(button, state);
    if (gameState == GameOver)
        buttonTryAgain->click(button, state);
}

void mouseMove(int x, int y) {
    if (gameState == MainMenu)
        buttonStart->checkCollision(x / gridSize, ((gridDimensions[1] * gridSize) - y) / gridSize);
    if (gameState == GameOver)
        buttonTryAgain->checkCollision(x / gridSize, ((gridDimensions[1] * gridSize) - y) / gridSize);
}


void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    string score;

    switch (gameState) {
    case MainMenu:

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Welcome to Snake!", 0.5*gridDimensions[0] - textOffsetX("Welcome to Snake!"), 0.75*gridDimensions[1]);
        drawText("WASD to move", 0.5 * gridDimensions[0] - textOffsetX("WASD to move"), 0.75 * gridDimensions[1] - 1.75);
        drawText("SPACE to restart", 0.5 * gridDimensions[0] - 0.85*textOffsetX("SPACE to restart"), 0.75 * gridDimensions[1] - 2.75);
        drawText("Don't hit yourself. Go for the high score!", 
            0.5 * gridDimensions[0] - 0.85 * textOffsetX("Don't hit yourself. Go for the high score!"), 
            0.75 * gridDimensions[1] - 4.5);
        buttonStart->draw(gridSize);

        break;
    case Game:

        // draw food
        glColor3f(0.0f, 1.0f, 0.0f);
        drawSquare(foodPosition[0], foodPosition[1], 1);

        // draw player
        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < playerLength; i++) {
            drawSquare(bodyPosition.begin()[(i * 2)], bodyPosition.begin()[(i * 2) + 1], 1);
        }

        // draw score
        glColor3f(1.0f, 1.0f, 1.0f);
        score = "Score: " + to_string(playerLength - 1);
        drawText(score, 0.5, (float)gridDimensions[1] - 1);

        break;

    case GameOver:

        // draw food
        glColor3f(0.0f, 1.0f, 0.0f);
        drawSquare(foodPosition[0], foodPosition[1], 1);

        // draw player
        glColor3f(1.0f, 0.0f, 0.0f);
        for (int i = 0; i < playerLength; i++) {
            drawSquare(bodyPosition.begin()[(i * 2)], bodyPosition.begin()[(i * 2) + 1], 1);
        }

        // draw score
        glColor3f(1.0f, 1.0f, 1.0f);
        score = "Score: " + to_string(playerLength - 1);
        drawText(score, 0.5, (float)gridDimensions[1] - 1);

        // draw game over box
        glColor3f(0.0f, 0.0f, 0.0f);
        drawSquare(0.5 * gridDimensions[0] - 5, 0.5 * gridDimensions[1] - 5, 10.5);
        buttonTryAgain->draw(gridSize);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText("Game Over!", 0.5 * gridDimensions[0] - 1.0f * textOffsetX("Game Over!"), 0.65 * gridDimensions[1]);
        score = "Final score: " + to_string(playerLength - 1);
        drawText(score, 0.5 * gridDimensions[0] - 0.8f * textOffsetX("Final score:   "), 0.65 * gridDimensions[1] - 2);

        break;
    }

    glutSwapBuffers();
}


void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, gridDimensions[0], 0, gridDimensions[1]);
    //gluOrtho2D(-1*0.5*gridDimensions[0], 0.5*gridDimensions[0], -1*0.5*gridDimensions[1], 0.5*gridDimensions[1]);

    glViewport(0, 0, (GLsizei)(gridSize*gridDimensions[0]), (GLsizei)(gridSize*gridDimensions[1]));

    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    tryAgain = restart;
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize((int)(gridSize * gridDimensions[0]), (int)(gridSize * gridDimensions[1]));

    glutCreateWindow("Snake");


    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(update);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMove);

    glutMainLoop();

    return 0;
}