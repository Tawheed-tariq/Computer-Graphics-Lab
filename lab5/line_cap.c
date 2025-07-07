#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

const int WIDTH = 800, HEIGHT = 600;
const int CAP_BUTT = 1, CAP_ROUND = 2, CAP_SQUARE = 3;

int lineWidth = 5;
int capStyle = CAP_BUTT;

void drawCircle(float cx, float cy, float radius, int startAngle = 0, int endAngle = 360) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int angle = startAngle; angle <= endAngle; angle += 10) {
        float rad = angle * M_PI / 180.0;
        glVertex2f(cx + cos(rad) * radius, cy + sin(rad) * radius);
    }
    glEnd();
}

// Draws a line from (x1, y1) to (x2, y2) with the specified cap style and width
void drawLineWithCap(float x1, float y1, float x2, float y2, float width, int cap) {
    float dx = x2 - x1, dy = y2 - y1;
    float len = sqrt(dx * dx + dy * dy);
    float nx = dx / len, ny = dy / len;      // Direction
    float px = -ny, py = nx;                 // Perpendicular

    float hw = width / 2.0f;glClear(GL_COLOR_BUFFER_BIT);
    Line(100, 100, 500, 400); 
    glFlush();
}


    // Main rectangle (blue)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(x1 + px * hw, y1 + py * hw);
        glVertex2f(x1 - px * hw, y1 - py * hw);
        glVertex2f(x2 - px * hw, y2 - py * hw);
        glVertex2f(x2 + px * hw, y2 + py * hw);
    glEnd();

    // Caps
    if (cap == CAP_ROUND) {
        drawCircle(x1, y1, hw, 90, 270); // Left half-circle
        drawCircle(x2, y2, hw);          // Full circle on right
    }
    else if (cap == CAP_SQUARE) {
        glBegin(GL_QUADS); // Left
            glVertex2f(x1 + px * hw, y1 + py * hw);
            glVertex2f(x1 - px * hw, y1 - py * hw);
            glVertex2f(x1 - px * hw - nx * width, y1 - py * hw - ny * width);
            glVertex2f(x1 + px * hw - nx * width, y1 + py * hw - ny * width);
        glEnd();
        glBegin(GL_QUADS); // Right
            glVertex2f(x2 + px * hw, y2 + py * hw);
            glVertex2f(x2 - px * hw, y2 - py * hw);
            glVertex2f(x2 - px * hw + nx * width, y2 - py * hw + ny * width);
            glVertex2f(x2 + px * hw + nx * width, y2 + py * hw + ny * width);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    float x1 = WIDTH * 0.2f, y1 = HEIGHT / 2.0f;
    float x2 = WIDTH * 0.8f, y2 = HEIGHT / 2.0f;

    drawLineWithCap(x1, y1, x2, y2, lineWidth, capStyle);
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv) {
    printf("Line Cap Styles (OpenGL)\n");
    printf("Enter line width (e.g., 5): ");
    scanf("%d", &lineWidth);

    printf("Select cap style:\n");
    printf("1 = Butt\n2 = Round\n3 = Projecting Square\nYour choice: ");
    scanf("%d", &capStyle);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Line Cap Styles");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(1, 1, 1, 1);


    glutMainLoop();
    return 0;
}
