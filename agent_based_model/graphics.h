#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "LV.cpp"
#include <GL/gl.h>
#include <GL/glut.h>

void create_window(void);
void reshape(int, int);
void display(void);
void update(int);
void keypress(unsigned char, int, int);

#endif //GRAPHICS_H