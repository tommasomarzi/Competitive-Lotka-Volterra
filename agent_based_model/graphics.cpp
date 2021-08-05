#include "graphics.h"

extern LV *simulation;
bool pause = false;

void create_window(void)
{
	glutInitWindowSize(w_width, w_height);                   //set window size 
	glutInitWindowPosition(0, 0);                            //set window position 
	glutCreateWindow("Competitive Lotka-Volterra");          //set window name
	glClearColor(1, 1, 1, 1);                                //values for the color buffers
}

void reshape(int width, int height) 
{
	glViewport(0, 0, w_width, w_height);          //transformation from device coords to window coords
	glMatrixMode(GL_PROJECTION);                  // Select The Projection Matrix
	glLoadIdentity();                             // Reset  The Projection Matrix
	gluOrtho2D(w_left, w_right, w_down, w_up);    //corners
	glMatrixMode(GL_MODELVIEW);                   // Select The Modelview Matrix
	glLoadIdentity();                             // Reset  The Modelview Matrix
	glutPostRedisplay();                          //marks the current window as needing to be redisplayed
}

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


void update(int value) 
{
	glutKeyboardFunc(keypress);
	if(!pause)
	{
		simulation->evolve();
		glutPostRedisplay();							//marks the current window to be redisplayed
	}
	glutTimerFunc(1000 / fps, update, 0);				//registers a timer callback to be triggered in a number of ms
}

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
