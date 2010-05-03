#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "hypercube.h"

HyperCube *cube;
float angle;
bool left_down;
int left_x,left_y;
GLfloat old_angle_x,old_angle_y;

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //escape
			exit(0);
			break;
		case 32: //space
			cube->proj_type++;
			glutReshapeWindow(500,600);
			break;
		case 99:
			GLboolean enabled;
			glGetBooleanv(GL_LIGHTING,&enabled);
			if (enabled) glDisable(GL_LIGHTING);
			else glEnable(GL_LIGHTING);
			break;
		case 111: //o
			cube->object_display = !cube->object_display;
			break;
		case 104: //h
			cube->pipe_display = !cube->pipe_display;
			break;
		case 97: //a
			angle += M_PI/20;
            cube->update_projector(angle);
			break;
		case 110: //n
			cube->node_display = !cube->node_display;
			break;
		case 112: //p
			cube->pipe_color++;
			break;
		case 122: //z
			angle -= M_PI/20;
            cube->update_projector(angle);
			break;
		case 101: //e
			cube->pipe_type++;
			break;
		default:
			printf("key=%d\n",key);
			break;
	}
}

void timer(int value) {
	cube->object_angle += 1;
	glutPostRedisplay();	
	glutTimerFunc(10,timer,value+1);
}

void mouse_down(int button, int state, int y, int x) {
	if (button != 0) return;

	if (state == GLUT_DOWN) {
		left_down = true;
		left_x = x;
		left_y = y;
		old_angle_x = cube->angle_x;
		old_angle_y = cube->angle_y;
	} else {
		left_down = false;
		cube->angle_x = old_angle_x + (x-left_x)/3.;
		cube->angle_y = old_angle_y + (y-left_y)/3.;
	}
}

void mouse_move(int y, int x) {
	if (!left_down) return;
	cube->angle_x = old_angle_x + (x-left_x)/3.;
	cube->angle_y = old_angle_y + (y-left_y)/3.;
}

void reshape(int w, int h) { cube->reshape(w,h); }
void render() { cube->render(); }

int main(int argc, char **argv)
{
    cube = new HyperCube();
    angle = 0;
    left_down = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 600);
	glutInitWindowPosition(50,50);
	glutCreateWindow(argv[0]);
	cube->init_opengl();
	glutReshapeFunc(reshape);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse_move);
	glutMouseFunc(mouse_down);
	glutTimerFunc(0,timer,0);
	glutMainLoop();

    delete cube;

	return 0;
}
