
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
}
