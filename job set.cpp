
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

// Update text that describes what the job is doing right now
// Returns the text you give it so you can log it as an error
string JobTask(read r) { sectionitem section;
	Job.task = r;
	return r;
}

// Count one more file, folder, or compare processed or error
void JobFileCount()          { sectionitem section; Job.file++;                      }
void JobFileError(read r)    { sectionitem section; Job.fileerror++;    JobError(r); }
void JobFolderCount()        { sectionitem section; Job.folder++;                    }
void JobFolderError(read r)  { sectionitem section; Job.foldererror++;  JobError(r); }
void JobCompareCount()       { sectionitem section; Job.compare++;                   }
void JobCompareError(read r) { sectionitem section; Job.compareerror++; JobError(r); }

// Record an error
void JobError(read r) { sectionitem section;

	// Count the error
	Job.error++;

	// Make a line of text with the error
	string s = make(r, L"\r\n");

	// Show the first 1000 errors on the screen
	if (Job.error <= STOPERRORS) Job.errors += s;
	if (Job.error == STOPERRORS) Job.errors += L"Additional errors in log file\r\n"; // Also make a note for the 1000th error

	// Write all the errors to the log file
	LogAppend(s);
}

// Path to the log file named backup.txt next to this running exe
string LogPath() {

	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	GetModuleFileName(NULL, bay, MAX_PATH);
	return before(bay, L"\\", Reverse) + L"\\backup.txt";
}

// Delete the log file to write a new one from the start
bool LogDelete() {

	return DiskDeleteFile(LogPath());
}

// Append the given line of text to the log file
bool LogAppend(read r) {

	// Open the file there or create one there and open it
	HANDLE file = CreateFile(
		LongPath(LogPath()),   // Path and file name
		GENERIC_WRITE,         // Only need to write
		0,                     // No sharing
		NULL,
		OPEN_ALWAYS,           // Create a new file here or open one already here
		FILE_ATTRIBUTE_NORMAL, // Normal attributes
		NULL);
	if (file == INVALID_HANDLE_VALUE) return false;

	// Move to the end of the file
	SetFilePointer(file, NULL, NULL, FILE_END);

	// Write it to the file
	DWORD written;
	int result = WriteFile(
		file,                      // Open file handle
		(LPVOID)r,                 // Pointer to data
		length(r) * sizeof(WCHAR), // Number of bytes there to write
		&written,                  // Number of bytes written
		NULL);
	if (!result) { CloseHandle(file); return false; }

	// Close the file
	CloseHandle(file);
	return true;
}
