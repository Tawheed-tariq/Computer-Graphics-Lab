#include <GL/gl.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>  
#include <unistd.h>    

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CLOCK_RADIUS = 0.9;
const float HOUR_HAND_LENGTH = 0.5;
const float MINUTE_HAND_LENGTH = 0.7;
const float SECOND_HAND_LENGTH = 0.8;
const float NUMBER_RADIUS = 0.75;  

int setTimeMode = 0;
int setHour = 3, setMin = 15, setSec = 0;  // default time for manual mode
int useSystemTime = 1;  


int showTimeSettingUI = 0;  
int selectedTimeField = 0;  
int tempHour = 0, tempMin = 0, tempSec = 0;  

struct timeval lastUpdateTime;
long long lastUpdateMillis = 0;

void drawClockHands();
void drawClockFace();
void drawHourMarkers();
void drawNumbers();
void renderBitmapString(float x, float y, void *font, char *string);
void drawDigitalTime(int hours, int minutes, int seconds);
void drawButtons();
void mouseClick(int button, int state, int x, int y);
long long currentTimeMillis();
void drawTimeSettingUI();
void updateTimeValue(int field, int increment);
int isPointInRect(float x, float y, float x1, float y1, float x2, float y2);
void windowToOpenGL(int x, int y, float *ox, float *oy);

long long currentTimeMillis() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

int main(int argc, char** argv) {
    // Initialize the last update time
    lastUpdateMillis = currentTimeMillis();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Real-Time Clock Animation");

    glutDisplayFunc(drawClockHands);
    glutMouseFunc(mouseClick);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutMainLoop();
    
    return 0;
}

void renderBitmapString(float x, float y, void *font, char *string) {
    char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawNumbers() {
    char str[3];
    glColor3f(1.0, 1.0, 1.0);  
    
    for (int i = 1; i <= 12; i++) {
        float angle = 90 - i * 30;  // 90 to start at 12 o'clock, 30 degrees per hour
        float radians = angle * M_PI / 180.0;
        float x = NUMBER_RADIUS * cos(radians);
        float y = NUMBER_RADIUS * sin(radians);
        
        sprintf(str, "%d", i);
        float text_width = glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, '0');
        if (i >= 10) text_width *= 2;  // approx width for two digits
        
        renderBitmapString(x - (text_width/2)/WINDOW_WIDTH, 
                          y - 0.02,  
                          GLUT_BITMAP_HELVETICA_12, str);
    }
}

void drawDigitalTime(int hours, int minutes, int seconds) {
    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds);
    
    glColor3f(1.0, 1.0, 0.7);  
    renderBitmapString(-0.2, -0.3, GLUT_BITMAP_HELVETICA_18, timeStr);
}

void drawClockFace() {
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
    glLineWidth(3.0);
    for (int hour = 0; hour < 12; hour++) {
        float angle = hour * 30 * M_PI / 180.0;  // 30 degrees per hour
        float outer_x = CLOCK_RADIUS * cos(angle);
        float outer_y = CLOCK_RADIUS * sin(angle);
        float inner_x = (CLOCK_RADIUS - 0.1) * cos(angle);
        float inner_y = (CLOCK_RADIUS - 0.1) * sin(angle);
        
        glBegin(GL_LINES);
        glColor3f(1.0, 0.8, 0.2);  
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
            glColor3f(0.7, 0.7, 0.7);  
            glVertex2f(outer_x, outer_y);
            glVertex2f(inner_x, inner_y);
            glEnd();
        }
    }
}

void drawTimeSettingUI() {
    // Only draw if the time setting UI is active
    if (!showTimeSettingUI) return;
    
    glColor3f(0.2, 0.2, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(-0.5, -0.4);
    glVertex2f(0.5, -0.4);
    glVertex2f(0.5, 0.1);
    glVertex2f(-0.5, 0.1);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.3, 0.05, GLUT_BITMAP_HELVETICA_18, "Set Time");
    
    // Draw time fields
    char hourStr[3], minStr[3], secStr[3];
    sprintf(hourStr, "%02d", tempHour);
    sprintf(minStr, "%02d", tempMin);
    sprintf(secStr, "%02d", tempSec);
    
    // Hour
    if (selectedTimeField == 0) {
        glColor3f(0.3, 0.3, 0.6); 
        glBegin(GL_QUADS);
        glVertex2f(-0.35, -0.1);
        glVertex2f(-0.15, -0.1);
        glVertex2f(-0.15, -0.2);
        glVertex2f(-0.35, -0.2);
        glEnd();
    }
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.32, -0.17, GLUT_BITMAP_HELVETICA_18, hourStr);
    
    // Minute
    if (selectedTimeField == 1) {
        glColor3f(0.3, 0.3, 0.6);
        glBegin(GL_QUADS);
        glVertex2f(-0.1, -0.1);
        glVertex2f(0.1, -0.1);
        glVertex2f(0.1, -0.2);
        glVertex2f(-0.1, -0.2);
        glEnd();
    }
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.07, -0.17, GLUT_BITMAP_HELVETICA_18, minStr);
    
    // Second
    if (selectedTimeField == 2) {
        glColor3f(0.3, 0.3, 0.6);
        glBegin(GL_QUADS);
        glVertex2f(0.15, -0.1);
        glVertex2f(0.35, -0.1);
        glVertex2f(0.35, -0.2);
        glVertex2f(0.15, -0.2);
        glEnd();
    }
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(0.18, -0.17, GLUT_BITMAP_HELVETICA_18, secStr);
    
    // Draw separators
    renderBitmapString(-0.125, -0.17, GLUT_BITMAP_HELVETICA_18, ":");
    renderBitmapString(0.125, -0.17, GLUT_BITMAP_HELVETICA_18, ":");
    
    // Hour up arrow
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25, -0.05);
    glVertex2f(-0.3, -0.09);
    glVertex2f(-0.2, -0.09);
    glEnd();
    
    // Hour down arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25, -0.25);
    glVertex2f(-0.3, -0.21);
    glVertex2f(-0.2, -0.21);
    glEnd();
    
    // Minute up arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, -0.05);
    glVertex2f(-0.05, -0.09);
    glVertex2f(0.05, -0.09);
    glEnd();
    
    // Minute down arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, -0.25);
    glVertex2f(-0.05, -0.21);
    glVertex2f(0.05, -0.21);
    glEnd();
    
    // Second up arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.25, -0.05);
    glVertex2f(0.2, -0.09);
    glVertex2f(0.3, -0.09);
    glEnd();
    
    // Second down arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.25, -0.25);
    glVertex2f(0.2, -0.21);
    glVertex2f(0.3, -0.21);
    glEnd();
    
    // Save button
    glColor3f(0.2, 0.5, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(-0.4, -0.35);
    glVertex2f(-0.1, -0.35);
    glVertex2f(-0.1, -0.27);
    glVertex2f(-0.4, -0.27);
    glEnd();
    
    // Cancel button
    glColor3f(0.5, 0.2, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(0.1, -0.35);
    glVertex2f(0.4, -0.35);
    glVertex2f(0.4, -0.27);
    glVertex2f(0.1, -0.27);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.35, -0.32, GLUT_BITMAP_HELVETICA_12, "Save");
    renderBitmapString(0.15, -0.32, GLUT_BITMAP_HELVETICA_12, "Cancel");
}

void updateTimeValue(int field, int increment) {
    switch (field) {
        case 0: // Hour
            tempHour += increment;
            if (tempHour > 23) tempHour = 0;
            if (tempHour < 0) tempHour = 23;
            break;
        case 1: // Minute
            tempMin += increment;
            if (tempMin > 59) tempMin = 0;
            if (tempMin < 0) tempMin = 59;
            break;
        case 2: // Second
            tempSec += increment;
            if (tempSec > 59) tempSec = 0;
            if (tempSec < 0) tempSec = 59;
            break;
    }
}

int isPointInRect(float x, float y, float x1, float y1, float x2, float y2) {
    return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

void drawButtons() {
    // Mode selection label
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.9, -0.7, GLUT_BITMAP_HELVETICA_12, "Clock Mode:");
    
    // System Time button
    if (useSystemTime) {
        glColor3f(0.2, 0.6, 0.2);
    } else {
        glColor3f(0.4, 0.4, 0.4);
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.9, -0.75);
        glVertex2f(-0.6, -0.75);
        glVertex2f(-0.6, -0.85);
        glVertex2f(-0.9, -0.85);
    glEnd();
    
    // Manual Time button
    if (!useSystemTime) {
        glColor3f(0.2, 0.6, 0.2); 
    } else {
        glColor3f(0.4, 0.4, 0.4); 
    }
    glBegin(GL_QUADS);
        glVertex2f(-0.5, -0.75);
        glVertex2f(-0.2, -0.75);
        glVertex2f(-0.2, -0.85);
        glVertex2f(-0.5, -0.85);
    glEnd();
    
    // Set Time button
    glColor3f(0.3, 0.3, 0.7);
    glBegin(GL_QUADS);
        glVertex2f(0.2, -0.75);
        glVertex2f(0.45, -0.75);
        glVertex2f(0.45, -0.85);
        glVertex2f(0.2, -0.85);
    glEnd();
    
    // Reset button
    glColor3f(0.7, 0.3, 0.3);
    glBegin(GL_QUADS);
        glVertex2f(0.55, -0.75);
        glVertex2f(0.8, -0.75);
        glVertex2f(0.8, -0.85);
        glVertex2f(0.55, -0.85);
    glEnd();
    
    glColor3f(1.0, 1.0, 1.0);
    renderBitmapString(-0.85, -0.82, GLUT_BITMAP_HELVETICA_12, "System Time");
    renderBitmapString(-0.45, -0.82, GLUT_BITMAP_HELVETICA_12, "Manual Time");
    renderBitmapString(0.25, -0.82, GLUT_BITMAP_HELVETICA_12, "Set Time");
    renderBitmapString(0.6, -0.82, GLUT_BITMAP_HELVETICA_12, "Reset");
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
    
    // decorative outer ring
    glLineWidth(4.0);
    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i += 5) {
        float angle = i * M_PI / 180.0;
        glVertex2f(0.95 * cos(angle), 0.95 * sin(angle));
    }
    glEnd();
    
    int hours, minutes, seconds;
    
    if (useSystemTime) {
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        hours = timeinfo->tm_hour;
        minutes = timeinfo->tm_min;
        seconds = timeinfo->tm_sec;
    } else {
        // manual time
        long long currentMillis = currentTimeMillis();
        long long elapsedMillis = currentMillis - lastUpdateMillis;
        
        int elapsedSeconds = elapsedMillis / 1000;
        lastUpdateMillis = currentMillis - (elapsedMillis % 1000); 
        
        setSec += elapsedSeconds;
        while (setSec >= 60) {
            setSec -= 60;
            setMin++;
            if (setMin >= 60) {
                setMin = 0;
                setHour = (setHour + 1) % 24;
            }
        }
        
        hours = setHour;
        minutes = setMin;
        seconds = setSec;
    }
    
    drawClockFace();
    drawHourMarkers();
    drawNumbers();
    
    float hourAngle = (90 - ((hours % 12) * 30 + (minutes / 60.0) * 30)) * M_PI / 180.0;
    float minuteAngle = (90 - (minutes * 6 + (seconds / 60.0) * 6)) * M_PI / 180.0;
    float secondAngle = (90 - seconds * 6) * M_PI / 180.0;
    
    // Draw hour hand
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);  
        glVertex2f(0, 0);
        glVertex2f(HOUR_HAND_LENGTH * cos(hourAngle), 
               HOUR_HAND_LENGTH * sin(hourAngle));
    glEnd();

    // Draw minute hand
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);  
        glVertex2f(0, 0);
        glVertex2f(MINUTE_HAND_LENGTH * cos(minuteAngle), 
                MINUTE_HAND_LENGTH * sin(minuteAngle));
    glEnd();

    // Draw second hand
    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.0, 0.7, 1.0);  
        glVertex2f(0, 0);
        glVertex2f(SECOND_HAND_LENGTH * cos(secondAngle), 
                SECOND_HAND_LENGTH * sin(secondAngle));
    glEnd();
    
    // center dot
    glColor3f(1.0, 1.0, 0.0);  
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i += 10) {
        float angle = i * M_PI / 180.0;
        float x = 0.02 * cos(angle);
        float y = 0.02 * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    drawDigitalTime(hours, minutes, seconds);
    
    drawButtons();
    
    if (showTimeSettingUI) {
        drawTimeSettingUI();
    }
}
// reference: https://community.khronos.org/t/how-do-i-convert-mouse-position-cordinates-to-opengl-cordinates/67854
void windowToOpenGL(int x, int y, float *ox, float *oy) {
    *ox = (2.0 * x / WINDOW_WIDTH) - 1.0;
    *oy = 1.0 - (2.0 * y / WINDOW_HEIGHT);
    
    float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    if (WINDOW_WIDTH >= WINDOW_HEIGHT) {
        *ox *= aspect;
    } else {
        *oy /= aspect;
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float ox, oy;
        windowToOpenGL(x, y, &ox, &oy);
        
        // If time setting UI is active, check for clicks on its elements
        if (showTimeSettingUI) {
            // Hour up arrow
            if (isPointInRect(ox, oy, -0.3, -0.09, -0.2, -0.05)) {
                updateTimeValue(0, 1);
                return;
            }
            // Hour down arrow
            if (isPointInRect(ox, oy, -0.3, -0.25, -0.2, -0.21)) {
                updateTimeValue(0, -1);
                return;
            }
            // Minute up arrow
            if (isPointInRect(ox, oy, -0.05, -0.09, 0.05, -0.05)) {
                updateTimeValue(1, 1);
                return;
            }
            // Minute down arrow
            if (isPointInRect(ox, oy, -0.05, -0.25, 0.05, -0.21)) {
                updateTimeValue(1, -1);
                return;
            }
            // Second up arrow
            if (isPointInRect(ox, oy, 0.2, -0.09, 0.3, -0.05)) {
                updateTimeValue(2, 1);
                return;
            }
            // Second down arrow
            if (isPointInRect(ox, oy, 0.2, -0.25, 0.3, -0.21)) {
                updateTimeValue(2, -1);
                return;
            }
            // Hour field selection
            if (isPointInRect(ox, oy, -0.35, -0.2, -0.15, -0.1)) {
                selectedTimeField = 0;
                return;
            }
            // Minute field selection
            if (isPointInRect(ox, oy, -0.1, -0.2, 0.1, -0.1)) {
                selectedTimeField = 1;
                return;
            }
            // Second field selection
            if (isPointInRect(ox, oy, 0.15, -0.2, 0.35, -0.1)) {
                selectedTimeField = 2;
                return;
            }
            // Save button
            if (isPointInRect(ox, oy, -0.4, -0.35, -0.1, -0.27)) {
                setHour = tempHour;
                setMin = tempMin;
                setSec = tempSec;
                useSystemTime = 0; // Switch to manual time mode
                showTimeSettingUI = 0; // Hide the UI
                lastUpdateMillis = currentTimeMillis(); // Reset the timer
                return;
            }
            // Cancel button
            if (isPointInRect(ox, oy, 0.1, -0.35, 0.4, -0.27)) {
                showTimeSettingUI = 0; // Just hide the UI
                return;
            }
        } else {
            // Check for button clicks when time setting UI is not active
            
            // System Time button
            if (isPointInRect(ox, oy, -0.9, -0.85, -0.6, -0.75)) {
                useSystemTime = 1;
                return;
            }
            
            // Manual Time button
            if (isPointInRect(ox, oy, -0.5, -0.85, -0.2, -0.75)) {
                useSystemTime = 0;
                // If switching to manual mode, initialize with current time if not already set
                if (setHour == 0 && setMin == 0 && setSec == 0) {
                    time_t now;
                    struct tm *timeinfo;
                    time(&now);
                    timeinfo = localtime(&now);
                    setHour = timeinfo->tm_hour;
                    setMin = timeinfo->tm_min;
                    setSec = timeinfo->tm_sec;
                }
                lastUpdateMillis = currentTimeMillis(); // Reset the timer
                return;
            }
            
            // Set Time button
            if (isPointInRect(ox, oy, 0.2, -0.85, 0.45, -0.75)) {
                // Show time setting UI
                showTimeSettingUI = 1;
                
                // Initialize with current values
                if (useSystemTime) {
                    time_t now;
                    struct tm *timeinfo;
                    time(&now);
                    timeinfo = localtime(&now);
                    tempHour = timeinfo->tm_hour;
                    tempMin = timeinfo->tm_min;
                    tempSec = timeinfo->tm_sec;
                } else {
                    tempHour = setHour;
                    tempMin = setMin;
                    tempSec = setSec;
                }
                
                selectedTimeField = 0; // Default selected field is hour
                return;
            }
            
            // Reset button
            if (isPointInRect(ox, oy, 0.55, -0.85, 0.8, -0.75)) {
                // Reset to current system time
                time_t now;
                struct tm *timeinfo;
                time(&now);
                timeinfo = localtime(&now);
                setHour = timeinfo->tm_hour;
                setMin = timeinfo->tm_min;
                setSec = timeinfo->tm_sec;
                lastUpdateMillis = currentTimeMillis(); // Reset the timer
                return;
            }
        }
    }
}