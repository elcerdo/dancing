/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  teapots.c
 *  This program demonstrates lots of material properties.
 *  A single light source illuminates the objects.
 */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

GLuint lists;
GLuint pipe;
GLfloat angle,angle_proj;
GLuint mode;
bool display_cubes;
bool display_hyper;

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

void update_projector() {
	for (int i=0; i<4; i++) for (int j=0; j<4; j++) {
		projector[i][j] = 0.;
	}

	projector[0][0] = 1;
	projector[1][1] = 1;
	projector[2][2] = 1;
	projector[3][3] = 1;
	projector[2][2] = cosf(angle_proj);
	projector[3][3] = cosf(angle_proj);
	projector[3][2] = sinf(angle_proj);
	projector[2][3] = -sinf(angle_proj);
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

/*
 * Initialize depth buffer, projection matrix, light source, and lighting
 * model.  Do not specify a material property here.
 */
void init(void)
{
	angle = 0;
	angle_proj = 0;
	display_hyper = true;
	display_cubes = true;
	update_points_proj();

	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = {5.0, 5.0, 8.0, 0.0};

	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.);

	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); 

	glClearColor(1,1,1,1);

	lists = glGenLists(1);
	pipe = lists+1;

	glNewList(lists, GL_COMPILE);
	glPushMatrix();
	glScalef(1.,2.,1.);
	glTranslatef(0,.5,0);
	glutSolidCube(1);
	glPopMatrix();
	glutSolidIcosahedron();
	glEndList();

	glNewList(pipe,GL_COMPILE);
	glPushMatrix();
	glScalef(.5,.2,.2);
	glTranslatef(-1,0,0);
	glBegin(GL_TRIANGLE_STRIP);
	for (int i=0; i<16; i++) {
		float angle = M_PI*i/8.;
		float ca = cosf(angle);
		float sa = sinf(angle);
		glNormal3f(0,sa,ca);
		glVertex3f(-1,ca,sa);
		glNormal3f(0,sa,ca);
		glVertex3f(1.,ca,sa);
	}
	glNormal3f(0,0,1);
	glVertex3f(-1,1,0);
	glNormal3f(0,0,1);
	glVertex3f(1,1,0);
	glEnd();
	glPopMatrix();
	glEndList();

}

/*
 * Move object into position.  Use 3rd through 12th 
 * parameters to specify the material property.  Draw a teapot.
 */
void renderTeapot(GLfloat x, GLfloat y,
		GLfloat ambr, GLfloat ambg, GLfloat ambb,
		GLfloat difr, GLfloat difg, GLfloat difb,
		GLfloat specr, GLfloat specg, GLfloat specb, GLfloat shine)
{
	GLfloat mat[4];

	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(angle,1,.8,.3333);
	mat[0] = ambr; mat[1] = ambg; mat[2] = ambb; mat[3] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = difr; mat[1] = difg; mat[2] = difb;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = specr; mat[1] = specg; mat[2] = specb;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0);
	glCallList(lists);
	glCallList(pipe);
	glPopMatrix();
}

void render_pipe(const GLfloat *a, const GLfloat *b) {
	GLfloat matrix[16];
	for (int i=0; i<16; i++) { matrix[i] = 0; }
	matrix[0]  = a[0]-b[0];
	matrix[1]  = a[1]-b[1];
	matrix[2]  = a[2]-b[2];
	matrix[3]  = 0;

	GLfloat norm = sqrtf( matrix[0]*matrix[0]+matrix[1]*matrix[1]+matrix[2]*matrix[2] );
	matrix[4]  = -matrix[1]/norm;
	matrix[5]  = matrix[0]/norm;
	matrix[6]  = matrix[2]/norm;
	matrix[7]  = 0;
	
	matrix[8]  = (matrix[1]*matrix[6]-matrix[2]*matrix[5])/norm;
	matrix[9]  = (matrix[2]*matrix[4]-matrix[0]*matrix[6])/norm;
	matrix[10] = (matrix[0]*matrix[5]-matrix[1]*matrix[4])/norm;
	matrix[11] = 0;

	matrix[15] = 1;
	glPushMatrix();
	glTranslatef(a[0],a[1],a[2]);
	glMultMatrixf(matrix);
	glCallList(pipe);
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
		renderTeapot(2.0, 17.0, 0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 0.6);
		renderTeapot(2.0, 14.0, 0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 0.1);
		renderTeapot(2.0, 11.0, 0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 0.3);
		renderTeapot(2.0, 8.0, 0.25, 0.20725, 0.20725, 1, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 0.088);
		renderTeapot(2.0, 5.0, 0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 0.6);
		renderTeapot(2.0, 2.0, 0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 0.1);
		renderTeapot(6.0, 17.0, 0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 0.21794872);
		renderTeapot(6.0, 14.0, 0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 0.2);
		renderTeapot(6.0, 11.0, 0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 0.6);
		renderTeapot(6.0, 8.0, 0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 0.1);
		renderTeapot(6.0, 5.0, 0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 0.4);
		renderTeapot(6.0, 2.0, 0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 0.4);
		renderTeapot(10.0, 17.0, 0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, .25);
		renderTeapot(10.0, 14.0, 0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, .25);
		renderTeapot(10.0, 11.0, 0.0, 0.0, 0.0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, .25);
		renderTeapot(10.0, 8.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.7, 0.6, 0.6, .25);
		renderTeapot(10.0, 5.0, 0.0, 0.0, 0.0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, .25);
		renderTeapot(10.0, 2.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.60, 0.60, 0.50, .25);
		renderTeapot(14.0, 17.0, 0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, .078125);
		renderTeapot(14.0, 14.0, 0.0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, .078125);
		renderTeapot(14.0, 11.0, 0.0, 0.05, 0.0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, .078125);
		renderTeapot(14.0, 8.0, 0.05, 0.0, 0.0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, .078125);
		renderTeapot(14.0, 5.0, 0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, .078125);
		renderTeapot(14.0, 2.0, 0.05, 0.05, 0.0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, .078125);
		glPopMatrix();
	}

	if (display_hyper) {
		GLfloat foo[4];
		glPushMatrix();
		glTranslatef(0,0,-10);
		glScalef(2,2,2);
		glRotatef(20,1,0,0);
		glRotatef(angle/4.,0,1,0);
		foo[0]=1.; foo[1]=0.; foo[2]=0., foo[3] = 1.;
		glMaterialfv(GL_FRONT, GL_AMBIENT, foo);
		foo[0]=0.; foo[1]=1.; foo[2]=0., foo[3] = 1.;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, foo);
		foo[0]=0.; foo[1]=1.; foo[2]=0., foo[3] = 1.;
		glMaterialfv(GL_FRONT, GL_SPECULAR, foo);
		glMaterialf(GL_FRONT, GL_SHININESS, 1.);
		glBegin(GL_LINES);
		for (int i=0; i<16; i++) for (int j=0; j<2; j++) {
			const GLfloat *a = points_proj[i];
			const GLfloat *b = points_proj[lines[i][j]];
			glVertex3fv(a);
			glVertex3fv(b);
		}
		glEnd();
		for (int i=0; i<16; i++) for (int j=0; j<2; j++) {
			const GLfloat *a = points_proj[i];
			const GLfloat *b = points_proj[lines[i][j]];
			render_pipe(a,b);
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
	switch (mode) {
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
	printf("key=%d\n",key);
	switch (key) {
		case 27: //escape
			exit(0);
			break;
		case 32: //space
			mode++;
			mode %= 2;
			glutReshapeWindow(500,600);
			break;
		case 99: //c
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
		case 122: //z
			angle_proj -= M_PI/20;
			update_projector();
			update_points_proj();
			break;
	}
}

void timer(int value) {
	angle += 1;
	glutPostRedisplay();	
	glutTimerFunc(10,timer,value+1);
}

/*
 * Main Loop 
 */
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
	glutTimerFunc(0,timer,0);
	glutMainLoop();
	return 0;
}
