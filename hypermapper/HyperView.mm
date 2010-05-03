#import "HyperView.h"

@implementation HyperView

- (void)dealloc
{
	delete cube;
	[super dealloc];
}

- (void)prepareOpenGL
{
	if (!cube) { cube = new HyperCube; }
	cube->init_opengl();
}

- (void)mouseDown:(NSEvent *)event
{
	if ([event buttonNumber] != 0) return;
	
	init_angle_x = cube->angle_x;
	init_angle_y = cube->angle_y;
	init_x = -[NSEvent mouseLocation].y;
	init_y = [NSEvent mouseLocation].x;
}

- (void)mouseUp:(NSEvent *)event
{
	if ([event buttonNumber] != 0) return;

	cube->angle_x = init_angle_x + (-[NSEvent mouseLocation].y - init_x)/3.;
	cube->angle_y = init_angle_y + ([NSEvent mouseLocation].x - init_y)/3.;

	[self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
	if ([event buttonNumber] != 0) return;
	
	cube->angle_x = init_angle_x + (-[NSEvent mouseLocation].y - init_x)/3.;
	cube->angle_y = init_angle_y + ([NSEvent mouseLocation].x - init_y)/3.;
	
	[self setNeedsDisplay:YES];
}

- (void)lockFocus
{
	NSOpenGLContext *context = [self openGLContext];
	[super lockFocus];
	if ([context view] != self) { [context setView:self]; }
	[context makeCurrentContext];
}

- (void)updateProjAngle:(id)sender
{
	cube->update_projector([sender floatValue]);
	[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)rect
{
	NSOpenGLContext *context = [self openGLContext];
	[context makeCurrentContext];
	GLsizei w = [self bounds].size.width;
	GLsizei h = [self bounds].size.height;
	cube->reshape(w,h);
	cube->render();
	[context flushBuffer];
}



@end
