
// Include statements
#include <windows.h>
#include <windef.h>
#include <atlstr.h>
#include <shlobj.h>
#include "resource.h"
#include "program.h"
#include "class.h"
#include "function.h"

// Compose the path to the hash log file like "C:\Programs\Hash 2011-Sep-29 8;50p 41.084s.html" next to this running exe
string LogPathHash() {

	return LogPath(make(L"Hash ", saydate(L";")));
}

// Path to the log file named Backup.html next to this running exe
string LogPathError() {

	return LogPath(L"Backup");
}

// Takes a name like "Backup" without the trailing ".html" extension
// Composes the path to the log file with the given name next to this running exe
string LogPath(read name) {

	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	GetModuleFileName(NULL, bay, MAX_PATH);
	return make(before(bay, L"\\", Reverse), L"\\", string(name), L".html");
}

// Delete the log file at the given path to write a new one there from the start
bool LogDelete(read path) {

	return DiskDeleteFile(path);
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

	// Start the file with two bytes that tell programs that unicode characters follow
	LogAppend(file, L"\ufeff"); // Writes the 2 bytes ff fe, with ff first

	// Write the HTML and text headers
	LogAppend(file, L"<html><head><title>Backup</title></head><body><pre style=\"font: 8pt Courier New\">\r\n");
	LogAppend(file, make(title, L"\r\n")); // Include the given title line of text
	LogAppend(file, L"\r\n");
	LogAppend(file, make(L"---- start ---- ", saydate(L":"), L" ----\r\n"));

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

	// Write the text and HTML footers
	LogAppend(file, make(L"---- end ---- ", saydate(L":"), L" ----\r\n"));
	LogAppend(file, L"</pre></body></html>\r\n");

	// Close the file
	if (!CloseHandle(file)) return false;
	return true;
}
