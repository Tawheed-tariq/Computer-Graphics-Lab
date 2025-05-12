#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    float x, y;
};

std::vector<Point> polygon = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.7f, 0.5f},
    {1.0f, 1.0f},
    {0.0f, 1.0f}
};

float dotProduct(Point p1, Point p2, Point p3) {
    float ax = p2.x - p1.x, ay = p2.y - p1.y;
    float bx = p2.x - p3.x, by = p2.y - p1.y;
    return ax * bx + ay * by;
}

float magnitude(Point p1, Point p2) {
    float ax = p2.x - p1.x, ay = p2.y - p1.y;
    return std::sqrt(ax*ax + ay*ay);
}

float angleBetween(Point a, Point b, Point c) {
    float dot_ab = dotProduct(a, b, c);
    float mag_a = magnitude(a, b);
    float mag_b = magnitude(c, b);
    float cosTheta = dot_ab / (mag_a * mag_b);
    if (cosTheta < -1.0f) cosTheta = -1.0f;
    if (cosTheta > 1.0f) cosTheta = 1.0f;
    return std::acos(cosTheta) * (180.0f / M_PI);  
}

bool isConvexByAngles(const std::vector<Point>& poly) {
    int n = poly.size();
    if (n < 4) return true;  

    for (int i = 0; i < n; i++) {
        Point prev = poly[(i - 1 + n) % n];
        Point curr = poly[i];
        Point next = poly[(i + 1) % n];

        float angle = angleBetween(prev, curr, next);
        if (angle >= 180.0f) return false;  // concave if angle ≥ 180°
    }
    return true;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw polygon
    glColor3f(0.2f, 0.5f, 0.9f);
    glBegin(GL_POLYGON);
    for (auto& p : polygon)
        glVertex2f(p.x, p.y);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    if (isConvexByAngles(polygon))
        std::cout << "The polygon is CONVEX (by angle method)." << std::endl;
    else
        std::cout << "The polygon is CONCAVE (by angle method)." << std::endl;

    // OpenGL setup
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Polygon Angle Check");
    gluOrtho2D(-1, 2, -1, 2);
    glClearColor(1, 1, 1, 1);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
