//rotate traingle 70 degrees anticlockwise with resect to any of th eedge

#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.14

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void rotateTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float pivotX, float pivotY, float angle) {
    float rad = angle * PI / 180.0;
    float cosA = cos(rad);
    float sinA = sin(rad);
    
    float T1[3][3] = {
        {1, 0, -pivotX},
        {0, 1, -pivotY},
        {0, 0, 1}
    };
    
    float R[3][3] = {
        {cosA, -sinA, 0},
        {sinA, cosA, 0},
        {0, 0, 1}
    };
    
    float T2[3][3] = {
        {1, 0, pivotX},
        {0, 1, pivotY},
        {0, 0, 1}
    };
    
    float transform[3][3];
    
    float temp[3][3];
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            temp[i][j] = 0;
            for(int k = 0; k < 3; k++) {
                temp[i][j] += R[i][k] * T1[k][j];
            }
        }
    }
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            transform[i][j] = 0;
            for(int k = 0; k < 3; k++) {
                transform[i][j] += T2[i][k] * temp[k][j];
            }
        }
    }
    
    float vertices[3][3] = {
        {x1, x2, x3}, 
        {y1, y2, y3}, 
        {1, 1, 1}
    };
    float newVertices[3][3];
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            newVertices[i][j] = 0;
            for(int k = 0; k < 3; k++) {

                newVertices[i][j] += transform[i][k] * vertices[k][j];
                
            } 
        }
    }
    
    drawTriangle(newVertices[0][0], newVertices[1][0], 
                newVertices[0][1], newVertices[1][1], 
                newVertices[0][2], newVertices[1][2]);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);
    drawTriangle(100.0, 100.0, 200.0, 100.0, 150.0, 200.0);
    
    glColor3f(1.0, 1.0, 0.0); 
    rotateTriangle(100.0, 100.0, 200.0, 100.0, 150.0, 200.0, 150.0, 200.0, 70.0);
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1500, 1500);
    glutCreateWindow("Rotate Triangle");
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    
    glutDisplayFunc(display);
    glutMainLoop();
    
    return 0;
}