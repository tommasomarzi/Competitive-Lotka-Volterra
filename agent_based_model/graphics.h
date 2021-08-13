#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "LV.cpp"
#include <GL/gl.h>
#include <GL/glut.h>

void create_window(void);                   //!< Create the window that will display the graphics.
void reshape(int, int);                     //!< Reshape callback function.
void display(void);                         //!< Display callback function.
void update(int);                           //!< Update the window after evolving the simulation.
void keypress(unsigned char, int, int);     //!< Handle the help menu in the graphical part.

#endif //GRAPHICS_H