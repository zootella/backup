
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

// The user clicked the Clear button
void ButtonClear() {

	// Clear the contents of the Tasks box
	WindowTextSet(Handle.tasks, "");
}

// The user clicked the Delete task menu item
void ButtonDelete() {

	// Let the user choose folders and list the task
	string path = DialogBrowse("Choose the folder to delete.");
	EditAppend(Handle.tasks, "Delete \"" + path + "\"");
}

// The user clicked the Copy task menu item
void ButtonCopy() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse("Choose the source folder to copy.");
	string path2 = DialogBrowse("Choose the destination folder for the copy.");
	EditAppend(Handle.tasks, "Copy \"" + path1 + "\" to \"" + path2 + "\"");
}

// The user clicked the Compare task menu item
void ButtonCompare() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse("Choose the source folder to compare from.");
	string path2 = DialogBrowse("Choose the destination folder to compare to.");
	EditAppend(Handle.tasks, "Compare \"" + path1 + "\" to \"" + path2 + "\"");
}

// The user clicked the Update task menu item
void ButtonUpdate() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse("Choose the source folder to update from.");
	string path2 = DialogBrowse("Choose the destination folder to update. This folder will be changed.");
	EditAppend(Handle.tasks, "Update \"" + path1 + "\" to \"" + path2 + "\"");
}

// The user clicked the Update Compare task menu item
void ButtonUpdateCompare() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse("Choose the source folder to update and compare from.");
	string path2 = DialogBrowse("Choose the destination folder to update and compare. This folder will be changed.");
	EditAppend(Handle.tasks, "Update Compare \"" + path1 + "\" to \"" + path2 + "\"");
}
