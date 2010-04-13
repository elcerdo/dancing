#import "Controller.h"
#import <sstream>

@implementation Controller

- (id)init {
	[super init];
	array = new HyperArray();
	return self;
}

- (void)dealloc {
	delete array;
	[super dealloc];
}

- (IBAction)solve:(id)sender {
	NSLog(@"Solving %@",sender);
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
	[status setStringValue:[NSString stringWithFormat:@"%s from %d goes to %d",column,i+1,value]];
}


@end
