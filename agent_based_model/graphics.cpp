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
	gluOrtho2D(left, right, down, up);            //corners
	glMatrixMode(GL_MODELVIEW);                   // Select The Modelview Matrix
	glLoadIdentity();                             // Reset  The Modelview Matrix
	glutPostRedisplay();                          //marks the current window as needing to be redisplayed
}

void display(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Clear  The Screen And The Depth Buffer
	glLoadIdentity();                                     // Reset  The View
  	double x_scale = (right - left) / n_rows;
	double y_scale = (up - down) / n_cols;
	float* specie;
	glBegin(GL_QUADS);                                    //delimit the vertices of a primitive (vertex stream)
	for (int x = 0; x < n_rows; ++x) 
	{
		for (int y = 0; y < n_cols; ++y) 
		{
            specie = simulation->get_specie_color(x, y);
			glColor3f(specie[0],specie[1],specie[2]);
			glVertex2f(    x*x_scale + left,    y*y_scale + down);
			glVertex2f((x+1)*x_scale + left,    y*y_scale + down);
			glVertex2f((x+1)*x_scale + left,(y+1)*y_scale + down);
			glVertex2f(    x*x_scale + left,(y+1)*y_scale + down);
		}
	}
	glEnd();
	glFlush();
	glutSwapBuffers();
}


void update(int value) 
{
	glutKeyboardFunc(keypress);
	if(pause)
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
			std::cout<<"Iteration: "<<simulation->get_iter()<<"/"<<ITER_MAX<<std::endl;
    } 
}
