
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

// Run a snippet of test code
void Test() {

}

/*



// Delete the contents of the log file
void LogClear() {


}

// Append the given line of text to the log file
void LogSave(read r, bool clear) {




	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	if (!GetModuleFileName(NULL, bay, MAX_PATH)) return;
	string path = before(bay, L"\\", Reverse) + L"\\backup.txt";



	HANDLE file = CreateFile(
		LongPath(path),        // Path and file name
		GENERIC_WRITE,         // Only need to write
		0,                     // No sharing
		NULL,
		OPEN_ALWAYS,           // Create a new file here or open one already here
		FILE_ATTRIBUTE_NORMAL, // Normal attributes
		NULL);
	if (file == INVALID_HANDLE_VALUE) return;





	SetFilePointer(file, NULL, NULL, clear ? FILE_BEGIN : FILE_END);




	// WRITE THE NUMBER OF CHARACTERS IN THE TEXT MULTIPLIED BY THE SIZE OF EACH CHARACTER
	FileWrite(file, (LPVOID)r, length(r) * sizeof(character)));



	DWORD written;
	int result = WriteFile(
		file,      // OPEN FILE HANDLE
		(LPVOID)r, // POINTER TO DATA BUFFER
		length(r) * sizeof(WCHAR),      // NUMBER OF BYTES TO WRITE
		&written,  // THE NUMBER OF BYTES WRITTEN IS STORED HERE
		NULL);
	if (!result) { Report("filewrite: error writefile"); return(false); }










}








*/



