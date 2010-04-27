#import <Cocoa/Cocoa.h>
 
@interface HyperView : NSOpenGLView {
	float transform[4][4];
	float projection[4][4];
}

@end
