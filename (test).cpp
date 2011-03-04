
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

	string s;
	s = LongPath(L"\\\\server\\share\\folder");
	s = LongPath(L"C:\\folder");



	DWORD d;
	bool b;

	d = GetFileAttributes(L"C:");
	b = d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);//true, 0x00002010

	d = GetFileAttributes(L"C:\\");
	b = d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);//true, 0x00002016

	d = GetFileAttributes(L"\\\\?\\C:");
	b = d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);//false, 0xffffffff

	d = GetFileAttributes(L"\\\\?\\C:\\");
	b = d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);//true, 0x00000216


}




string LongPath(read path) {

	// Network path
	if (starts(path, L"\\\\"))                       //       "\\server\share\folder"
		return make(L"\\\\?\\UNC\\", clip(path, 2)); // "\\?\UNC\server\share\folder"

	// Disk path
	else                               //     "C:\folder"
		return make(L"\\\\?\\", path); // "\\?\C:\folder"
}
