
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
	WindowTextSet(Handle.tasks, L"");
}

// The user clicked the Delete task menu item
void ButtonDelete() {

	// Let the user choose folders and list the task
	string path = DialogBrowse(L"Choose the folder to delete.");
	EditAppend(Handle.tasks, L"Delete \"" + path + L"\"");
}

// The user clicked the Copy task menu item
void ButtonCopy() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse(L"Choose the source folder to copy.");
	string path2 = DialogBrowse(L"Choose the destination folder for the copy.");
	EditAppend(Handle.tasks, L"Copy \"" + path1 + L"\" to \"" + path2 + L"\"");
}

// The user clicked the Compare task menu item
void ButtonCompare() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse(L"Choose the source folder to compare from.");
	string path2 = DialogBrowse(L"Choose the destination folder to compare to.");
	EditAppend(Handle.tasks, L"Compare \"" + path1 + L"\" to \"" + path2 + L"\"");
}

// The user clicked the Update task menu item
void ButtonUpdate() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse(L"Choose the source folder to update from.");
	string path2 = DialogBrowse(L"Choose the destination folder to update. This folder will be changed.");
	EditAppend(Handle.tasks, L"Update \"" + path1 + L"\" to \"" + path2 + L"\"");
}

// The user clicked the Update Compare task menu item
void ButtonUpdateCompare() {

	// Let the user choose folders and list the task
	string path1 = DialogBrowse(L"Choose the source folder to update and compare from.");
	string path2 = DialogBrowse(L"Choose the destination folder to update and compare. This folder will be changed.");
	EditAppend(Handle.tasks, L"Update Compare \"" + path1 + L"\" to \"" + path2 + L"\"");
}
