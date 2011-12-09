
// Include statements
#include <windows.h>
#include <windef.h>
#include <atlstr.h>
#include <shlobj.h>
#include "resource.h"
#include "program.h"
#include "class.h"
#include "function.h"

// Takes a name like "Backup" without the trailing ".html" extension
// Composes the path to the log file with the given name next to this running exe
string LogPath(read name) {

	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	GetModuleFileName(NULL, bay, MAX_PATH);
	return make(before(bay, L"\\", Reverse), L"\\", string(name), L".html");
}

// Create a new log file at the given path and open it
HANDLE LogOpen(read path, read title) {

	// Open the file there or create one there and open it
	HANDLE file = CreateFile(
		LongPath(path),        // Path and file name
		GENERIC_WRITE,         // Only need to write
		0,                     // No sharing
		NULL,
		CREATE_ALWAYS,         // Create a new file here or overwrite one already here
		FILE_ATTRIBUTE_NORMAL, // Normal attributes
		NULL);
	if (file == INVALID_HANDLE_VALUE) return NULL;

	// Write the headers
	LogAppend(file, L"\ufeff"); // Start with the 2 bytes ff fe, with ff first, to tell programs that unicode characters follow
	LogAppend(file, L"<html><head><title>Backup</title></head><body><pre style=\"font: 8pt Courier New\">\r\n"); // HTML header for web browsers
	LogAppend(file, make(title, L"\r\n")); // Text headers for the user
	LogAppend(file, L"\r\n");
	LogAppend(file, make(saydate(L":"), L" start\r\n"));
	LogAppend(file, L"----------------------------------------\r\n");

	// Return the open file handle
	return file;
}

// Takes the handle to an open log file and a line of text that ends with "\r\n"
// Appends the text to the end of the file and moves the file pointer past it
bool LogAppend(HANDLE file, read r) {

	// Write it to the file
	DWORD bytes = length(r) * sizeof(WCHAR);
	DWORD written = 0;
	int result = WriteFile(
		file,      // Open file handle
		(LPVOID)r, // Pointer to data
		bytes,     // Number of bytes there to write
		&written,  // Number of bytes written
		NULL);
	if (!result || written != bytes) return false;
	return true;
}

// Close the given open log file
bool LogClose(HANDLE file) {

	// Write the footers
	LogAppend(file, L"----------------------------------------\r\n");
	LogAppend(file, make(saydate(L":"), L" end\r\n"));
	LogAppend(file, L"</pre></body></html>\r\n");

	// Close the file
	if (!CloseHandle(file)) return false;
	return true;
}
