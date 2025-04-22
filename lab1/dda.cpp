#include <GL/glut.h>
#include <iostream>
using namespace std;

void drawLineDDA(float x1, float y1, float x2, float y2, float r, float g, float b) {
    float dx = x2 - x1, dy = y2 - y1, steps;
    steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;

    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2i(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
    glFlush();
}

void drawLabel(float x, float y, const char* label) {
    glRasterPos2f(x, y);
    for (const char* c = label; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawLineDDA(50, 50, 200, 250, 1.0, 0.0, 0.2); 
    drawLabel(50, 50, "A");
    drawLabel(200, 250, "B");

    drawLineDDA(100, 50, 200, 100, 0.5, 1.0, 0.3); 
    drawLabel(100, 50, "C");
    drawLabel(200, 100, "D");

    drawLabel(100, 200, "S");
    drawLabel(200, 300, "T");
    drawLineDDA(100, 200, 200, 300, 0.1, 0.6, 0.0); 
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("DDA Line Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}