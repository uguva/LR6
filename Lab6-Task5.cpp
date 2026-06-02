#define NOMINMAX
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>

using namespace std;

int max_depth;
int win_width, win_height;

// Функция расчета цвета заливки
vector<float> getDepthColor(int current_depth, int max_depth) {
    if (max_depth == 0) return { 0.8f, 1.0f, 0.0f }; 
    
    float factor = static_cast<float>(max_depth - current_depth) / max_depth;
    
    float r = 0.9f - 0.9f * factor;
    float g = 1.0f - 0.2f * factor;
    float b = 0.1f + 0.1f * factor;
    
    return { r, g, b };
}

// Отрисовка строго определенного уровня глубины
void drawTreeLevel(int current_depth, int target_depth) {
    if (current_depth < 0) return;

    // Рисуем квадраты тогда, когда дошли до нужного слоя
    if (current_depth == target_depth) {
        vector<float> color = getDepthColor(current_depth, max_depth);

        // 1. Сплошная заливка фрактала
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(1.0f, 0.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(0.0f, 1.0f);
        glEnd();

        glColor3f(0.0f, 0.6f, 0.2f); 
        glLineWidth(1.0f);           
        glBegin(GL_LINE_LOOP);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(1.0f, 0.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(0.0f, 1.0f);
        glEnd();
        
        return;
    }

    float s = 1.0f / sqrt(2.0f);

    // Левая ветка
    glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        glScalef(s, s, 1.0f);
        drawTreeLevel(current_depth - 1, target_depth);
    glPopMatrix();

    // Правая ветка
    glPushMatrix();
        glTranslatef(1.0f, 1.0f, 0.0f);
        glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
        glScalef(s, s, 1.0f);
        glTranslatef(-1.0f, 0.0f, 0.0f);
        drawTreeLevel(current_depth - 1, target_depth);
    glPopMatrix();
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (h == 0) h = 1;
    float aspect = static_cast<float>(w) / static_cast<float>(h);

    float view_height = 5.5f; 
    float view_width = view_height * aspect;

    gluOrtho2D(0.5f - view_width / 2.0f, 0.5f + view_width / 2.0f, -0.5f, -0.5f + view_height);
    
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // Рендерим слои от самых крупных (max_depth) к самым мелким (0).
    for (int d = max_depth; d >= 0; --d) {
        drawTreeLevel(max_depth, d);
    }
    
    glFlush();
}

int main(int argc, char** argv) {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    cout << "Введите глубину рекурсии (например 11): ";
    cin >> max_depth;
    cout << "Введите ширину и высоту окна (например 1280 720): ";
    cin >> win_width >> win_height;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Pythagoras Tree - Perfect Layering");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}