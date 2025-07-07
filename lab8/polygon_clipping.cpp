#include <GL/glut.h>
#include <vector>
#include <iostream>

#define H 1366
#define W 768

using namespace std;

struct Point {
    float x, y;
};

vector<Point> polygon = {
    {-300, 0}, {-100, 200}, {0, 300}, {100, 0}, {0, -200}, {-200, -100}
};

float xmin = -150, ymin = -100, xmax = 150, ymax = 100;

vector<Point> clippedPolygon;

void myInit() {
    glClearColor(0, 0, 0, 0); 
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-H/2, H/2, -W/2, W/2);
}

Point intersect(Point a, Point b, int edge) {
    Point p;
    float m = (b.x - a.x != 0) ? (b.y - a.y) / (b.x - a.x) : 1e10;
    int x1 = a.x , y1 = a.y;
    switch(edge) {
        case 0: 
            p.x = xmin;
            p.y = y1 + m * (xmin - x1);
            break;
        case 1:
            p.x = xmax;
            p.y = y1 + m * (xmax - x1);
            break;
        case 2: 
            p.y = ymin;
            p.x = (m == 0) ? x1 : x1 + (1 / m) * (ymin - y1);
            break;
        case 3: 
            p.y = ymax;
            p.x = (m == 0) ? x1 : x1 + (1 / m) * (ymax - y1);
            break;
    }

    return p;
}

bool inside(Point p, int edge) {
    switch(edge) {
        case 0: return p.x >= xmin;  
        case 1: return p.x <= xmax;  
        case 2: return p.y >= ymin;  
        case 3: return p.y <= ymax;  
    }
    return false;
}

void sutherlandHodgmanClip() {
    vector<Point> input = polygon;
    for (int edge = 0; edge < 4; ++edge) {
        vector<Point> output;
        int n = input.size();
        for (int i = 0; i < n; i++) {
            Point curr = input[i];
            Point prev = input[(i - 1 + n) % n];
            bool currIn = inside(curr, edge);
            bool prevIn = inside(prev, edge);

            if (currIn && prevIn) { //inside to inside: save current
                output.push_back(curr);
            } else if (!prevIn && currIn) { //outside to inside: save intersection and current
                output.push_back(intersect(prev, curr, edge));
                output.push_back(curr);
            } else if (prevIn && !currIn) { //inside to outside: save intersection
                output.push_back(intersect(prev, curr, edge));
            }
            // else both outside : save nothing
        }
        input = output;
    }
    clippedPolygon = input;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmin, ymax);
        glVertex2f(xmax, ymax);
        glVertex2f(xmax, ymin);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
        for (auto &p : polygon) glVertex2f(p.x, p.y);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
        for (auto &p : clippedPolygon) glVertex2f(p.x, p.y);
    glEnd();

    glFlush();
}

int main(int argc, char **argv) {
    sutherlandHodgmanClip();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1366, 768);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");
    glutDisplayFunc(display);
    myInit();
    glutMainLoop();
    return 0;
}
