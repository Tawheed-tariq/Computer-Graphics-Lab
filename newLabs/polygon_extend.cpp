#include <GL/glut.h>
#include <iostream>
using namespace std;

int width = 640, height = 480;

// Target boundary and fill colors
GLubyte fillColor[3] = {255, 0, 0};      
GLubyte boundaryColor[3] = {0, 0, 0};    

// Utility to get pixel color at (x, y)
void getPixel(int x, int y, GLubyte color[3]) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
}

// Set pixel at (x, y)
void setPixel(int x, int y, GLubyte color[3]) {
    glColor3ubv(color);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

// Recursive boundary fill (4-connected)
void boundaryFill(int x, int y) {
    GLubyte currentColor[3];
    getPixel(x, y, currentColor);

    // Compare current color with fill and boundary
    if ((currentColor[0] != boundaryColor[0] || currentColor[1] != boundaryColor[1] || currentColor[2] != boundaryColor[2]) &&
        (currentColor[0] != fillColor[0] || currentColor[1] != fillColor[1] || currentColor[2] != fillColor[2])) {
        
        setPixel(x, y, fillColor);

        boundaryFill(x + 1, y);
        boundaryFill(x - 1, y);
        boundaryFill(x, y + 1);
        boundaryFill(x, y - 1);
    }
}

// Draw polygon with black boundary
void drawPolygon() {
    glColor3ubv(boundaryColor);
    glBegin(GL_LINE_LOOP);
        glVertex2i(200, 200);
        glVertex2i(300, 200);
        glVertex2i(300, 300);
        glVertex2i(200, 300);
    glEnd();
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawPolygon();
}

void mouse(int button, int state, int x, int y) {
    y = height - y;  // Invert y-coordinate
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        boundaryFill(x, y);
    }
}

void init() {
    glClearColor(1, 1, 1, 1); // White background
    gluOrtho2D(0, width, 0, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutCreateWindow("Boundary Fill - OpenGL");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
