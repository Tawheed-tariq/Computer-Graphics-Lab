#include <GL/glut.h>
#include <stdio.h>

float color[3] = {1.0, 1.0, 1.0}; 

void drawCircle(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
    glEnd();
}

void bresenhamCircle(int xc, int yc, int r) {
    int x = 0, y = r;
    int p = 5/4 -r;
    drawCircle(xc, yc, x, y);
    while (y >= x) {
        x++;
        if (p >= 0) {
            y--;
            p = p + 2 * (x - y) + 1;
        } else {
            p = p + 2 * x + 1;
        }
        drawCircle(xc, yc, x, y);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(color[0], color[1], color[2]);
    int xc = 250, yc = 250, r = 100;
    bresenhamCircle(xc, yc, r);
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

void askColor() {
    int choice;
    printf("Choose the color of the circumference:\n");
    printf("1. Red\n");
    printf("2. Green\n");
    printf("3. Blue\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            color[0] = 1.0; color[1] = 0.0; color[2] = 0.0;
            break;
        case 2:
            color[0] = 0.0; color[1] = 1.0; color[2] = 0.0;
            break;
        case 3:
            color[0] = 0.0; color[1] = 0.0; color[2] = 1.0;
            break;
        default:
            printf("Invalid choice. Defaulting to white.\n");
            color[0] = 1.0; color[1] = 1.0; color[2] = 1.0;
            break;
    }
}

int main(int argc, char** argv) {
    askColor();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Circle");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}