
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
	LogAppendOld(s);
}
