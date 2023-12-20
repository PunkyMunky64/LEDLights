#pragma once
#include "graphics.h"
#include <windows.h> 
#include <GL/glut.h> 
#include <functional>
#include <iostream>

Gfx* Gfx::active_gfx = nullptr;

Gfx::Gfx(int width, int height) {
    winw = width;
    winh = height;
}

void Gfx::set_active() {
    active_gfx = this;
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 165, timer, 0);
}

void Gfx::run_active(int argc, char** argv) {
    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(active_gfx->winw, active_gfx->winh);   // Set the window's initial width & height
    glutInitWindowPosition(active_gfx->x, active_gfx->y);     // Position the window's initial top-left corner
    glutCreateWindow("");                                     // Create a window with the given title
    glutDisplayFunc(display_active);                          // Register display callback handler for window re-paint
    glutTimerFunc(1000/165, timer, 0);
    glutMainLoop();                                           // Enter the infinitely event-processing loop
}

void Gfx::set_display_func(void (Gfx::*display_func)(void)) {
    this->display_func = display_func;
    this->display_func_is_stray_flag = false;
}

void Gfx::set_display_func(void(*func)(void)) {
    this->display_func_stray = func;
    this->display_func_is_stray_flag = true;
}

void Gfx::display() {
    if (display_func_is_stray_flag) {
        display_func_stray();
    } else {
        (this->*display_func)();
    }
}

void Gfx::display_active() {
    active_gfx->display();
}
