/*! \file main_abm.cpp
    \brief It contains the main function for the agent-based model. 
*/

#include "graphics.cpp"

LV *simulation;

/**
 * Handle the main loop of the agent-based model.
 *
 * The parameters are needed for the glutInit call (check https://www.opengl.org/resources/libraries/glut/spec3/node10.html)
   even if in this context they are not used.
 * @param argc number of strings in the command line.
 * @param argv array containing the strings in the command line.
 * @return 0 if the simulation was successful.
 */
int main(int argc, char **argv)
{  
	simulation = new LV(n_rows, n_cols);
	simulation->initializer_fill();
	if(ENABLE_GRAPHICS)
	{
		glutInit(&argc, argv);
		create_window();
    	glutReshapeFunc(reshape);             //sets the reshape callback for the current window
		glutDisplayFunc(display);             //sets the display callback for the current window
		update(0);
		glutKeyboardFunc(keypress);
		glutMainLoop();						  //enters the GLUT event processing loop
		
	}
	else
	{
		while(true)
		{
			simulation->evolve();
		}
	}
	return 0;		
}