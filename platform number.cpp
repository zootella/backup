
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

// Combine high and low DWORDs into an unsigned 8 byte number
big Combine(DWORD high, DWORD low) {
	return ((big)high << 32) | (big)low;
}

// Get the high DWORD from an unsigned 8 byte number
DWORD High(big b) {
	return (DWORD)(b >> 32);
}

// Get the low DWORD from an unsigned 8 byte number
DWORD Low(big b) {
	return (DWORD)(b & 0xffffffff);
}
