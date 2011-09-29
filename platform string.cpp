
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

// Combine the given text into a string
string make(read r1, read r2, read r3, read r4, read r5, read r6, read r7, read r8, read r9) {

	string s1 = r1, s2 = r2, s3 = r3, s4 = r4, s5 = r5, s6 = r6, s7 = r7, s8 = r8, s9 = r9;
	return s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9;
}

// Write the minus sign and number into text
string numerals(int number, int width) {

	WCHAR bay[MAX_PATH];
	_itow_s(number, bay, MAX_PATH, 10); // The 10 is for base 10
	string s = bay;

	if (width > 0) { // Minimum width requested
		while (length(s) < width) { // Add 0s at the start until it's wide enough
			s = L"0" + s;
		}
	}
	return s;
}

// True if r has text
bool is(read r) {

	// The text does not begin with the null terminator, and is not blank
	return r[0] != L'\0';
}

// True if r is blank
bool isblank(read r) {

	// The text begins with the null terminator, and is blank
	return r[0] == L'\0';
}

// True if r starts with t, false if it does not or if r or t are blank
bool starts(read r, read t, matching m) {

	// Use find to determine if the tag is at the start of the text
	return find(r, t, Forward, m) == 0;
}

// True if r ends with t, false if it does not or if r or t are blank
bool trails(read r, read t, matching m) {

	// Find the last instance of the tag
	int result = find(r, t, Reverse, m);
	if (result == -1) return false; // Tag not found

	if (result == length(r) - length(t)) return true;  // Tag found on end
	else                                 return false; // Tag found elsewhere
}

// True if r has t, false if it does not or if r or t are blank
bool has(read r, read t, matching m) {

	// Use find to determine if the tag exists in the text
	return find(r, t, Forward, m) != -1;
}

// Find in r the first or last zero based index of t, -1 if not found or if r or t are blank
int find(read r, read t, direction d, matching m) {

	// Get lengths
	int rlength = length(r);
	int tlength = length(t);

	// If either is blank or r is shorter than t, return not found
	if (!rlength || !tlength || rlength < tlength) return -1;

	// Variables for loop
	bool valid;         // Valid tells if the tag is being found
	int rindex, tindex; // Scanning indexes
	WCHAR rchar, tchar; // Characters

	// Scan rindex between 0 and rlength - tlength in the desired direction
	if (d == Forward) rindex = 0;
	else              rindex = rlength - tlength;
	while (1) {
		if (d == Forward) { if (rindex > rlength - tlength) break; }
		else              { if (rindex < 0)                 break; }

		// Set valid true and look for the tag at rindex, to either break false at first mismatch or finish true
		valid = true;
		for (tindex = 0; tindex <= tlength - 1; tindex++) {

			// Get the pair of characters
			rchar = r[rindex + tindex];
			tchar = t[tindex];

			// Uppercase them if matching was requested
			if (m == Matching) {

				rchar = (WCHAR)CharUpper((LPWSTR)(ULONG_PTR)MAKELONG((WORD)rchar, 0));
				tchar = (WCHAR)CharUpper((LPWSTR)(ULONG_PTR)MAKELONG((WORD)tchar, 0));
			}

			// Mismatch found, set false and break
			if (rchar != tchar) { valid = false; break; }
		}

		// The tag was found at rindex, return it, done
		if (valid) return rindex;

		if (d == Forward) rindex++;
		else              rindex--;
	}

	// Not found
	return -1;
}

// Takes text and tag, and direction and matching
// Splits the text before the tag
// Returns a string, the text from r if not found in either direction
string before(read r, read t, direction d, matching m) {

	// Use split
	CString b, a;
	split(r, t, &b, &a, d, m);
	return b;
}

// Takes text and tag, and direction and matching
// Splits the text after the tag
// Returns a string, blank if not found in either direction
string after(read r, read t, direction d, matching m) {

	// Use split
	CString b, a;
	split(r, t, &b, &a, d, m);
	return a;
}

// Takes text and tag, strings for before and after, and direction and matching
// Splits the text around the tag, writing text in before and after
// Returns nothing, puts all text in before and none in after if not found in either direction
void split(read r, read t, string *b, string *a, direction d, matching m) {

	// Find the tag in the text using the direction and matching passed to this function
	int i = find(r, t, d, m);
	if (i == -1) {

		// Not found, all text is before and none is after, done
		*b = r;
		*a = L"";
		return;
	}

	// Get lengths
	int rlength = length(r);
	int tlength = length(t);

	// Clip out before and after from a copy of r so that r and *b being the same won't mangle *a
	string source = r;
	*b = clip(source, 0, i);
	*a = clip(source, i + tlength, rlength - tlength - i);
}

// Takes text, a starting index, and a number of characters to copy or -1 for all
// Clips out that text, not reading outside of r
string clip(read r, int startindex, int characters) {

	// Get the length and eliminate special cases
	string s;
	int n = length(r);
	if (n == 0 || characters == 0) { return s; }            // No characters to clip or none requested
	if (startindex < 0 || startindex > n - 1) { return s; } // Start index outside of r

	// Adjust local copy of characters
	if (characters < 0 || characters > n - startindex) characters = n - startindex;

	// Copy the text into the string, crop it, and return it
	s = r;
	s = s.Mid(startindex, characters);
	return s;
}

// Takes text and tag, and direction and matching
// Confirms the text does not start or end with the tag, removing multiple instances of it if necessary
string off(read r, read t, direction d, matching m) {

	string s = r;
	if (d == Forward) { while(starts(s, t, m)) s = clip(s, length(t), -1); }            // Remove the tag from the start of the string
	else              { while(trails(s, t, m)) s = clip(s, 0, length(s) - length(t)); } // Remove the tag from the end of the string
	return s;
}

// Remove the tags from the start and end of the given text
string trim(read r, read t1, read t2, read t3) {

	// Copy the text into a string
	string s = r;

	// Remove the tags from the start of the string until gone
	while (true) {

		if      (starts(s, t1)) s = clip(s, length(t1), -1);
		else if (starts(s, t2)) s = clip(s, length(t2), -1);
		else if (starts(s, t3)) s = clip(s, length(t3), -1);
		else break;
	}

	// Remove the tags from the end of the string until gone
	while (true) {

		if      (trails(s, t1)) s = clip(s, 0, length(s) - length(t1));
		else if (trails(s, t2)) s = clip(s, 0, length(s) - length(t2));
		else if (trails(s, t3)) s = clip(s, 0, length(s) - length(t3));
		else break;
	}

	// Return the string
	return s;
}

// Compose text like "14 apples"
string saynumber(int number, read name) {

	if      (number == 0) return make(L"no ", name, L"s");                               // Zero yields "no [name]s"
	else if (number == 1) return make(L"1 ", name);                                      // One yields "1 [name]"
	else                  return make(insertcommas(numerals(number)), L" ", name, L"s"); // Greater yields "[number] [name]s"
}

// Insert commas between groups of three characters
string insertcommas(read r) {

	// Make strings
	string s, left, bottom;
	s = r;

	// Move down commas and groups of 3 characters
	while (length(s) > 3) {

		left = clip(s, length(s) - 3, 3);
		s = clip(s, 0, length(s) - 3);
		bottom = L"," + left + bottom;
	}

	// Move down the leading group of up to 3 characters and return the string
	bottom = s + bottom;
	return bottom;
}

// Given a number of milliseconds, compose text to describe how long that is
string saytime(DWORD time) {

	// Return explination for less than a second
	if (time < 1000) return L"less than a second";

	// Calculate the hour, minute, and second numbers
	int hour = time / 3600000;
	int minute = (time / 60000) - (hour * 60);
	int second = (time / 1000) - (hour * 3600) - (minute * 60);

	// Compose the text to display and return it
	string s;
	if (hour) s += saynumber(hour, L"hour");
	if (hour || minute) s += L" " + saynumber(minute, L"minute");
	s += L" " + saynumber(second, L"second");
	return trim(s, L" ");
}
