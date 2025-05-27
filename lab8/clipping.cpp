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

// Clipping rectangle
float xmin = -150, ymin = -100, xmax = 150, ymax = 100;

// Clipped polygon result
vector<Point> clippedPolygon;

void myInit() {
    glClearColor(0, 0, 0, 0); // Black background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-H/2, H/2, -W/2, W/2);
}

// Helper function to compute intersection of edge with clip boundary
Point intersect(Point a, Point b, int edge) {
    Point p;
    float m = (b.x - a.x != 0) ? (b.y - a.y) / (b.x - a.x) : 1e10;

    switch(edge) {
        case 0: // Left
            p.x = xmin;
            p.y = a.y + m * (xmin - a.x);
            break;
        case 1: // Right
            p.x = xmax;
            p.y = a.y + m * (xmax - a.x);
            break;
        case 2: // Bottom
            p.y = ymin;
            p.x = (m == 0) ? a.x : a.x + (1 / m) * (ymin - a.y);
            break;
        case 3: // Top
            p.y = ymax;
            p.x = (m == 0) ? a.x : a.x + (1 / m) * (ymax - a.y);
            break;
    }

    return p;
}

bool inside(Point p, int edge) {
    switch(edge) {
        case 0: return p.x >= xmin;  // Left
        case 1: return p.x <= xmax;  // Right
        case 2: return p.y >= ymin;  // Bottom
        case 3: return p.y <= ymax;  // Top
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

            if (currIn && prevIn) {
                output.push_back(curr);
            } else if (!prevIn && currIn) {
                output.push_back(intersect(prev, curr, edge));
                output.push_back(curr);
            } else if (prevIn && !currIn) {
                output.push_back(intersect(prev, curr, edge));
            }
            // else both outside: do nothing
        }
        input = output;
    }
    clippedPolygon = input;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping rectangle in red
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmin, ymax);
    glVertex2f(xmax, ymax);
    glVertex2f(xmax, ymin);
    glEnd();

    // Draw original polygon in cyan
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : polygon) glVertex2f(p.x, p.y);
    glEnd();

    // Draw clipped polygon in white
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
