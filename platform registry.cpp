
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

// Open or create and open a registry key for reading and writing
// Returns false on error
bool registryitem::Open(HKEY root, read path) {

	// Make sure we were given a root key and path
	if (!root || isblank(path)) return false;

	// Open or create and open the key
	HKEY k;
	DWORD info;
	int result = RegCreateKeyEx(
		root,                    // Handle to open root key
		path,                    // Subkey name
		0,
		"",
		REG_OPTION_NON_VOLATILE, // Save information in the registry file
		KEY_ALL_ACCESS,          // Get access to read and write values in the key we're making and opening
		NULL,
		&k,                      // Put the opened or created key handle here
		&info);                  // Tells if the key was opened or created and opened
	if (result != ERROR_SUCCESS || !k) { Report("error regcreatekeyex"); return false; }

	// Save the open key in this object
	key = k;
	return true;
}

// Read text from the registry
// Returns blank on not found or error
string RegistryRead(HKEY root, read path, read name) {

	// Open the key
	registryitem registry;
	if (!registry.Open(root, path)) return "";

	// Get the size required
	DWORD size;
	int result = RegQueryValueEx(
		registry.key, // Handle to an open key
		name,         // Name of the value to read
		0,
		NULL,
		NULL,         // No data buffer, we're requesting the size
		&size);       // Required size in bytes including the null terminator
	if (result == ERROR_FILE_NOT_FOUND) return "";
	if (result != ERROR_SUCCESS) { Report("error regqueryvalueex"); return ""; }

	// Open a string
	string s;
	LPTSTR buffer = s.GetBuffer(size); // How many characters we'll write, including the null terminator

	// Read the binary data
	result = RegQueryValueEx(
		registry.key,   // Handle to an open key
		name,           // Name of the value to read
		0,
		NULL,
		(LPBYTE)buffer, // Data buffer, writes the null terminator
		&size);         // Size of data buffer in bytes
	s.ReleaseBuffer();
	if (result != ERROR_SUCCESS) Report("error regqueryvalueex");

	// Return the string
	return s;
}

// Write text to the registry
void RegistryWrite(HKEY root, read path, read name, read value) {

	// Open the key
	registryitem registry;
	if (!registry.Open(root, path)) return;

	// Set or make and set the text value
	int result = RegSetValueEx(
		registry.key,        // Handle to an open key
		name,                // Name of the value to set or make and set
		0,
		REG_SZ,              // Variable type is a null-terminated string
		(const byte *)value, // Address of the value data to load
		length(value) + 1);  // Size of the value data in bytes, add 1 to write the null terminator
	if (result != ERROR_SUCCESS) Report("error regsetvalueex");
}
