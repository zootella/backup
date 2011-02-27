
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

// A new thread starts running this function
// Perform the job tasks
DWORD WINAPI Tasks() {

	// Break the text into lines that aren't blank
	string s = JobTasks();
	while (is(s) && JobContinue()) {
		string line;
		split(s, L"\r\n", &line, &s);
		line = trim(line, L" ");
		if (is(line)) {

			// Parse the line into parts
			string task, source, destination;
			string parse = line;
			split(parse, L"\"", &task, &parse);
			split(parse, L"\"", &source, &parse);
			split(parse, L"\"", &parse, &destination);
			task = trim(task, L" "); // Trim spaces
			source = trim(source, L" ");
			destination = trim(destination, L" ", L"\""); // Trim spaces and remove trailing quote
			task = off(task, L"\\", Reverse); // Remove trailing backslashes, "C:\" becomes "C:" so we can put on "\folder" later
			source = off(source, L"\\", Reverse);
			destination = off(destination, L"\\", Reverse);

			// Run the task
			if      (task == L"Delete")         TaskDelete(source);
			else if (task == L"Copy")           TaskCopy(source, destination);
			else if (task == L"Compare")        TaskCompare(source, destination);
			else if (task == L"Update")         TaskUpdate(source, destination, false);
			else if (task == L"Update Compare") TaskUpdate(source, destination, true);
			else                                JobError(L"Cannot " + line); // Unknown task
		}
	}

	// The job is done
	JobDone();

	// Let the thread exit
	return 0;
}

// Delete path
void TaskDelete(read path) {

	// Make sure path is to a folder we can edit
	if (!DiskFolder(path, false, true)) { JobError(make(L"Cannot write \"", path, L"\"")); return; }

	// Delete path
	TaskDeleteFolder(path); 
}

// Copy source to destination
void TaskCopy(read source, read destination) {

	// Make sure source is to a folder, and make folders to destination and confirm we can write there
	if (!DiskFolder(source, false, false)) { JobError(make(L"Cannot read \"", source, L"\"")); return; }
	if (!DiskFolder(destination, true, true)) { JobError(make(L"Cannot write \"", destination, + L"\"")); return; }

	// Copy source to destination
	TaskCopyFolder(source, destination, false); // false to not also compare
}

// Compare source to destination
void TaskCompare(read source, read destination) {

	// Make sure source and destination are to folders we can read
	if (!DiskFolder(source, false, false)) { JobError(make(L"Cannot read \"", source, L"\"")); return; }
	if (!DiskFolder(destination, false, false)) { JobError(make(L"Cannot read \"", destination, L"\"")); return; }

	// Compare source to destination
	TaskCompareFolder(source, destination);
}

// Update source to destination, true to compare each file after we copy it
void TaskUpdate(read source, read destination, bool compare) {

	// Make sure source is to a folder, and make folders to destination and confirm we can write there
	if (!DiskFolder(source, false, false)) { JobError(make(L"Cannot read \"", source, L"\"")); return; }
	if (!DiskFolder(destination, true, true)) { JobError(make(L"Cannot write \"", destination, L"\"")); return; }

	// Perform the two tasks that make up the update
	TaskUpdateClear(source, destination);         // Clear the destination of extra and different files and folders
	TaskUpdateFill(source, destination, compare); // Copy missing files and folders in the destination from the source
}
