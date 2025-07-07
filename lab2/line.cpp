#include <GL/glut.h>
#include <iostream>

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to white
    glColor3f(1.0, 1.0, 1.0); 
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void Line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    double slope = dy/dx;
    int p, x = x1, y = y1;

    if(slope < 1){
        p = 2*dy - dx;
        
        while(x != x2){
            if(p < 0){
                x++;
                p = p + 2*dy;
            }
            else(p >= 0){
                x++;
                y++;
                p = p + 2*(dy - dx);
            }
            setPixel(x, y);
        }
    }else{
        p = 2 * dx - dy;

        while(y != y2){
            if(p < 0){
                y++;
                p = p + 2*dx;
            }
            else(p >= 0){
                x++;
                y++;
                p = p + 2*(dx - dy);
            }
            setPixel(x, y);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Line(100, 100, 500, 400); 
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Bresenham's Line Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}