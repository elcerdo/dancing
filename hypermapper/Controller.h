#import <Cocoa/Cocoa.h>
#import "HyperArray.h"

@interface Controller : NSObject
{
	IBOutlet NSTextField *status_label;
	IBOutlet NSButton *solve_button;
	NSThread *thread;
	HyperArray *array;
	HyperThreadParams *params;
}

- (id)init;
- (void)awakeFromNib;
- (void)dealloc;

- (IBAction)solve:(id)sender;
- (void)timer_callback:(NSTimer *)timer;

- (int)numberOfRowsInTableView:(NSTableView *)table;
- (id)tableView:(NSTableView *)table objectValueForTableColumn:(NSTableColumn *)j row:(int)i;
- (void)tableView:(NSTableView *)table setObjectValue:(id)object forTableColumn:(NSTableColumn *)j row:(int)i;

@end
