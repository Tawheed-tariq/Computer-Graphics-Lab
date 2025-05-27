#include<stdio.h>
#include<GL/glut.h>
#define outcode int

double xmin=100,ymin=100,xmax=300,ymax=300;
double xvmin=200,yvmin=200,xvmax=300,yvmax=300;
double x0=20,y0=80,x1=420,y1=320; // Predefined line points
const int RIGHT=8;
const int LEFT=2;
const int TOP=4;
const int BOTTOM=1;
outcode ComputeOutCode(double x, double y);

void CohenSutherland(double x0, double y0, double x1, double y1)
{
    outcode outcode0, outcode1, outcodeOut;
    bool accept=false, done=false;
    outcode0=ComputeOutCode(x0,y0);
    outcode1=ComputeOutCode(x1,y1);
    double clipped_x0=x0, clipped_y0=y0, clipped_x1=x1, clipped_y1=y1;
    
    do
    {
        if(!(outcode0 | outcode1))
        {
            accept=true;
            done=true;
        }
        else if(outcode0 & outcode1)
            done=true;
        else
        {
            double x, y;
            outcodeOut=outcode0?outcode0:outcode1;
            if(outcodeOut & TOP)
            {
                x=clipped_x0+(clipped_x1-clipped_x0)*(ymax-clipped_y0)/(clipped_y1-clipped_y0);
                y=ymax;
            }
            else if(outcodeOut & BOTTOM)
            {
                x=clipped_x0+(clipped_x1-clipped_x0)*(ymin-clipped_y0)/(clipped_y1-clipped_y0);
                y=ymin;
            }
            else if(outcodeOut & RIGHT)
            {
                y=clipped_y0+(clipped_y1-clipped_y0)*(xmax-clipped_x0)/(clipped_x1-clipped_x0);
                x=xmax;
            }
            else
            {
                y=clipped_y0+(clipped_y1-clipped_y0)*(xmin-clipped_x0)/(clipped_x1-clipped_x0);
                x=xmin;
            }
            if(outcodeOut==outcode0)
            {
                clipped_x0=x;
                clipped_y0=y;
                outcode0=ComputeOutCode(clipped_x0,clipped_y0);
            }
            else
            {
                clipped_x1=x;
                clipped_y1=y;
                outcode1=ComputeOutCode(clipped_x1,clipped_y1);
            }
        }
    }while(!done);
    
    if(accept)
    {
        // Draw clipped line in white
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINES);
            glVertex2d(clipped_x0,clipped_y0);
            glVertex2d(clipped_x1,clipped_y1);
        glEnd();
    }
}

outcode ComputeOutCode(double x, double y)
{
    outcode code=0;
    if(y > ymax)
        code = TOP;
    else if(y < ymin)
        code = BOTTOM;
    if(x > xmax)
        code = RIGHT;
    else if(x < xmin)
        code = LEFT;
    return code;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw original line in green
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
        glVertex2d(x0,y0);
        glVertex2d(x1,y1);
    glEnd();
    
    // Draw clipping window in red
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
    glEnd();
    
    // Draw clipped line
    CohenSutherland(x0,y0,x1,y1);
    glFlush();
}

void myinit()
{
    glClearColor(0.0,0.0,0.0,1.0); // Black background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,499.0,0.0,499.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Cohen-Sutherland Line Clipping");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}