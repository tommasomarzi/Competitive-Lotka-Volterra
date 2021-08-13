#include "graphics.cpp"

LV *simulation;

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