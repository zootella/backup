
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
extern jobitem Job;

// Get the lines of text from the tasks box
string JobTasks() { sectionitem section;
	return Job.tasks;
}

// Get current information from the job for how the window should look
displayitem JobDisplay() { sectionitem section;

	// Make a display item to fill with information and return
	displayitem display;

	// Banner text
	if      (Job.stage == JobStageBefore)  display.banner = "start";
	else if (Job.stage == JobStageRunning) display.banner = "running";
	else if (Job.stage == JobStageDone)
		if (isblank(Job.errors))           display.banner = "done";
		else                               display.banner = "errors";

	// Before the job starts
	if (Job.stage == JobStageBefore) {

		// Tasks box editable
		display.edittasks = true;

		// Only stop is disabled
		display.enableclear = true;
		display.enabletask  = true;
		display.enablestart = true;
		display.enablestop  = false;
		display.enablereset = true;

	// The job is running
	} else if (Job.stage == JobStageRunning) {

		// Tasks box read-only
		display.edittasks = false;

		// Only stop is enabled
		display.enableclear = false;
		display.enabletask  = false;
		display.enablestart = false;
		display.enablestop  = true;
		display.enablereset = false;

	// The job is done
	} else if (Job.stage == JobStageDone) {

		// Tasks box read-only
		display.edittasks = false;

		// Only reset is enabled
		display.enableclear = false;
		display.enabletask  = false;
		display.enablestart = false;
		display.enablestop  = false;
		display.enablereset = true;

	}

	// Compose status text
	string s; // If we're before the job, leave s blank to make the status box blank
	if (Job.stage != JobStageBefore) {

		// Show how long the job's been running, or took to finish
		if (Job.stage == JobStageRunning) {
			s = "Running for " + saytime(GetTickCount() - Job.time) + "\r\n";
		} else if (Job.stage == JobStageDone) {
			if (isblank(Job.errors)) s = "Done in "             + saytime(Job.time) + "\r\n";
			else                     s = "Done with errors in " + saytime(Job.time) + "\r\n";
		}

		// Show counts
		s += "Processed " +
			saynumber(Job.folder,       _T("folder"))        + _T(", ") +
			saynumber(Job.foldererror,  _T("folder error"))  + _T(", ") +
			saynumber(Job.file,         _T("file"))          + _T(", ") +
			saynumber(Job.fileerror,    _T("file error"))    + _T(", ") +
			saynumber(Job.compare,      _T("compare"))       + _T(", ") +
			saynumber(Job.compareerror, _T("compare error")) + _T("\r\n");

		// Add the given line that says what the program is doing now
		s += Job.task;
	}
	display.status = s;

	// Copy the lines of error text
	display.errors = Job.errors;

	// Return the object we filled with information
	return display;
}
