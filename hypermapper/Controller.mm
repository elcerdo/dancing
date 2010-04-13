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
	
	delete array;
	array = new HyperArray(params->local_array);
	
	[timer invalidate];
	[status_label setStringValue:[NSString stringWithFormat:@"%d solutions found, %d moves and %d constraints remaining",array->solutions.size(),params->moves,params->constraints]];
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
	
	if (ss.str() == "" and array->unset_value(i,column)) {
		[status_label setStringValue:[NSString stringWithFormat:@"unsetting %s from %d",column,i+1]];
		return;
	}
	
	int value = 0;
	ss >> value;
	if (ss.fail()) return;
	
	if (array->set_value(i,column,value)) {
		[status_label setStringValue:[NSString stringWithFormat:@"%s from %d goes to %d",column,i+1,value]];
		return;
	}
}


@end
