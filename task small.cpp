
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

// Delete the contents of the folder at path, and then the folder itself
void TaskDeleteFolder(read path) {

	// Delete the contents of the folder
	finditem f(path);
	while (f.result()) {

		// Delete the subfolder or file
		if (f.folder())
			TaskDeleteFolder(make(path, L"\\", f.info.cFileName));
		else
			TaskDeleteFile(make(path, L"\\", f.info.cFileName));

		if (!JobContinue()) return;
	}

	// Delete the now empty folder
	string s = make(L"Deleting folder \"", path, L"\""); // Don't update the status with this, just compose it for an error
	if (!DiskDeleteFolder(path)) JobError(s);
}

// Delete the file at path
void TaskDeleteFile(read path) {

	// Delete the file
	string s = JobTask(make(L"Deleting \"", path, L"\""));
	if (!DiskDeleteFile(path)) JobError(s);
}

// Copy the folder at source to the destination path, true to compare each file right after we copy it
void TaskCopyFolder(read source, read destination, bool compare) {

	// Make the destination folder if it's not already there
	TaskMakeFolder(destination);

	// Copy the contents across
	finditem f(source);
	while (f.result() && JobContinue()) {

		// Copy the subfolder or file
		if (f.folder())
			TaskCopyFolder(make(source, L"\\", f.info.cFileName), make(destination, L"\\", f.info.cFileName), compare);
		else
			TaskCopyFile(make(source, L"\\", f.info.cFileName), make(destination, L"\\", f.info.cFileName), compare);
	}
}

// Make a new folder at path or confirm one is already there
void TaskMakeFolder(read path) {

	// Make the folder
	string s = make(L"Making folder \"", path, L"\""); // Don't update the status with this, just compose it for an error
	DiskMakeFolder(path) ? JobFolderCount() : JobFolderError(s);
}

// Copy the file at source to the destination path, true to compare it also
void TaskCopyFile(read source, read destination, bool compare) {

	// Copy the file
	string s = JobTask(make(L"Copying \"", source, L"\" to \"", destination, L"\""));
	DiskCopyFile(source, destination) ? JobFileCount() : JobFileError(s);

	// Compare the files
	if (compare) TaskCompareFile(source, destination);
}

// Compare the folder at source to the folder at destination
void TaskCompareFolder(read source, read destination) {

	// Make sure source and destination are folders
	string s = make(L"Checking folders \"", source, L"\" and \"", destination, L"\\"); // Don't update the status with this, just compose it for an error
	if (!DiskIsFolder(source) || !DiskIsFolder(destination)) {
		JobError(s); // Add a line of text to the errors, but don't count this as a file compare error
		return;      // Leave without going into source and finding all its contents as errors also
	}

	// Loop through the source folder contents
	finditem f(source);
	while (f.result() && JobContinue()) {

		// Compare the subfolder or file
		if (f.folder())
			TaskCompareFolder(make(source, L"\\", f.info.cFileName), make(destination, L"\\", f.info.cFileName));
		else
			TaskCompareFile(make(source, L"\\", f.info.cFileName), make(destination, L"\\", f.info.cFileName));
	}
}

// Compare the file at source to the one at destination
void TaskCompareFile(read source, read destination) {

	// Compare the files
	string s = JobTask(make(L"Comparing \"", source, L"\" to \"", destination, L"\""));
	DiskCompareFile(source, destination) ? JobCompareCount() : JobCompareError(s);
}

// Delete the folders and files in destination that don't match the source
void TaskUpdateClear(read source, read destination) {

	// Loop through the contents of the destination folder
	JobTask(make(L"Clearing \"", destination, L"\""));
	finditem f(destination);
	string s, d;
	while (f.result() && JobContinue()) {
		s = make(source,      L"\\", f.info.cFileName); // The path that matches on the source side
		d = make(destination, L"\\", f.info.cFileName); // The path we found on the destination side

		if (f.folder())                // Found a subfolder on the destination side
			if (DiskIsFolder(s))       // If there is also a folder on the source side
				TaskUpdateClear(s, d); // Move into the pair of folders
			else                       // Otherwise there is a file or nothing on the source side
				TaskDeleteFolder(d);   // Delete the folder on the destination side
		else                           // Found a file on the destination side
			if (!DiskSameFile(s, &f))  // If there isn't a file on the source side that matches
				TaskDeleteFile(d);     // Delete the file on the destination side
	}
}

// Copy source to destination, not overwriting anything
void TaskUpdateFill(read source, read destination, bool compare) {

	// Loop through the contents of the source folder
	JobTask(make(L"Updating \"", source, L"\""));
	finditem f(source);
	string s, d;
	while (f.result() && JobContinue()) {
		s = make(source,      L"\\", f.info.cFileName); // The path we found on the source side
		d = make(destination, L"\\", f.info.cFileName); // The path that matches on the destination side

		if (f.folder())                        // Found a source subfolder
			if (DiskIsFolder(d))               // There is also a folder on the destination side
				TaskUpdateFill(s, d, compare); // Move into the pair of folders
			else                               // There isn't a folder on the destination side, so it must be available
				TaskCopyFolder(s, d, compare); // Copy the folder across
		else                                   // Found a source file
			if (!DiskIsFile(d))                // If there isn't a file on the destination side, or we can't tell
				TaskCopyFile(s, d, compare);   // Copy the file across, and optionally compare it
	}
}
