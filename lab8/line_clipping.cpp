#include <GL/glut.h>
#include <iostream>
using namespace std;

int windowHeight = 600;

void setPixel(int x, int y, float fillColor[3]) {
    glColor3fv(fillColor);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float color[3]) {
    glReadPixels(x, windowHeight - y, 1, 1, GL_RGB, GL_FLOAT, color);
}

bool isSameColor(float color1[3], float color2[3]) {
    for (int i = 0; i < 3; ++i)
        if (abs(color1[i] - color2[i]) > 0.01) return false;
    return true;
}

void floodFill(int x, int y, float oldColor[3], float newColor[3]) {
    float color[3];
    getPixelColor(x, y, color);

    if (isSameColor(color, oldColor)) {
        setPixel(x, y, newColor);
        floodFill(x + 1, y, oldColor, newColor);
        floodFill(x - 1, y, oldColor, newColor);
        floodFill(x, y + 1, oldColor, newColor);
        floodFill(x, y - 1, oldColor, newColor);
    }
}

void drawPolygonWithColoredEdges() {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0); // Red
    glVertex2i(200, 200); glVertex2i(300, 200);

    glColor3f(0, 1, 0); // Green
    glVertex2i(300, 200); glVertex2i(275, 300);

    glColor3f(0, 0, 1); // Blue
    glVertex2i(275, 300); glVertex2i(225, 300);

    glColor3f(1, 1, 0); // Yellow
    glVertex2i(225, 300); glVertex2i(200, 200);

    glEnd();
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPolygonWithColoredEdges();

    glFlush();
    glFinish(); // Ensure drawing is complete

    float oldColor[3];
    getPixelColor(250, 250, oldColor);  // Get background color
    float fillColor[3] = {1.0f, 0.0f, 1.0f};  // Magenta fill

    floodFill(250, 250, oldColor, fillColor);
}

void init() {
    glClearColor(0, 0, 0, 1); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500); // Coordinate system
    glPointSize(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowHeight, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Flood Fill with Multi-Colored Edges");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
