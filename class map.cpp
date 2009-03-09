
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

// Map the file at path into memory
bool mapitem::open(read path) {

	// Open the file
	file = CreateFile(
		path,                      // Path and file name
		GENERIC_READ,              // Get read-only access
		FILE_SHARE_READ,           // Allow other calls and processes to read the file while it's open
		NULL,
		OPEN_EXISTING,             // Don't create a file
		FILE_FLAG_SEQUENTIAL_SCAN, // We plan to read the file front to end
		NULL);
	if (file == INVALID_HANDLE_VALUE) return false;

	// Find out how big it is
	DWORD high = 0;
	DWORD low = GetFileSize(file, &high);
	size = Combine(high, low);

	// Open the map
	map = CreateFileMapping(
		file,          // Handle to open file
		NULL,
		PAGE_READONLY, // Read-only access
		0,             // Map the whole file
		0,
		NULL);         // Don't specify a name for the object
	if (!map) return false;

	// Everything worked
	return true;
}

// Set the view on the first or next chunk of the file
// Returns false on error or done
bool mapitem::set() {

	// The last time we clipped out the final block
	if (i + s == size) return false;

	// Set i and s to clip out the next block
	i = i + s;
	s = VIEWSIZE;
	if (i + s > size) s = size - i; // The last block may be smaller

	// Change the view
	if (view && view != INVALID_HANDLE_VALUE) UnmapViewOfFile(view);
	view = MapViewOfFile(
		map,           // Handle to open map
		FILE_MAP_READ, // Read-only access
		High(i),       // Offset high
		Low(i),        // Offset low
		Low(s));       // Number of bytes to map
	if (!view) return false;

	// Everything worked
	return true;
}
