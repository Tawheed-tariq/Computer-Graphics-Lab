#include <GL/glut.h>
#include <vector>
#include <iostream>

struct Point {
    float x, y;
};

std::vector<Point> polygon = {
    {0.0, 0.0},
    {1.0, 0.0},
    {0.5, 1.0},
    {-0.5, 1.5},
    {-0.5, 0.5}
};

bool isConvex(const std::vector<Point>& points) {
    int n = points.size();
    if (n < 3) return false;

    float crossProductZ = 0;
    bool gotFirst = false;

    for (int i = 0; i < n; ++i) {
        Point A = points[i];
        Point B = points[(i + 1) % n];
        Point C = points[(i + 2) % n];

        float dx1 = B.x - A.x;
        float dy1 = B.y - A.y;
        float dx2 = C.x - B.x;
        float dy2 = C.y - B.y;

        float z = dx1 * dy2 - dy1 * dx2; // 2D cross product z-component

        if (!gotFirst) {
            crossProductZ = z;
            gotFirst = true;
        } else if (z * crossProductZ < 0) {
            return false; // Sign change ⇒ not convex
        }
    }

    return true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (const auto& pt : polygon)
        glVertex2f(pt.x, pt.y);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    std::cout << "Polygon is " << (isConvex(polygon) ? "Convex" : "Concave") << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Polygon Display");
    glutDisplayFunc(display);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glutMainLoop();

    return 0;
}
