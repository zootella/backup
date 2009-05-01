
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
jobitem Job; // Make the job object here

// Reset the program to let the user prepare the job
void JobReset() { sectionitem section;

	// Change the stage
	if (Job.stage == JobStageRunning) return; // Can only reset when before or done

	// Initialize job information
	Job.stage = JobStageBefore;
	Job.stop = false;
	Job.time = 0;
	Job.tasks = _T("");
	Job.task = Job.errors = _T("");
	Job.error = 0;
	Job.folder = Job.file = Job.compare = 0;
	Job.foldererror = Job.fileerror = Job.compareerror = 0;

	// Load tasks text from the registry to the tasks box
	string s = RegistryRead(REGISTRYKEY, REGISTRYPATH, REGISTRYNAME);
	WindowTextSet(Handle.tasks, s);
}

// Start the job, locking down the program while it runs
void JobStart() { sectionitem section;

	// Change the stage
	if (Job.stage != JobStageBefore) return; // Can only start when before
	Job.stage = JobStageRunning;

	// Record the time now as when the job starts
	Job.time = GetTickCount();

	// Save tasks text from the tasks box to the registry
	Job.tasks = WindowTextGet(Handle.tasks);
	RegistryWrite(REGISTRYKEY, REGISTRYPATH, REGISTRYNAME, Job.tasks);

	// Start a new thread that will perform all the tasks
	BeginThread(Tasks); // Have the thread run the Tasks() function
}

// Check to see if the job should stop
bool JobContinue() { sectionitem section;

	// Check the stage
	if (Job.stage != JobStageRunning) return false; // Should only be used when running

	// Return false to make the caller not do the next task
	return !Job.stop;
}

// Stop the job now, while it's running
void JobStop() { sectionitem section;

	// Check the stage
	if (Job.stage != JobStageRunning) return; // Can only stop when running

	// Record an error that we stopped it
	JobError(_T("Stopped"));

	// Set the flag for the thread to see
	Job.stop = true;
}

// The job is done
void JobDone() { sectionitem section;

	// Change the stage
	if (Job.stage != JobStageRunning) return; // Can only stop when running
	Job.stage = JobStageDone;

	// Calculate how long the job took
	Job.time = GetTickCount() - Job.time;

	// Clear the last task it did, this would show up in status
	JobTask(_T(""));
}
