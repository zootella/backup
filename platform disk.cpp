
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

// Prefix the given path before giving it to a system call so very long paths work
string LongPath(read path) {

	// Network path
	if (starts(path, L"\\\\")) {                     // Raw             "\\server\share\folder"
		return make(L"\\\\?\\UNC\\", clip(path, 2)); // Processed "\\?\UNC\server\share\folder"

	// Disk path
	} else {                             // Raw           "C:\folder"
		string s = path;
		if (trails(s, L":")) s += L"\\"; // Turn "C:" into "C:\" for GetFileAttributes
		return make(L"\\\\?\\", s);      // Processed "\\?\C:\folder"
	}
}

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
	SetFileAttributes(LongPath(path), FILE_ATTRIBUTE_NORMAL);

	// Delete the folder and check the result
	int result = RemoveDirectory(LongPath(path));
	DWORD error = GetLastError();
	return result != 0 || error == ERROR_FILE_NOT_FOUND;
}

// Delete the file at path
bool DiskDeleteFile(read path) {

	// Remove a read-only attribute that would prevent delete from working
	SetFileAttributes(LongPath(path), FILE_ATTRIBUTE_NORMAL);

	// Delete the file and check the result
	int result = DeleteFile(LongPath(path));
	DWORD error = GetLastError();
	return result != 0 || error == ERROR_FILE_NOT_FOUND;
}

// Make a new folder at path, or confirm one is already there
bool DiskMakeFolder(read path) {
	if (DiskIsFolder(path)) return true; // Already there
	return CreateDirectory(LongPath(path), NULL) != 0; // NULL to use default security attributes
}

// Copy the file at source to the available path destination, will not overwrite
bool DiskCopyFile(read source, read destination) {
	return CopyFile(LongPath(source), LongPath(destination), true) != 0; // true to not overwrite
}

// True if path is to a folder on the disk
// Also returns true for drive roots and network shares like "C:" and "\\computer\share"
bool DiskIsFolder(read path) {

	// Only return true if we can get the file attributes and they include the directory flag
	DWORD d = GetFileAttributes(LongPath(path));
	return d != INVALID_FILE_ATTRIBUTES && (d & FILE_ATTRIBUTE_DIRECTORY);
}

// True if path is to a file on the disk
bool DiskIsFile(read path) {

	// Only return true if we can get the file attributes and they don't include the directory flag
	DWORD d = GetFileAttributes(LongPath(path));
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
	if (!map1.openfile(path1) || !map1.openmap()) return false; // Unable to open them
	if (!map2.openfile(path2) || !map2.openmap()) return false;
	while (true) {
		if (!map1.set()) return false; // Unable to view the next chunk
		if (!map2.set()) return false;
		if (memcmp(map1.view, map2.view, Low(map1.s)) != 0) return false; // Found bytes that don't match
		if (map1.done()) return true; // Nothing after this block
	}
}

// Hash the file at path, saving the hash value in base 16 in the given string
bool DiskHashFile(read path, string *s) {

	// Open the file and find out how big it is
	mapitem map;
	if (!map.openfile(path)) return false;

	// Empty file
	if (!map.size) {

		// The hash value of no data
		*s = L"da39a3ee5e6b4b0d3255bfef95601890afd80709";
		return true;
	}

	// Open the mapping
	if (!map.openmap()) return false;

	// Access the cryptographic service provider
	if (!Handle.provider) { // Runs once each time the program runs, the first time this function is called
		if (!CryptAcquireContext(&Handle.provider, NULL, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) return false;
	}

	// Prepare to hash some data
	HCRYPTHASH hash;
	if (!CryptCreateHash(Handle.provider, CALG_SHA1, 0, 0, &hash)) return false;

	// Hash each byte of data
	while (true) {
		if (!map.set()) { CryptDestroyHash(hash); return false; } // View the next block

		// Hash the next block
		if (!CryptHashData(hash, (byte *)map.view, (DWORD)map.s, 0)) { CryptDestroyHash(hash); return false; }

		// Nothing after this block
		if (map.done()) {

			// Get the hash value
			BYTE value[20];
			DWORD size = 20;
			if (!CryptGetHashParam(hash, HP_HASHVAL, value, &size, 0)) { CryptDestroyHash(hash); return false; }

			// Convert the hash value into base 16 text
			WCHAR bay[MAX_PATH];
			for (int i = 0; i < 20; i++) {
				wsprintf(bay + (i * 2), L"%02x", value[i]); // Each call writes two wide characters and a null terminator
			}

			// Save it in the given string
			*s = bay;

			// Erase the hash
			if (!CryptDestroyHash(hash)) return false;
			return true;
		}
	}
}
