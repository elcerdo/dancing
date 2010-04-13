#import "Controller.h"
#import <sstream>
#import <cassert>

@implementation Controller

- (id)init {
	[super init];
	params = NULL;
	array = new HyperArray();
	return self;
}

- (void)awakeFromNib {
	[status_label setStringValue:@"welcome!!!"];
}

- (void)dealloc {
	delete array;
	[super dealloc];
}

- (IBAction)solve:(id)sender {
	assert(not params);
	[status_label setStringValue:@"solving"];
	[solve_button setEnabled:NO];
	[NSTimer scheduledTimerWithTimeInterval:.1 target:self selector:@selector(timer_callback:) userInfo:nil repeats:YES];
	params = new HyperThreadParams(*array);
}

- (void)timer_callback:(NSTimer *)timer {
	assert(params);
	if (not params->is_finished()) return;

	[timer invalidate];
	[status_label setStringValue:@"finished"];
	[solve_button setEnabled:YES];
	delete params;
	params = NULL;
}

- (int)numberOfRowsInTableView:(NSTableView *)table {
	return array->HEIGHT;
}

- (id)tableView:(NSTableView *)table objectValueForTableColumn:(NSTableColumn *)j row:(int)i {
	int value = array->get_value(i,[[[j headerCell] stringValue] UTF8String]);
	if (not value) return @"";
	return [NSString stringWithFormat:@"%d",value];
}

- (void)tableView:(NSTableView *)table setObjectValue:(id)object forTableColumn:(NSTableColumn *)j row:(int)i {
	const char *column = [[[j headerCell] stringValue] UTF8String];
	std::stringstream ss([object UTF8String]);
	int value = 0;
	ss >> value;
	if (ss.fail()) return;
	if (not array->set_value(i,column,value)) return;
	[status_label setStringValue:[NSString stringWithFormat:@"%s from %d goes to %d",column,i+1,value]];
}


@end
