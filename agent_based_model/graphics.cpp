/*! \file graphics.cpp
    \brief Handle the graphical part of the agent-based model.
*/

#include "graphics.h"


extern LV *simulation;
bool pause = false;


/**
 * It creates a window with size defined by the parameters w_width and w_height of the setup.py, specific
   position and title. 
 */
void create_window(void)
{
	glutInitWindowSize(w_width, w_height);                   //set window size 
	glutInitWindowPosition(0, 0);                            //set window position 
	glutCreateWindow("Competitive Lotka-Volterra");          //set window name
	glClearColor(1, 1, 1, 1);                                //values for the color buffers
}


/**
 * This function will be called by the glutReshapeFunc, which requires as argument: void (*func)(int width, int height).
 * For more information about how glutReshapeFunc works, check the official documentation: https://www.opengl.org/resources/libraries/glut/spec3/node48.html
 * @param width the new width (in pixels) of the windows. By default, this value is set to w_width.
 * @param height the new height (in pixels) of the windows. By default, this value is set to w_height.
 */
void reshape(int width = w_width, int height = w_height) 
{
	glViewport(0, 0, width, height);          	  // transformation from device coords to window coords
	glMatrixMode(GL_PROJECTION);                  // Select The Projection Matrix
	glLoadIdentity();                             // Reset  The Projection Matrix
	gluOrtho2D(w_left, w_right, w_down, w_up);    // corners
	glMatrixMode(GL_MODELVIEW);                   // Select The Modelview Matrix
	glLoadIdentity();                             // Reset  The Modelview Matrix
}


/**
 * This function will be called by the glutDisplayFunc, which requires as argument: void (*func)(void).
 * In particular, it draws each cell of the grid with the associated color. 
 * 
 * For more information about how glutDisplayFunc works, check the official documentation: https://www.opengl.org/resources/libraries/glut/spec3/node46.html
 */
void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear  The Screen And The Depth Buffer
	glLoadIdentity();                                     // Reset  The View
  	double x_scale = (w_right - w_left) / n_rows;
	double y_scale = (w_up - w_down) / n_cols;
	float* species;
	glBegin(GL_QUADS);                                    //delimit the vertices of a primitive (vertex stream)
	for (int x = 0; x < n_rows; ++x) 
	{
		for (int y = 0; y < n_cols; ++y) 
		{
            species = simulation->get_species_color(x, y);
			glColor3f(species[0],species[1],species[2]);
			glVertex2f(w_left + x_scale*x,     w_down + y_scale*y);
			glVertex2f(w_left + x_scale*(x+1), w_down + y_scale*y);
			glVertex2f(w_left + x_scale*(x+1), w_down + y_scale*(y+1));
			glVertex2f(w_left + x_scale*x,     w_down + y_scale*(y+1));
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}


/**
 * Once the grid is updated, it calls the glutPostRedisplay function (https://www.opengl.org/resources/libraries/glut/spec3/node20.html),
   which sets the current window to be redisplayed.
 *
 * The update function is called recursively by glutTimerFunc function (https://www.opengl.org/resources/libraries/glut/spec3/node64.html) and
   therefore the following shape is required: void (*func)(int value).
 * @param value identifier of the callback.
 */
void update(int value) 
{
	glutKeyboardFunc(keypress);
	if(!pause)
	{
		simulation->evolve();
		glutPostRedisplay(); 
	}
	glutTimerFunc(1000 / fps, update, 0);				//registers a timer callback to be triggered in a number of ms
}


/**
 * This function is called by the glutKeyboardFunc (https://www.opengl.org/resources/libraries/glut/spec3/node49.html),
 * which sets the keyboard callback.
 * The following options for the parameter key are available:
 * 
 * 'e': stop the simulation and exit.
 * 
 * 'p': stop/resume the simulation.
 * 
 * 'i': print the current iteration.
 * 
 * 's': print the values of the species w.r.t. their carrying capacities.	
 * 
 * @param key chosen keyboard character.
 * @param x mouse x coordinate (useless in this context but necessary for the glutKeyboardFunc call).
 * @param y mouse y coordinate (useless in this context but necessary for the glutKeyboardFunc call). 
 */
void keypress(unsigned char key, int x, int y) 
{                                                                              
    switch(key) 
	{
    	case 'e':                                                                                                                                       
            exit(0);                                                                                                                                
        case 'p':
            pause = !pause;
        	break;
		case 'i':
			cout<<"Iteration: "<<simulation->get_iter()<<"/"<<ITER_MAX<<endl;
			break;
		case 's':
			simulation->get_stats();
			break;
    } 
}
