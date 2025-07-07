#include<stdio.h>
#include<GL/glut.h>

double xmin=100,ymin=100,xmax=300,ymax=300;
double x1=20,y1=80,x2=420,y2=320; 
const int RIGHT=2;
const int LEFT=1;
const int TOP=8;
const int BOTTOM=4;
int ComputeOutCode(double x, double y);

void CohenSutherland(double x1, double y1, double x2, double y2)
{
    int outcode0, outcode1, outcodeOut;
    bool accept=false, done=false;
    outcode0=ComputeOutCode(x1,y1);
    outcode1=ComputeOutCode(x2,y2);
    double clipped_x1=x1, clipped_y1=y1, clipped_x2=x2, clipped_y2=y2;

    do
    {
        if((outcode0 | outcode1) == 0) 
        {   //completely inside
            accept=true;
            done=true;
        }
        else if((outcode0 & outcode1) != 0) 
            //completely outside
            done=true;
        else
        {
            double x, y;
            outcodeOut=outcode0?outcode0:outcode1;
            if(outcodeOut & TOP)
            {
                x=clipped_x1+(clipped_x2-clipped_x1)*(ymax-clipped_y1)/(clipped_y2-clipped_y1);
                y=ymax;
            }
            else if(outcodeOut & BOTTOM)
            {
                x=clipped_x1+(clipped_x2-clipped_x1)*(ymin-clipped_y1)/(clipped_y2-clipped_y1);
                y=ymin;
            }
            else if(outcodeOut & RIGHT)
            {
                y=clipped_y1+(clipped_y2-clipped_y1)*(xmax-clipped_x1)/(clipped_x2-clipped_x1);
                x=xmax;
            }
            else
            {
                y=clipped_y1+(clipped_y2-clipped_y1)*(xmin-clipped_x1)/(clipped_x2-clipped_x1);
                x=xmin;
            }
            if(outcodeOut==outcode0)
            {
                clipped_x1=x;
                clipped_y1=y;
                outcode0=ComputeOutCode(clipped_x1,clipped_y1);
            }
            else
            {
                clipped_x2=x;
                clipped_y2=y;
                outcode1=ComputeOutCode(clipped_x2,clipped_y2);
            }
        }
    }while(!done);

    if(accept)
    {

        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINES);

            glVertex2d(clipped_x1,clipped_y1);
            glVertex2d(clipped_x2,clipped_y2);
        glEnd();
    }
}

int ComputeOutCode(double x, double y)
{
    int code=0; 
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

        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
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
    CohenSutherland(x1,y1,x2,y2);
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