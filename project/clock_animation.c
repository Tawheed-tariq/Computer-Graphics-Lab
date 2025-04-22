// g++ clock_animation.c -o clock -lGL -lGLU -lglut
// https://labex.io/tutorials/c-creating-a-simple-clock-animation-using-opengl-298829
#include <GL/gl.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CLOCK_RADIUS = 0.9;
const float HOUR_HAND_LENGTH = 0.5;
const float MINUTE_HAND_LENGTH = 0.7;
const float SECOND_HAND_LENGTH = 0.8;

// Function prototypes
void drawClockHands();
void reshape(int w, int h);
void idle();
void drawClockFace();
void drawHourMarkers();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Clock Animation");

    glutDisplayFunc(drawClockHands);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutMainLoop();

    return 0;
}

void drawClockFace() {
    // Draw clock face (circle)
    glLineWidth(2.0);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5) {
        float angle = i * M_PI / 180.0;
        float x = CLOCK_RADIUS * cos(angle);
        float y = CLOCK_RADIUS * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawHourMarkers() {
    // Draw hour markers
    glLineWidth(3.0);
    for (int hour = 0; hour < 12; hour++) {
        float angle = hour * 30 * M_PI / 180.0;  // 30 degrees per hour
        float outer_x = CLOCK_RADIUS * cos(angle);
        float outer_y = CLOCK_RADIUS * sin(angle);
        float inner_x = (CLOCK_RADIUS - 0.1) * cos(angle);
        float inner_y = (CLOCK_RADIUS - 0.1) * sin(angle);
        
        glBegin(GL_LINES);
        glColor3f(1.0, 0.8, 0.2);  // Gold color for hour markers
        glVertex2f(outer_x, outer_y);
        glVertex2f(inner_x, inner_y);
        glEnd();
    }
    
    // Draw minute markers
    glLineWidth(1.0);
    for (int minute = 0; minute < 60; minute++) {
        if (minute % 5 != 0) {  // Skip positions where hour markers are
            float angle = minute * 6 * M_PI / 180.0;  // 6 degrees per minute
            float outer_x = CLOCK_RADIUS * cos(angle);
            float outer_y = CLOCK_RADIUS * sin(angle);
            float inner_x = (CLOCK_RADIUS - 0.05) * cos(angle);
            float inner_y = (CLOCK_RADIUS - 0.05) * sin(angle);
            
            glBegin(GL_LINES);
            glColor3f(0.7, 0.7, 0.7);  // Light gray for minute markers
            glVertex2f(outer_x, outer_y);
            glVertex2f(inner_x, inner_y);
            glEnd();
        }
    }
}

void drawClockHands() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw background
    glBegin(GL_QUADS);
    glColor3f(0.1, 0.1, 0.1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
    
    // Draw a center circle
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * M_PI / 180.0;
        float x = 0.05 * cos(angle);
        float y = 0.05 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    // Draw clock face and markers
    drawClockFace();
    drawHourMarkers();
    
    // Get current time
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    int hours = timeinfo->tm_hour % 12;
    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;
    
    // Calculate angles for clock hands
    // Note: -90 degrees to start at 12 o'clock
    double hourAngle = -90 + (hours * 30) + (minutes * 0.5);  // 30 degrees per hour + adjustment for minutes
    double minuteAngle = -90 + (minutes * 6);  // 6 degrees per minute
    double secondAngle = -90 + (seconds * 6);  // 6 degrees per second

    // Draw hour hand
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);  // Red
    glVertex2f(0, 0);
    glVertex2f(HOUR_HAND_LENGTH * cos(hourAngle * M_PI / 180.0), 
               HOUR_HAND_LENGTH * sin(hourAngle * M_PI / 180.0));
    glEnd();

    // Draw minute hand
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);  // Green
    glVertex2f(0, 0);
    glVertex2f(MINUTE_HAND_LENGTH * cos(minuteAngle * M_PI / 180.0), 
               MINUTE_HAND_LENGTH * sin(minuteAngle * M_PI / 180.0));
    glEnd();

    // Draw second hand
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.7, 1.0);  // Light blue
    glVertex2f(0, 0);
    glVertex2f(SECOND_HAND_LENGTH * cos(secondAngle * M_PI / 180.0), 
               SECOND_HAND_LENGTH * sin(secondAngle * M_PI / 180.0));
    glEnd();
    
    // Draw a small center dot over the hands
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * M_PI / 180.0;
        float x = 0.02 * cos(angle);
        float y = 0.02 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    // Handle window resizing
    float aspectRatio = (float)w / (float)h;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Maintain aspect ratio
    if (w <= h) {
        gluOrtho2D(-1.0, 1.0, -1.0/aspectRatio, 1.0/aspectRatio);
    } else {
        gluOrtho2D(-1.0*aspectRatio, 1.0*aspectRatio, -1.0, 1.0);
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle() {
    // Redraw the scene
    glutPostRedisplay();
}