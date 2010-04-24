#import <Cocoa/Cocoa.h>
#import "HyperArray.h"

@interface Controller : NSObject
{
	IBOutlet NSTextField *status_label;
	IBOutlet NSButton *solve_button;
	IBOutlet NSTableView *solution_table;
	IBOutlet NSTableView *constraint_table;
	NSThread *thread;
	HyperArray *array;
	HyperThreadParams *params;
	int selected_solution;
}

- (id)init;
- (void)awakeFromNib;
- (void)dealloc;

- (IBAction)solve:(id)sender;
- (IBAction)select_solution:(id)sender;
- (void)timer_callback:(NSTimer *)timer;

- (int)numberOfRowsInTableView:(NSTableView *)table;
- (id)tableView:(NSTableView *)table objectValueForTableColumn:(NSTableColumn *)j row:(int)i;
- (void)tableView:(NSTableView *)table setObjectValue:(id)object forTableColumn:(NSTableColumn *)j row:(int)i;

- (IBAction)save:(id)sender;
- (IBAction)load:(id)sender;

@end
