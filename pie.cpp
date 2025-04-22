// g++ pie.cpp -o pie -lGL -lGLU -lglut

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

float percentages[] = {8, 52, 30, 10}; 
float colors[4][3] = { {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0} };  // Red, Blue, Green, Yellow

void drawPieChart() {
    float start_angle = 0.0f;
    float end_angle;
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    for (i = 0; i < 4; i++) {
        end_angle = start_angle + (percentages[i] / 100.0f) * 360.0f;

        glColor3fv(colors[i]);
        
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);  // Center of the pie

            for (float angle = start_angle; angle <= end_angle; angle += 0.1f) {
                float x = cos(angle * PI / 180.0f);
                float y = sin(angle * PI / 180.0f);
                glVertex2f(x, y);
            }
        glEnd();

        start_angle = end_angle;
    }

    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.5, 1.5, -1.5, 1.5);  // Define a 2D orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pie Chart using OpenGL GLUT");

    init();
    glutDisplayFunc(drawPieChart);
    glutMainLoop();

    return 0;
}
