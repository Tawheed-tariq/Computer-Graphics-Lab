#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

int lineWidth = 5;
int capStyle = 1; 
const int windowWidth = 800;
const int windowHeight = 600;

void drawLineWithCap(float x1, float y1, float x2, float y2, float width, int style) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx*dx + dy*dy);
    
    // Normalize direction vector
    float nx = dx / length;
    float ny = dy / length;
    
    // Calculate perpendicular vector
    float px = -ny;
    float py = nx;
    
    // Draw the main line (BLUE)
    glColor3f(0.0, 0.0, 1.0); 
    glBegin(GL_QUADS);
        glVertex2f(x1 + px * width/2, y1 + py * width/2);
        glVertex2f(x1 - px * width/2, y1 - py * width/2);
        glVertex2f(x2 - px * width/2, y2 - py * width/2);
        glVertex2f(x2 + px * width/2, y2 + py * width/2);
    glEnd();
    
    // Draw caps
    glColor3f(0.0, 0.0, 1.0); 
    
    if (style == 2) { // Round Cap
        // Left cap (circle)
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x1, y1); // Center
            for (int angle = 90; angle <= 270; angle += 10) {
                float rad = angle * 3.14159 / 180.0;
                glVertex2f(x1 + cos(rad) * width/2, y1 + sin(rad) * width/2);
            }
        glEnd();
        
        // Right cap (circle)
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(x2, y2); // Center
            for (int angle = 0; angle <= 360; angle += 10) {
                float rad = angle * 3.14159 / 180.0;
                glVertex2f(x2 + cos(rad) * width/2, y2 + sin(rad) * width/2);
            }
        glEnd();
    }
    else if (style == 3) { // Projecting Square Cap
        // Left cap
        glBegin(GL_QUADS);
            glVertex2f(x1 + px * width/2, y1 + py * width/2);
            glVertex2f(x1 - px * width/2, y1 - py * width/2);
            glVertex2f(x1 - px * width/2 - nx * width, y1 - py * width/2 - ny * width);
            glVertex2f(x1 + px * width/2 - nx * width, y1 + py * width/2 - ny * width);
        glEnd();
        
        // Right cap
        glBegin(GL_QUADS);
            glVertex2f(x2 + px * width/2, y2 + py * width/2);
            glVertex2f(x2 - px * width/2, y2 - py * width/2);
            glVertex2f(x2 - px * width/2 + nx * width, y2 - py * width/2 + ny * width);
            glVertex2f(x2 + px * width/2 + nx * width, y2 + py * width/2 + ny * width);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Center line coordinates (in window coordinates)
    float x1 = windowWidth * 0.2;
    float y1 = windowHeight * 0.5;
    float x2 = windowWidth * 0.8;
    float y2 = windowHeight * 0.5;
    
    // Draw the line with selected cap style
    drawLineWithCap(x1, y1, x2, y2, lineWidth, capStyle);
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
}

void processMenuEvents(int option) {
    capStyle = option;
    glutPostRedisplay();
}

void processSubMenuEvents(int option) {
    lineWidth = option;
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Get user input for line width
    cout << "Enter the line width (default 5): ";
    cin >> lineWidth;
    
    cout << "Choose Cap Style:\n";
    cout << "1) Butt Cap\n";
    cout << "2) Round Cap\n";
    cout << "3) Projecting Square Cap\n";
    cout << "Enter your choice: ";
    cin >> capStyle;
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Line Cap Styles Demo");
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // Create right-click menu
    int subMenu = glutCreateMenu(processSubMenuEvents);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("5", 5);
    glutAddMenuEntry("10", 10);
    glutAddMenuEntry("15", 15);
    glutAddMenuEntry("20", 20);
    
    int mainMenu = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Butt Cap", 1);
    glutAddMenuEntry("Round Cap", 2);
    glutAddMenuEntry("Projecting Square Cap", 3);
    glutAddSubMenu("Line Width", subMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    init();
    glutMainLoop();
    
    return 0;
}