
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
	s = LogPath(L"hello");
	s = LogPathError();
	s = LogPathHash(L"E:\\Desk\\test");



}
