//
//  Controller.h
//  hypermapper
//
//  Created by Marion Chaudat on 13/04/10.
//  Copyright 2010. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "HyperArray.h"


@interface Controller : NSObject
{
	IBOutlet NSTextField *status;
	HyperArray *array;
}

- (id)init;
- (void)dealloc;
- (IBAction)solve:(id)sender;

- (int)numberOfRowsInTableView:(NSTableView *)table;
- (id)tableView:(NSTableView *)table objectValueForTableColumn:(NSTableColumn *)j row:(int)i;
- (void)tableView:(NSTableView *)table setObjectValue:(id)object forTableColumn:(NSTableColumn *)j row:(int)i;

@end
