// Afia Raisa (41230301862)  and Bishruty Debnath (41230301948)
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Game configuration constants
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const int MOVE_SPEED = 4;
const int COLLISION_DISTANCE = 60;
const int START_TIME = 30;
const int FRAME_RATE = 16;

// Square class
class Square {
private:
    float x1, y1, x2, y2, x3, y3, x4, y4;
    float centerX, centerY;
    float size;

public:
    // Constructor initializes square at bottom-left
    Square() : x1(40), y1(40), x2(40), y2(0),
               x3(0), y3(0), x4(0), y4(40), size(40) {
        updateCenter();
    }

    // Move square by dx, dy
    void move(float dx, float dy) {
        x1 += dx; y1 += dy;
        x2 += dx; y2 += dy;
        x3 += dx; y3 += dy;
        x4 += dx; y4 += dy;
        updateCenter();
    }

    // Update center position of square
    void updateCenter() {
        centerX = (x1 + x2 + x3 + x4) / 4.0f;
        centerY = (y1 + y2 + y3 + y4) / 4.0f;
    }

    // Draw square with ash/gray color
    void draw() const {
        // Ash color (gray)
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_POLYGON);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
            glVertex2f(x4, y4);
        glEnd();

        // Darker outline
        glColor3f(0.3f, 0.3f, 0.3f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
            glVertex2f(x4, y4);
        glEnd();
    }

    float getCenterX() const { return centerX; }
    float getCenterY() const { return centerY; }

    // Keep square within window boundaries
    void clampToWindow() {
        if (x1 < 0 || x2 < 0 || x3 < 0 || x4 < 0) {
            float dx = (x1 < 0) ? -x1 : (x2 < 0) ? -x2 : (x3 < 0) ? -x3 : -x4;
            move(dx, 0);
        }
        if (x1 > WINDOW_WIDTH || x2 > WINDOW_WIDTH ||
            x3 > WINDOW_WIDTH || x4 > WINDOW_WIDTH) {
            float dx = (x1 > WINDOW_WIDTH) ? WINDOW_WIDTH - x1 :
                       (x2 > WINDOW_WIDTH) ? WINDOW_WIDTH - x2 :
                       (x3 > WINDOW_WIDTH) ? WINDOW_WIDTH - x3 : WINDOW_WIDTH - x4;
            move(dx, 0);
        }
        if (y1 < 0 || y2 < 0 || y3 < 0 || y4 < 0) {
            float dy = (y1 < 0) ? -y1 : (y2 < 0) ? -y2 : (y3 < 0) ? -y3 : -y4;
            move(0, dy);
        }
        if (y1 > WINDOW_HEIGHT || y2 > WINDOW_HEIGHT ||
            y3 > WINDOW_HEIGHT || y4 > WINDOW_HEIGHT) {
            float dy = (y1 > WINDOW_HEIGHT) ? WINDOW_HEIGHT - y1 :
                       (y2 > WINDOW_HEIGHT) ? WINDOW_HEIGHT - y2 :
                       (y3 > WINDOW_HEIGHT) ? WINDOW_HEIGHT - y3 : WINDOW_HEIGHT - y4;
            move(0, dy);
        }
    }
};

// Triangle class - controlled smoothly by mouse
class Triangle {
private:
    float x, y;
    float targetX, targetY;
    float size;
    bool visible;
    float smoothSpeed;

public:
    // Constructor initializes triangle at top-center
    Triangle() : x(250), y(400), targetX(250), targetY(400),
                 size(30), visible(true), smoothSpeed(0.15f) {
        srand(time(nullptr));
    }

    // Set target position (controlled by mouse)
    void setTargetPosition(float newX, float newY) {
        targetX = newX;
        targetY = newY;
    }

    void setVisible(bool v) { visible = v; }
    bool isVisible() const { return visible; }

    float getX() const { return x; }
    float getY() const { return y; }

    // Smoothly move triangle towards target position
    void update() {
        if (!visible) return;

        // Calculate distance to target
        float dx = targetX - x;
        float dy = targetY - y;
        float distance = sqrt(dx*dx + dy*dy);

        // Move smoothly towards target
        if (distance > 1.0f) {
            // Use smooth interpolation
            x += dx * smoothSpeed;
            y += dy * smoothSpeed;
        } else {
            // Snap to target if very close
            x = targetX;
            y = targetY;
        }

        // Keep triangle within window boundaries
        if (x < size) x = size;
        if (x > WINDOW_WIDTH - size) x = WINDOW_WIDTH - size;
        if (y < size) y = size;
        if (y > WINDOW_HEIGHT - size) y = WINDOW_HEIGHT - size;
    }

    // Draw triangle with orange color
    void draw() const {
        if (!visible) return;

        glColor3f(1.0f, 0.5f, 0.0f);
        glBegin(GL_TRIANGLES);
            glVertex2f(x, y + size);
            glVertex2f(x - size, y - size);
            glVertex2f(x + size, y - size);
        glEnd();

        glColor3f(0.8f, 0.3f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x, y + size);
            glVertex2f(x - size, y - size);
            glVertex2f(x + size, y - size);
        glEnd();
    }
};

// Main Game class
class Game {
private:
    Square square;
    Triangle triangle;

    bool keys[256];
    bool gameOver;
    int timeLeft;
    int winner;
    float squareSpeed;
    int score;
    int highScore;

    // Check if square collides with triangle
    void checkCollision() {
        float dx = square.getCenterX() - triangle.getX();
        float dy = square.getCenterY() - triangle.getY();
        float distance = sqrt(dx * dx + dy * dy);

        if (distance < COLLISION_DISTANCE) {
            gameOver = true;
            winner = 1;
            triangle.setVisible(false);
            score = timeLeft * 10;
            if (score > highScore) highScore = score;
        }
    }

public:
    // Constructor
    Game() : gameOver(false), timeLeft(START_TIME), winner(0),
             squareSpeed(MOVE_SPEED), score(0), highScore(0) {
        for (int i = 0; i < 256; i++) keys[i] = false;
        srand(time(nullptr));
    }

    // Handle keyboard press
    void handleKeyPress(unsigned char key) {
        if (gameOver) {
            if (key == 'r' || key == 'R') reset();
            return;
        }
        keys[key] = true;
    }

    // Handle keyboard release
    void handleKeyRelease(unsigned char key) {
        keys[key] = false;
    }

    // Handle mouse click - sets triangle target position
    void handleMouseClick(int button, int state, int x, int y) {
        if (gameOver) return;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            int mouseY = WINDOW_HEIGHT - y;
            triangle.setTargetPosition(x, mouseY);
        }
    }

    // Handle mouse motion - smooth tracking
    void handleMouseMotion(int x, int y) {
        if (gameOver) return;
        int mouseY = WINDOW_HEIGHT - y;
        triangle.setTargetPosition(x, mouseY);
    }

    // Update game state
    void update() {
        if (gameOver) return;

        // Move square based on WASD keys
        float dx = 0, dy = 0;
        if (keys['w'] || keys['W']) dy += squareSpeed;
        if (keys['s'] || keys['S']) dy -= squareSpeed;
        if (keys['a'] || keys['A']) dx -= squareSpeed;
        if (keys['d'] || keys['D']) dx += squareSpeed;

        // Normalize diagonal movement
        if (dx != 0 && dy != 0) {
            dx *= 0.7071f;
            dy *= 0.7071f;
        }

        square.move(dx, dy);
        square.clampToWindow();
        triangle.update();
        checkCollision();
    }

    // Update timer
    void updateTimer() {
        if (!gameOver) {
            timeLeft--;
            if (timeLeft <= 0) {
                timeLeft = 0;
                gameOver = true;
                winner = 2;
                score = 100;
                if (score > highScore) highScore = score;
            }
        }
    }

    // Reset game
    void reset() {
        square = Square();
        triangle = Triangle();
        gameOver = false;
        timeLeft = START_TIME;
        winner = 0;
        score = 0;
    }

    // Draw everything
    void draw() const {
        square.draw();
        triangle.draw();
        drawHUD();

        if (gameOver) {
            drawGameOver();
        }
    }

    // Draw heads-up display
    void drawHUD() const {
        // Display timer
        char timerText[50];
        sprintf(timerText, "Time: %d", timeLeft);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(20, WINDOW_HEIGHT - 30);
        for (int i = 0; timerText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timerText[i]);
        }

        // Display score
        char scoreText[50];
        sprintf(scoreText, "Score: %d", score);
        glRasterPos2f(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 30);
        for (int i = 0; scoreText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
        }

        // Display high score
        char highScoreText[50];
        sprintf(highScoreText, "Best: %d", highScore);
        glRasterPos2f(WINDOW_WIDTH - 150, WINDOW_HEIGHT - 55);
        for (int i = 0; highScoreText[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, highScoreText[i]);
        }

        // Display controls hint
        glColor3f(0.6f, 0.6f, 0.6f);
        glRasterPos2f(20, 20);
        const char* controls = "WASD: Move Square | Mouse: Move Triangle Smoothly";
        for (int i = 0; controls[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, controls[i]);
        }
    }

    // Draw game over screen
    void drawGameOver() const {
        // Semi-transparent overlay
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f);
        glBegin(GL_QUADS);
            glVertex2f(0, 0);
            glVertex2f(WINDOW_WIDTH, 0);
            glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
            glVertex2f(0, WINDOW_HEIGHT);
        glEnd();

        char message[50];
        const char* restartMsg = "Press 'R' to Restart";

        if (winner == 1) {
            sprintf(message, "SQUARE WINS! Score: %d", score);
            glColor3f(0.5f, 0.5f, 0.5f);
        } else if (winner == 2) {
            sprintf(message, "TRIANGLE WINS!");
            glColor3f(1.0f, 0.5f, 0.0f);
        } else {
            sprintf(message, "GAME OVER");
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        int msgLen = strlen(message);
        glRasterPos2f(WINDOW_WIDTH/2 - msgLen * 8, WINDOW_HEIGHT/2 + 30);
        for (int i = 0; message[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
        }

        glColor3f(1.0f, 1.0f, 1.0f);
        int restartLen = strlen(restartMsg);
        glRasterPos2f(WINDOW_WIDTH/2 - restartLen * 6, WINDOW_HEIGHT/2 - 20);
        for (int i = 0; restartMsg[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, restartMsg[i]);
        }
    }

    bool isGameOver() const { return gameOver; }
};

// Global game instance
Game* game = nullptr;

// GLUT display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if (game) game->draw();
    glutSwapBuffers();
}

// GLUT keyboard callback
void keyboard(unsigned char key, int x, int y) {
    if (game) game->handleKeyPress(key);
}

// GLUT keyboard release callback
void keyboardUp(unsigned char key, int x, int y) {
    if (game) game->handleKeyRelease(key);
}

// GLUT mouse callback - sets triangle target
void mouse(int button, int state, int x, int y) {
    if (game) game->handleMouseClick(button, state, x, y);
}

// GLUT mouse motion callback - smooth triangle tracking
void mouseMotion(int x, int y) {
    if (game) game->handleMouseMotion(x, y);
}

// GLUT passive mouse motion callback - smooth triangle tracking without clicking
void passiveMouseMotion(int x, int y) {
    if (game) game->handleMouseMotion(x, y);
}

// GLUT timer callback for game update
void update(int value) {
    if (game) {
        game->update();
        glutPostRedisplay();
        glutTimerFunc(FRAME_RATE, update, 0);
    }
}

// GLUT timer callback for timer update
void timerUpdate(int value) {
    if (game) {
        game->updateTimer();
        glutPostRedisplay();
        glutTimerFunc(1000, timerUpdate, 0);
    }
}

// GLUT reshape callback
void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Initialize OpenGL settings
void initOpenGL() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Square and Triangle Game");

    initOpenGL();
    game = new Game();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);           // Track mouse when button is pressed
    glutPassiveMotionFunc(passiveMouseMotion); // Track mouse without clicking

    glutTimerFunc(FRAME_RATE, update, 0);
    glutTimerFunc(1000, timerUpdate, 0);

    glutMainLoop();

    delete game;
    return 0;
}
