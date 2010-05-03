#ifndef __HYPERCUBE__H__
#define __HYPERCUBE__H__

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif


class HyperCube {
public:
    HyperCube();
    void init_opengl();
    void render(void) const;
    void reshape(int w, int h) const;

    void update_projector(float angle_proj);
    void update_points_proj();

    GLuint proj_type;
    GLuint pipe_type;
    GLuint pipe_color;
    bool object_display;
    GLfloat object_angle;
    bool pipe_display;
    bool node_display;
    GLfloat angle_x,angle_y;
private:
    void render_teapot(GLfloat x, GLfloat y) const;
    void render_pipe(const GLfloat *a, const GLfloat *b) const;
    float projector[4][4];
    float points_proj[16][4];

    GLuint list_base;
    GLuint object,pipe1,pipe2,node;
};




#endif
