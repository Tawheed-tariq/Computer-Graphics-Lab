#include <GL/glut.h>
#include <iostream>
#include <cmath>

int lineStyle = 1; 

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glColor3f(1.0, 1.0, 1.0);         
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void Line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;
    bool isSteep = dy > dx;

    if (isSteep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(dx, dy);
        std::swap(sx, sy);
    }

    int p = 2 * dy - dx;
    int x = x1, y = y1;

    for (int i = 0; i <= dx; i++) {
        if (lineStyle == 1) { 
            isSteep ? setPixel(y, x) : setPixel(x, y);
        } else if (lineStyle == 2) { 
            if ((i / 10) % 2 == 0) isSteep ? setPixel(y, x) : setPixel(x, y);
        } else if (lineStyle == 3) { 
            if (i % 5 == 0) isSteep ? setPixel(y, x) : setPixel(x, y);
        }

        x += sx;
        if (p < 0) {
            p += 2 * dy;
        } else {
            y += sy;
            p += 2 * (dy - dx);
        }
    }
    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Line(100, 100, 500, 400); 
    glFlush();
}

void getUserChoice() {
    printf("Choose the line style:\n");
    printf("1. Normal\n");
    printf("2. Dashed\n");
    printf("3. Dotted\n");
    printf("Enter choice (1-3): ");
    std::cin >> lineStyle;

    if (lineStyle < 1 || lineStyle > 3) {
        printf("Invalid choice. Defaulting to normal.\n");
        lineStyle = 1;
    }
}

int main(int argc, char** argv) {
    getUserChoice();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Bresenham's Line Algorithm with Line Styles");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
