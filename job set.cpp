
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

	// Add the given text to the bottom of the job's error list
	if (is(Job.errors)) Job.errors += L"\r\n";
	Job.errors += r;

	// If there have been a huge number of errors, stop the job
	Job.error++;
	if (Job.error >= STOPERRORS) {
		Job.errors += L"\r\nStopped at " + numerals(STOPERRORS) + L" errors";
		Job.stop = true;
	}


	/*
	// Write the first 1000 errors to the screen
	if 



	// Write the error to the log file

*/
}





// Append the given line of text to the log file
bool JobErrorLog(read r, bool clear) {

	// Compose path to log file named backup.txt next to this running exe
	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	if (!GetModuleFileName(NULL, bay, MAX_PATH)) return false;
	string path = before(bay, L"\\", Reverse) + L"\\backup.txt";

	// Open the file there or create one there and open it
	HANDLE file = CreateFile(
		LongPath(path),        // Path and file name
		GENERIC_WRITE,         // Only need to write
		0,                     // No sharing
		NULL,
		OPEN_ALWAYS,           // Create a new file here or open one already here
		FILE_ATTRIBUTE_NORMAL, // Normal attributes
		NULL);
	if (file == INVALID_HANDLE_VALUE) return false;

	// Clear the file
	if (clear) {

		SetFilePointer(file, NULL, NULL, FILE_BEGIN);
		if (!SetEndOfFile(file)) { CloseHandle(file); return false; }

	// Append this new line to the end
	} else {

		SetFilePointer(file, NULL, NULL, FILE_END);
	}

	// Compose the given text as a complete line
	string s = make(r, L"\r\n");

	// Write it to the file
	DWORD written;
	int result = WriteFile(
		file,                      // Open file handle
		(LPVOID)(read)s,         // Pointer to data
		length(s) * sizeof(WCHAR), // Number of bytes there to write
		&written,                  // Number of bytes written
		NULL);
	if (!result) { CloseHandle(file); return false; }

	// Close the file
	CloseHandle(file);
	return true;
}

