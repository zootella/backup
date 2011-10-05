
// Include statements
#include <windows.h>
#include <windef.h>
#include <atlstr.h>
#include <shlobj.h>
#include "resource.h"
#include "program.h"
#include "class.h"
#include "function.h"

//old


// Append the given line of text to the log file
bool LogAppendOld(read r) {

	// Open the file there or create one there and open it
	HANDLE file = CreateFile(
		LongPath(LogPathError()),   // Path and file name
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

//new

// Takes the path to the folder to hash
// Compose the path to the hash file like "Hash 2011-Sep-29 8;50p 41.084s Folder Name.txt" next to this running exe
string LogPathHash(read folder) {

	// Get the local time right now
	SYSTEMTIME info;
	ZeroMemory(&info, sizeof(info));
	GetLocalTime(&info);

	// Compose month text
	string month;
	switch (info.wMonth) {
		case  1: month = L"Jan"; break;
		case  2: month = L"Feb"; break;
		case  3: month = L"Mar"; break;
		case  4: month = L"Apr"; break;
		case  5: month = L"May"; break;
		case  6: month = L"Jun"; break;
		case  7: month = L"Jul"; break;
		case  8: month = L"Aug"; break;
		case  9: month = L"Sep"; break;
		case 10: month = L"Oct"; break;
		case 11: month = L"Nov"; break;
		case 12: month = L"Dec"; break;
	}

	// Prepare hour and AM/PM text
	string m = info.wHour < 12 ? L"a" : L"p";
	int h = info.wHour;
	if (!h) h = 12;
	if (h > 12) h -= 12;

	// Turn numbers into text
	string year        = numerals(info.wYear);
	string day         = numerals(info.wDay);
	string hour        = numerals(h);
	string minute      = numerals(info.wMinute, 2);
	string second      = numerals(info.wSecond, 2);
	string millisecond = numerals(info.wMilliseconds, 3);

	// Get the folder name, blank if folder is a drive root, the share name if folder is the path to a network share root
	string name = after(folder, L"\\", Reverse);
	if (is(name)) name = L" " + name;

	// Put it all together
	return LogPath(make(L"Hash ", year, L"-", month, L"-", day, L" ", hour, L";") + make(minute, m, L" ", second, L".", millisecond, L"s", name));
}

// Path to the log file named backup.txt next to this running exe
string LogPathError() {

	return LogPath(L"backup");
}

// Takes a name like "backup" without the trailing ".txt" extension
// Composes the path to the log file with the given name next to this running exe
string LogPath(read name) {

	WCHAR bay[MAX_PATH];
	lstrcpy(bay, L"");
	GetModuleFileName(NULL, bay, MAX_PATH);
	return make(before(bay, L"\\", Reverse), L"\\", string(name), L".txt");
}

// Delete the log file at the given path to write a new one there from the start
bool LogDelete(read path) {

	return DiskDeleteFile(path);
}

// Open the existing log file at the given path, or create a new one there and open it
HANDLE LogOpen(read path) {

	// Open the file there or create one there and open it
	HANDLE file = CreateFile(
		LongPath(path),        // Path and file name
		GENERIC_WRITE,         // Only need to write
		0,                     // No sharing
		NULL,
		OPEN_ALWAYS,           // Create a new file here or open one already here
		FILE_ATTRIBUTE_NORMAL, // Normal attributes
		NULL);
	if (file == INVALID_HANDLE_VALUE) return NULL;

	// Move to the end of the file
	SetFilePointer(file, NULL, NULL, FILE_END);
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

	if (!CloseHandle(file)) return false;
	return true;
}

