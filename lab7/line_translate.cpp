#include<stdio.h>
#include<GL/glut.h>

void drawLine(int x1, int y1, int x2, int y2){
    glBegin(GL_LINES);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
    glEnd();
}


void translate_line(int x1, int y1, int x2, int y2, int tx, int ty){

    float T[3][3] = {
        {1, 0, -tx},
        {0, 1, -ty},
        {0, 0, 1}
    };

    float vertices[3][3] = {
        {x1, x2, 0},
        {y1, y2, 0},
        {1, 1, 1}
    };

    float res[3][3];
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            res[i][j] = 0;
            for(int k = 0; k < 3; k++){
                res[i][j] += T[i][k] * vertices[k][j];
            }
        }
    }

    drawLine(res[0][0], res[1][0], res[0][1], res[1][1]);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);

    drawLine(-100, -50, 100, 50);

    glColor3f(1.0, 1.0, 0.0);
    translate_line(-100, -50, 100, 50, 50, 30);

    glFlush();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Line Rotation");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-200, 200, -200, 200);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}