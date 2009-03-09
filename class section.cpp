
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

// Make a local sectionitem object to stay in the critical section while you're in a function
sectionitem::sectionitem() { EnterCriticalSection(&Handle.section); }
sectionitem::~sectionitem() { LeaveCriticalSection(&Handle.section); }
