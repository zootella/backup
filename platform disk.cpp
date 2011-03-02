
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
//extern handleitem Handle;

// True if path is to a folder, drive root, or network share
// Set create to true to make folders if necessary
// Set write to true to also confirm we can write to it
bool DiskFolder(read path, bool create, bool write) {

	// A trailing backslash is not allowed, give this "C:", not "C:\"
	if (trails(path, L"\\")) return false;

	// Strings to split
	string s = path;
	string b, build;

	// Drive path, like "C:" or "C:\folder\subfolder"
	if (clip(s, 1, 1) == L":") {

		// Loop making build like "C:", "C:\folder", "C:\folder\subfolder"
		while (is(s)) {
			split(s, L"\\", &b, &s);
			if (is(build)) build += L"\\";
			build += b;
			if (!DiskFolderCheck(build, create)) return false; // Make it a folder
		}

	// Network path, like "\\computer\share" or "\\computer\share\folder\subfolder"
	} else if (starts(s, L"\\\\")) {

		// Make build like "\\computer\share"
		s = clip(s, 2);
		string computer, share, build;
		split(s, L"\\", &computer, &s);
		split(s, L"\\", &share, &s);
		build = L"\\\\" + computer + L"\\" + share;
		if (!DiskFolderCheck(build, create)) return false; // Make it a folder

		// Loop making build like "\\computer\share\folder", "\\computer\share\folder\subfolder"
		while (is(s)) {
			split(s, L"\\", &b, &s);
			build += L"\\" + b;
			if (!DiskFolderCheck(build, create)) return false; // Make it a folder
		}

	// Invalid path
	} else {
		return false;
	}

	// Also confirm writing there works
	if (write) {
		string tick = make(path, L"\\", numerals(GetTickCount())); // Try making and deleting a subfolder
		if (!DiskMakeFolder(tick)) return false;
		if (!DiskDeleteFolder(tick)) return false;
	}

	// Everything worked
	return true;
}

// Look for a folder at path, set create true to try to make one there if necessary
// True if there's a folder at path
bool DiskFolderCheck(read path, bool create) {

	// Check or make one folder
	if (DiskIsFolder(path)) return true; // Already a folder, return true
	if (create) return DiskMakeFolder(path); // Try to make the folder, return false on error
	else return false; // Can't make the folder and no folder there
}

// Delete the empty folder at path
bool DiskDeleteFolder(read path) {

	// Remove a read-only attribute that would prevent delete from working
	SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);

	// Delete the folder and check the result
	int result = RemoveDirectory(path);
	DWORD error = GetLastError();
	return result != 0 || error == ERROR_FILE_NOT_FOUND;
}

// Delete the file at path
bool DiskDeleteFile(read path) {

	// Remove a read-only attribute that would prevent delete from working
	SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);

	// Delete the file and check the result
	int result = DeleteFile(path);
	DWORD error = GetLastError();
	return result != 0 || error == ERROR_FILE_NOT_FOUND;
}

// Make a new folder at path, or confirm one is already there
bool DiskMakeFolder(read path) {
	if (DiskIsFolder(path)) return true; // Already there
	return CreateDirectory(path, NULL) != 0; // NULL to use default security attributes
}

// Copy the file at source to the available path destination, will not overwrite
bool DiskCopyFile(read source, read destination) {
	return CopyFile(source, destination, true) != 0; // true to not overwrite
}

// True if path is to a folder on the disk
// Also returns true for drive roots and network shares like "C:" and "\\computer\share"
bool DiskIsFolder(read path) {

	// Only return true if we can get the file attributes and they include the directory flag
	DWORD d = GetFileAttributes(path);
	return d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);
}

// True if path is to a file on the disk
bool DiskIsFile(read path) {

	// Only return true if we can get the file attributes and they don't include the directory flag
	DWORD d = GetFileAttributes(path);
	return d != INVALID_FILE_ATTRIBUTES && !(d & FILE_ATTRIBUTE_DIRECTORY);
}

// True if there's a file at path1 with the same size and last modified date as f2 found
bool DiskSameFile(read path1, finditem *f2) {

	// Get information about the path
	finditem f1(path1, false);
	if (!f1.found() || f1.folder()) return false; // File not found or other error, or is a folder

	// Only return true if the sizes and last modified dates are the same
	return
		f1.info.nFileSizeLow  == f2->info.nFileSizeLow  &&
		f1.info.nFileSizeHigh == f2->info.nFileSizeHigh &&
		f1.info.ftLastWriteTime.dwLowDateTime  == f2->info.ftLastWriteTime.dwLowDateTime &&
		f1.info.ftLastWriteTime.dwHighDateTime == f2->info.ftLastWriteTime.dwHighDateTime;
}

// Compare the files at the given paths
// Returns true if same, false if different or any error
bool DiskCompareFile(read path1, read path2) {

	// Get information about both paths
	finditem f1(path1, false);
	finditem f2(path2, false);

	// Make sure they are both found, not folders, and the same size
	if (!f1.found() || !f2.found()) return false;
	if (f1.folder() || f2.folder()) return false;
	if (f1.info.nFileSizeLow != f2.info.nFileSizeLow || f1.info.nFileSizeHigh != f2.info.nFileSizeHigh) return false;

	// Two empty files are the same
	if (!f1.info.nFileSizeLow && !f2.info.nFileSizeLow && !f1.info.nFileSizeHigh && !f2.info.nFileSizeHigh) return true;

	// Compare each byte of data
	mapitem map1, map2;
	if (!map1.open(path1)) return false; // Unable to open them
	if (!map2.open(path2)) return false;
	while (true) {
		if (!map1.set()) return false; // Unable to view the next chunk
		if (!map2.set()) return false;
		if (memcmp(map1.view, map2.view, Low(map1.s)) != 0) return false; // Found bytes that don't match
		if (map1.done()) return true; // Nothing after this block
	}
}
