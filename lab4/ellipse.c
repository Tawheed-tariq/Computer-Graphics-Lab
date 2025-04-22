#include <GL/glut.h>
#include <stdio.h>

float color[3] = {1.0, 1.0, 1.0}; 

void plotPoints(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glVertex2i(-x, y);
    glVertex2i(x, -y);
    glVertex2i(-x, -y);
    glEnd();
}

void ellipse(int rx, int ry) {
    int x = 0, y = ry;
    float dx, dy, d1, d2;
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // Region 1
    d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    while (dx < dy) {
        plotPoints(x, y);
        x++;
        dx = dx + (2 * ry * ry);
        if (d1 < 0) {
            d1 = d1 + dx + (ry * ry);
        } else {
            y--;
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }

    // Region 2
    d2 = ((float)(ry * ry) * (x + 0.5) * (x + 0.5)) + ((float)(rx * rx) * (y - 1) * (y - 1)) - ((float)(rx * rx * ry * ry));
    while (y >= 0) {
        plotPoints(x, y);
        y--;
        dy = dy - (2 * rx * rx);
        if (d2 > 0) {
            d2 = d2 + (rx * rx) - dy;
        } else {
            x++;
            dx = dx + (2 * ry * ry);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }

    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(color[0], color[1], color[2]);
    int rx = 100, ry = 60;
    ellipse(rx, ry);
    glFlush();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
}

void askColor() {
    int choice;
    printf("Choose the color of the ellipse:\n");
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
    glutCreateWindow("Midpoint Ellipse Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
