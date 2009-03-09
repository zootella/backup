
// Include statements
#include <windows.h>
#include <windef.h>
#include <atlstr.h>
#include <shlobj.h>
#include "resource.h"
#include "program.h"
#include "class.h"
#include "function.h"

// Global objects
extern handleitem Handle;

// Change the window to hold current information
void WindowUpdate() {

	// Get current information from the job
	displayitem display = JobDisplay();

	// Compare our record of the window with the way it should be to only change what's necessary
	bool paint = Handle.display.banner != display.banner; // Paint the banner
	if (Handle.display.edittasks != display.edittasks) WindowEdit(Handle.tasks, display.edittasks); // Tasks box editable or read-only
	if (Handle.display.enableclear != display.enableclear) EnableWindow(Handle.clear, display.enableclear); // Enable buttons
	if (Handle.display.enabletask  != display.enabletask)  EnableWindow(Handle.task,  display.enabletask);
	if (Handle.display.enablestart != display.enablestart) EnableWindow(Handle.start, display.enablestart);
	if (Handle.display.enablestop  != display.enablestop)  EnableWindow(Handle.stop,  display.enablestop);
	if (Handle.display.enablereset != display.enablereset) EnableWindow(Handle.reset, display.enablereset);
	if (Handle.display.status != display.status) WindowTextSet(Handle.status, display.status); // Status and errors text
	if (Handle.display.errors != display.errors) { WindowTextSet(Handle.errors, display.errors); EditScroll(Handle.errors); }

	// Update our record of the way the window is after the changes we just made, for now and the next time
	Handle.display = display;

	// If the banner changed, repaint the window right now
	if (paint) PaintMessage(Handle.window);
}
