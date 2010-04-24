#import "Controller.h"
#import <sstream>
#import <cassert>

@implementation Controller

- (id)init {
	[super init];
	selected_solution = 0;
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

- (IBAction)select_solution:(id)sender {
	selected_solution = [sender intValue];
	[solution_table reloadData];
	[status_label setStringValue:[NSString stringWithFormat:@"selected solution %d",selected_solution]];
}

- (IBAction)solve:(id)sender {
	assert(not params);
	[status_label setStringValue:@"solving"];
	[solve_button setEnabled:NO];
	[NSTimer scheduledTimerWithTimeInterval:.1 target:self selector:@selector(timer_callback:) userInfo:nil repeats:YES];
	array->solutions.clear();
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
	[solution_table reloadData];
	
	delete params;
	params = NULL;
}

- (int)numberOfRowsInTableView:(NSTableView *)table {
	return array->HEIGHT;
}

- (id)tableView:(NSTableView *)table objectValueForTableColumn:(NSTableColumn *)j row:(int)i {
	int value = array->get_value(i,[[j identifier] UTF8String],selected_solution);
	if (not value) return @"";
	return [NSString stringWithFormat:@"%d",value];
}

- (void)tableView:(NSTableView *)table setObjectValue:(id)object forTableColumn:(NSTableColumn *)j row:(int)i {
	const char *column = [[j identifier] UTF8String];
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

- (IBAction)save:(id)sender {
	NSSavePanel *panel = [NSSavePanel savePanel];
	SEL sel = @selector(saveEnded:returnCode:contextInfo:);
	[panel beginSheetForDirectory:@"~" file:@"data.hypermapper" modalForWindow:[solution_table window] modalDelegate:self didEndSelector:sel contextInfo:NULL];
}

- (void)saveEnded:(NSSavePanel *)panel returnCode:(int)code contextInfo:(void *)context {
	if (code != NSOKButton) return;
	const char *filename = [[panel filename] UTF8String];
	bool success = array->save_file(filename);
	if (success) [status_label setStringValue:[NSString stringWithFormat:@"saved %s",filename]];
}

- (IBAction)load:(id)sender {
	NSOpenPanel *panel = [NSOpenPanel openPanel];
	SEL sel = @selector(openEnded:returnCode:contextInfo:);
	[panel beginSheetForDirectory:@"~" file:nil types:nil modalForWindow:[solution_table window] modalDelegate:self didEndSelector:sel contextInfo:NULL];
}

- (void)openEnded:(NSOpenPanel *)panel returnCode:(int)code contextInfo:(void *)context {
	if (code != NSOKButton) return;
	const char *filename = [[panel filename] UTF8String];
	bool success = array->load_file(filename);
	if (success) [status_label setStringValue:[NSString stringWithFormat:@"loaded %s",filename]];
	else [status_label setStringValue:[NSString stringWithFormat:@"error while loading %s",filename]];
	[solution_table reloadData];
	[constraint_table reloadData];
}

@end
