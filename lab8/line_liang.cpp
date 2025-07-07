#include<stdio.h>
#include<GL/glut.h>
#include<stdbool.h>

double xmin=100, ymin=100, xmax=300, ymax=300;
double x1=20, y1=80, x2=420, y2=320; 

void LiangBarsky(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double p[4], q[4];
    double u1 = 0.0, u2 = 1.0;
    
    p[0] = -dx;  
    p[1] = dx;   
    p[2] = -dy;  
    p[3] = dy;   
    
    q[0] = x1 - xmin;  
    q[1] = xmax - x1;  
    q[2] = y1 - ymin;  
    q[3] = ymax - y1;  
    
    for(int k = 0; k < 4; k++)
    {
        if(p[k] == 0 && q[k] < 0) 
            return; 
        else
        {
            double r_k = q[k] / p[k];
            
            if(p[k] < 0)
            {
                if(r_k > u1)
                    u1 = r_k;
            }
            else
            {
                if(r_k < u2)
                    u2 = r_k;
            }
            
            if(u1 > u2) 
                return;
        }
    }


    double clipped_x1, clipped_y1, clipped_x2, clipped_y2;

    clipped_x1 = x1 + u1 * dx;
    clipped_y1 = y1 + u1 * dy;
    clipped_x2 = x1 + u2 * dx;
    clipped_y2 = y1 + u2 * dy;

    
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
        glVertex2d(clipped_x1, clipped_y1);
        glVertex2d(clipped_x2, clipped_y2);
    glEnd();
    
    return;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2d(x1, y1);
        glVertex2d(x2, y2);
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();
    
    LiangBarsky(x1, y1, x2, y2);
    
    glFlush();
}

void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Liang-Barsky Line Clipping");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}