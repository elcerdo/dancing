#import <Cocoa/Cocoa.h>

#import "hypercube.h"

@interface HyperView : NSOpenGLView {
	HyperCube *cube;
	float init_angle_x,init_angle_y;
	int init_x,init_y;
	
}

- (IBAction)updateProjAngle:(id)sender;

@end
