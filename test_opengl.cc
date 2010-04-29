#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
//#include <GL/glext.h>
#include <GL/gl.h>
//#include <GL/glu.h>
#endif

GLuint list_base;
GLuint object,pipe1,pipe2,node;

bool left_down;
int left_x,left_y;
GLfloat old_angle_x,old_angle_y;

GLfloat angle_o,angle_y,angle_x,angle_proj;
GLuint mode_proj,mode_type;
bool display_cubes;
bool display_hyper;
bool display_nodes;

int selected_color;

const int lines[16][2] = {
{1,4},
{2,5},
{3,6},
{0,7},

{5,12},
{6,13},
{7,14},
{4,15},

{9,0},
{10,1},
{11,2},
{8,3},

{13,8},
{14,9},
{15,10},
{12,11}
};

const float points[16][4] = {
{1,1,-1,-1},
{-1,1,-1,-1},
{-1,1,1,-1},
{1,1,1,-1},
{1,1,-1,1},
{-1,1,-1,1},
{-1,1,1,1},
{1,1,1,1},

{1,-1,-1,-1},
{-1,-1,-1,-1},
{-1,-1,1,-1},
{1,-1,1,-1},
{1,-1,-1,1},
{-1,-1,-1,1},
{-1,-1,1,1},
{1,-1,1,1}
};

float projector[4][4] = {
{1,0,0,0},
{0,1,0,0},
{0,0,1,0},
{0,0,0,1}
};

float points_proj[16][4];

const float colors[7][3] = {
{207,137,39},
{185,251,81},
{89,172,82},
{97,156,170},
{91,79,89},
{240,38,38},
{250,203,177}
};

void update_projector() {
	for (int i=0; i<4; i++) for (int j=0; j<4; j++) {
		projector[i][j] = 0.;
	}

	projector[0][0] = 1;
	projector[1][1] = 1;
	projector[2][2] = 1;
	projector[3][3] = 1;
	projector[1][1] = cosf(angle_proj);
	projector[3][3] = cosf(angle_proj);
	projector[3][1] = sinf(angle_proj);
	projector[1][3] = -sinf(angle_proj);
}

void update_points_proj() {
	for (int i=0; i<16; i++) {
		for (int j=0; j<4; j++) {
			points_proj[i][j] = 0;
			for (int k=0; k<4; k++) { points_proj[i][j] += projector[j][k]*points[i][k]; }
		}
		points_proj[i][3] += 3;
		points_proj[i][3] /= 4;
		for (int j=0; j<4; j++) {
			points_proj[i][j] /= points_proj[i][3];
		}
	}
}

void init(void)
{
	srand(time(NULL));

	selected_color = 0;
	left_down = false;
	angle_o = 0;
	angle_x = 22;
	angle_y = 30;
	angle_proj = 0;
	display_hyper = true;
	display_cubes = false;
	display_nodes = true;
	mode_proj = 0;
	mode_type = 0;
	update_points_proj();

	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = {0.0, 10.0, 20.0, 0.0};

	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.);

	glFrontFace(GL_CW);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_BLEND);
	//glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); 

	glClearColor(1,1,1,1);

	list_base = glGenLists(4);
	object = list_base;
	pipe1  = list_base+1;
	pipe2  = list_base+2;
	node   = list_base+3;

	glNewList(object, GL_COMPILE);
	glPushMatrix();
	glScalef(1.,2.,1.);
	glTranslatef(0,.5,0);
	glutSolidCube(1);
	glPopMatrix();
	glutSolidIcosahedron();
	glEndList();

	glNewList(pipe2,GL_COMPILE);
	glPushMatrix();
	glScalef(1,.2,.2);
	glTranslatef(.5,0,0);
	glutSolidCube(1);
	glPopMatrix();
	glEndList();
	
	glNewList(pipe1,GL_COMPILE);
	glPushMatrix();
	glScalef(.5,.1,.1);
	glTranslatef(1,0,0);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i=0; i<16; i++) {
		float angle = M_PI*i/8.;
		float ca = cosf(angle);
		float sa = sinf(angle);
		glNormal3f(0,ca,sa);
		glVertex3f(1,ca,sa);
		glVertex3f(-1.,ca,sa);
	}
	glNormal3f(0,1,0);
	glVertex3f(1,1,0);
	glVertex3f(-1,1,0);
	glEnd();
	glPopMatrix();
	glEndList();

	glNewList(node,GL_COMPILE);
	glutSolidSphere(.3,16,16);
	glEndList();
}

void set_material( GLfloat ambr, GLfloat ambg, GLfloat ambb, GLfloat difr, GLfloat difg, GLfloat difb, GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine, GLfloat alpha = 1.)
{
	GLfloat mat[4];
	mat[0] = ambr; mat[1] = ambg; mat[2] = ambb; mat[3] = alpha;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = difr; mat[1] = difg; mat[2] = difb;
	glColor4fv(mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = specr; mat[1] = specg; mat[2] = specb;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);
}

void render_teapot(GLfloat x, GLfloat y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle_o,1,.8,.3333);
	glCallList(object);
	glPopMatrix();
}

GLfloat random_coord(GLfloat radius=.2) {
	return radius * (static_cast<GLfloat>(rand())/RAND_MAX - .5);
}

void set_material_rgb(const float *color) {
	float r = color[0]/255.;
	float g = color[1]/255.;
	float b = color[2]/255.;
	set_material(r,g,b,r,g,b,r,g,b,0.);
}

void render_pipe(const GLfloat *a, const GLfloat *b) {
	GLfloat matrix[16];
	matrix[0]  = b[0]-a[0];
	matrix[1]  = b[1]-a[1];
	matrix[2]  = b[2]-a[2];
	matrix[3]  = 0;
	GLfloat normx = sqrtf( matrix[0]*matrix[0]+matrix[1]*matrix[1]+matrix[2]*matrix[2] );

	if (matrix[0] != 0. || matrix[1] != 0.) {
		GLfloat normy = sqrtf( matrix[0]*matrix[0]+matrix[1]*matrix[1] );
		matrix[4]  = matrix[1]/normy;
		matrix[5]  = -matrix[0]/normy;
		matrix[6]  = 0;
		matrix[7]  = 0;
	} else {
		matrix[4]  = -1;
		matrix[5]  = 0;
		matrix[6]  = 0;
		matrix[7]  = 0;
	}

	matrix[8]  = (matrix[1]*matrix[6]-matrix[2]*matrix[5])/normx;
	matrix[9]  = (matrix[2]*matrix[4]-matrix[0]*matrix[6])/normx;
	matrix[10] = (matrix[0]*matrix[5]-matrix[1]*matrix[4])/normx;
	matrix[11] = 0;

	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;

	glPushMatrix();
	glTranslatef(a[0],a[1],a[2]);
	glMultMatrixf(matrix);
	switch (mode_type) {
	case 0:
		glCallList(pipe1);
		break;
	case 1:
		glCallList(pipe2);
		break;
	case 2:
		glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glEnd();
		break;
	case 3:
		glBegin(GL_LINE_STRIP);
		for (int k=0; k<20; k++) {
			glNormal3f(0,random_coord(),random_coord());
			glVertex3f(0,random_coord(),random_coord());
			glVertex3f(1,random_coord(),random_coord());
		}
		glEnd();
		break;
	case 4:
		glBegin(GL_LINE_STRIP);
		for (int k=0; k<20; k++) {
			glNormal3f(0,random_coord(.3),random_coord(.3));
			glVertex3f(0,random_coord(.3),random_coord(.3));
			glVertex3f(1,random_coord(.3),random_coord(.3));
		}
		glEnd();
		break;
	}
	glPopMatrix();
}

/**
 *  First column:  emerald, jade, obsidian, pearl, ruby, turquoise
 *  2nd column:  brass, bronze, chrome, copper, gold, silver
 *  3rd column:  black, cyan, green, red, white, yellow plastic
 *  4th column:  black, cyan, green, red, white, yellow rubber
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (display_cubes) {
		glPushMatrix();
		glTranslatef(-8,-9.5,-8);
		set_material(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6); render_teapot(2.0, 17.0);
		set_material(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1); render_teapot(2.0, 14.0);
		set_material(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3); render_teapot(2.0, 11.0);
		set_material(0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088); render_teapot(2.0, 8.0);
		set_material(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6); render_teapot(2.0, 5.0); //ruby 
		set_material(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1); render_teapot(2.0, 2.0);
		set_material(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 0.21794872); render_teapot(6.0, 17.0); //brass
		set_material(0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2); render_teapot(6.0, 14.0);
		set_material(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6); render_teapot(6.0, 11.0);
		set_material(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1); render_teapot(6.0, 8.0);
		set_material(0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4); render_teapot(6.0, 5.0);
		set_material(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4); render_teapot(6.0, 2.0);
		set_material(0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, .25); render_teapot(10.0, 17.0);
		set_material(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, .25); render_teapot(10.0, 14.0);
		set_material(0.0, 0.0, 0.0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25); render_teapot(10.0, 11.0);
		set_material(0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.7, 0.6, 0.6, .25); render_teapot(10.0, 8.0);
		set_material(0.0, 0.0, 0.0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, .25); render_teapot(10.0, 5.0);
		set_material(0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.60, 0.60, 0.50, .25); render_teapot(10.0, 2.0);
		set_material(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, .078125); render_teapot(14.0, 17.0);
		set_material(0.0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, .078125); render_teapot(14.0, 14.0);
		set_material(0.0, 0.05, 0.0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, .078125); render_teapot(14.0, 11.0);
		set_material(0.05, 0.0, 0.0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, .078125); render_teapot(14.0, 8.0);
		set_material(0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, .078125); render_teapot(14.0, 5.0);
		set_material(0.05, 0.05, 0.0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, .078125); render_teapot(14.0, 2.0);
		glPopMatrix();
	}

	if (display_hyper) {
		glPushMatrix();

		glTranslatef(0,0,-10);
		glScalef(2,2,2);
		glRotatef(angle_y,1,0,0);
		glRotatef(angle_x,0,1,0);

		//set_material(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 0.21794872);
		//set_material(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, .078125);
		//set_material_rgb(240,38,38);
		set_material_rgb(colors[selected_color]);
		for (int i=0; i<16; i++) for (int j=0; j<2; j++) {
			const GLfloat *a = points_proj[i];
			const GLfloat *b = points_proj[lines[i][j]];
			render_pipe(a,b);
		}

		if (display_nodes) {
			set_material(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
			for (int i=0; i<16; i++) {
				const GLfloat *a = points_proj[i];
				glPushMatrix();
				glTranslatef(a[0],a[1],a[2]);
				glCallList(node);
				glPopMatrix();
			}
		}

		glPopMatrix();
	}

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (mode_proj) {
	case 0:
		if (w <= h) glOrtho(-8.0, 8.0, -8.0*(GLfloat)h/(GLfloat)w, 8.0*(GLfloat)h/(GLfloat)w, 0, 20);
		else glOrtho(-8.0*(GLfloat)w/(GLfloat)h, 8.0*(GLfloat)w/(GLfloat)h, -8.0, 8.0, 0, 20);
		break;
	case 1:
		if (w <= h) glFrustum(-1.0, 1.0, -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w, 1.0, 20.0);
		else glFrustum(-1.0*(GLfloat)w/(GLfloat)h, 1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0, 1.0, 20.0);
		break;
	default:
		printf("unknown mode\n");
		break;
	}
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27: //escape
			exit(0);
			break;
		case 32: //space
			mode_proj++;
			mode_proj %= 2;
			glutReshapeWindow(500,600);
			break;
		case 99:
			GLboolean enabled;
			glGetBooleanv(GL_LIGHTING,&enabled);
			if (enabled) glDisable(GL_LIGHTING);
			else glEnable(GL_LIGHTING);
			break;
		case 111: //o
			display_cubes = !display_cubes;
			break;
		case 104: //h
			display_hyper = !display_hyper;
			break;
		case 97: //a
			angle_proj += M_PI/20;
			update_projector();
			update_points_proj();
			break;
		case 110: //n
			display_nodes = !display_nodes;
			break;
		case 112: //p
			selected_color++;
			selected_color %= 7;
			break;
		case 122: //z
			angle_proj -= M_PI/20;
			update_projector();
			update_points_proj();
			break;
		case 101: //e
			mode_type++;
			mode_type %= 5;
			break;
		default:
			printf("key=%d\n",key);
			break;
	}
}

void timer(int value)
{
	angle_o += 1;
	glutPostRedisplay();	
	glutTimerFunc(10,timer,value+1);
}

void mouse_down(int button, int state, int x, int y) {
	if (button != 0) return;

	if (state == GLUT_DOWN) {
		left_down = true;
		left_x = x;
		left_y = y;
		old_angle_x = angle_x;
		old_angle_y = angle_y;
	} else {
		left_down = false;
		angle_x = old_angle_x + (x-left_x)/3.;
		angle_y = old_angle_y + (y-left_y)/3.;
	}
}

void mouse_move(int x, int y) {
	if (!left_down) return;
	angle_x = old_angle_x + (x-left_x)/3.;
	angle_y = old_angle_y + (y-left_y)/3.;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 600);
	glutInitWindowPosition(50,50);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse_move);
	glutMouseFunc(mouse_down);
	glutTimerFunc(0,timer,0);
	glutMainLoop();
	return 0;
}
