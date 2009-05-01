
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

string make(read r1, read r2, read r3, read r4, read r5, read r6, read r7, read r8, read r9)
{
	// takes text
	// copies and catcatenates the text into a string
	// returns a string

	string s1 = r1, s2 = r2, s3 = r3, s4 = r4, s5 = r5, s6 = r6, s7 = r7, s8 = r8, s9 = r9;
	return(s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9);
}

string upper(read r)
{
	// takes text
	// uppercases the characters in it
	// returns a string

	string s = r;
	s.MakeUpper();
	return(s);
}

string lower(read r)
{
	// takes text
	// lowercases the characters in it
	// returns a string

	string s = r;
	s.MakeLower();
	return(s);
}

int number(read r)
{
	// takes text
	// reads the text as a number, handling a leading minus sign properly
	// returns the number, or 0 if given blank or alphabetic text

	return _tstoi(r); // USE FUNCTION LIKE ATOI
}

string numerals(int number)
{
	// takes a number
	// writes the minus sign adn number into text
	// returns a string

	character bay[MAX_PATH];
	_itot(number, bay, 10); // THE 10 IS FOR BASE TEN
	return bay;
}

bool is(read r)
{
	// takes text
	// determines if the text is not blank
	// returns true if it is, false if not

	if (r[0] != '\0') return(true);  // THE TEXT DOES NOT BEGIN WITH THE NULL TERMINATOR, AND IS NOT BLANK
	else              return(false);
}

bool isblank(read r)
{
	// takes text
	// determines if the text is blank
	// returns true if it is, false if not

	if (r[0] == '\0') return(true);  // THE TEXT BEGINS WITH THE NULL TERMINATOR, AND IS BLANK
	else              return(false);
}

bool same(read r1, read r2, matching m)
{
	// takes text r1 and r2, and matching
	// determines if r1 and r2 are the same
	// returns true if they are, false if they are not

	if (compare(r1, r2, m) == 0) return(true);  // THEY ARE THE SAME
	else                         return(false);
}

int compare(read r1, read r2, matching m)
{
	// takes text r1 and r2, and matching
	// calls lstrcmp or lstrcmpi on them
	// returns the result, which is negative if r1 is before r2, zero if they are the same, and positive if r1 is below r2

	if (m == Different) return(lstrcmp(r1, r2));  // CASE SENSITIVE, THE DEFAULT
	else                return(lstrcmpi(r1, r2)); // CASE INSENSITIVE, MATCHING CASES
}

bool starts(read r, read t, matching m)
{
	// takes text r and t, and matching
	// determins if the text starts with the tag
	// returns true if it does, false if it does not or if r or t are blank

	// USE FIND TO DETERMINE IF THE TAG IS AT THE START OF THE TEXT
	if (find(r, t, Forward, m) == 0) return(true);
	else                             return(false);
}

bool trails(read r, read t, matching m)
{
	// takes text r and t, and matching
	// determins if the text ends with the tag
	// returns true if it does, false if it does not or if r or t are blank

	// FIND THE LAST INSTANCE OF THE TAG
	int result;
	result = find(r, t, Reverse, m);
	if (result == -1) return(false); // TAG NOT FOUND

	if (result == length(r) - length(t)) return(true);  // TAG FOUND ON END
	else                                 return(false); // TAG FOUND ELSEWHERE
}

bool has(read r, read t, matching m)
{
	// takes text r and t, and matching
	// determins if the tag appears in the text
	// returns true if it does, false if it does not or if r or t are blank

	// USE FIND TO DETERMINE IF THE TAG EXISTS IN THE TEXT
	if (find(r, t, Forward, m) != -1) return(true);
	else                              return(false);
}

int find(read r, read t, direction d, matching m)
{
	// takes text r and t, and direction and matching
	// finds in r the first or last instance of t
	// returns the zero based index of t in r, or -1 if not found or if r or t are blank

	// GET LENGTHS
	int rlength, tlength;
	rlength = length(r);
	tlength = length(t);

	// IF EITHER IS BLANK OR R IS SHORTER THAN T, RETURN NOT FOUND
	if (!rlength || !tlength || rlength < tlength) return(-1);

	// VARIABLES FOR LOOP
	bool valid;             // VALID TELLS IF THE TAG IS BEING FOUND
	int rindex, tindex;     // SCANNING INDEXES
	character rchar, tchar; // CHARACTERS

	// SCAN RINDEX BETWEEN 0 AND RLENGTH - TLENGTH IN THE DESIRED DIRECTION
	if (d == Forward) rindex = 0;
	else              rindex = rlength - tlength;
	while (1) {
		if (d == Forward) { if (rindex > rlength - tlength) break; }
		else              { if (rindex < 0)                 break; }

		// SET VALID TRUE AND LOOK FOR THE TAG AT RINDEX, TO EITHER BREAK FALSE AT FIRST MISMATCH OR FINISH TRUE
		valid = true;
		for (tindex = 0; tindex <= tlength - 1; tindex++) {

			// GET THE PAIR OF CHARACTERS
			rchar = r[rindex + tindex];
			tchar = t[tindex];

			// UPPERCASE THEM IF MATCHING WAS REQUESTED
			if (m == Matching) {

				rchar = (character)CharUpper((write)(ULONG_PTR)MAKELONG((WORD)rchar, 0));
				tchar = (character)CharUpper((write)(ULONG_PTR)MAKELONG((WORD)tchar, 0));
			}

			// MISMATCH FOUND, SET FALSE AND BREAK
			if (rchar != tchar) { valid = false; break; }
		}

		// THE TAG WAS FOUND AT RINDEX, RETURN IT (DONE)
		if (valid) return(rindex);

		if (d == Forward) rindex++;
		else              rindex--;
	}

	// NOT FOUND
	return(-1);
}

string parse(read r, read t1, read t2, matching m)
{
	// takes text to parse, opening and closing tags, and matching
	// gets the text between the tags
	// returns a string

	// CLIP FROM AFTER THE FIRST TAG AND THEN BEFORE THE SECOND OR BLANK IF NOT FOUND
	string s;
	s = after(r, t1, Forward, m);
	if (has(s, t2, m)) s = before(s, t2, Forward, m);
	else               s = _T("");
	return(s);
}

string before(read r, read t, direction d, matching m)
{
	// takes text and tag, and direction and matching
	// splits the text before the tag
	// returns a string, the text from r if not found in either direction

	// USE SPLIT
	string b, a;
	split(r, t, &b, &a, d, m);
	return(b);
}

string after(read r, read t, direction d, matching m)
{
	// takes text and tag, and direction and matching
	// splits the text after the tag
	// returns a string, blank if not found in either direction

	// USE SPLIT
	string b, a;
	split(r, t, &b, &a, d, m);
	return(a);
}

void split(read r, read t, string *b, string *a, direction d, matching m)
{
	// takes text and tag, strings for before and after, and direction and matching
	// splits the text around the tag, writing text in before and after
	// returns nothing, puts all text in before and none in after if not found in either direction

	// FIND THE TAG IN THE TEXT USING THE DIRECTION AND MATCHING PASSED TO THIS FUNCTION
	int i;
	i = find(r, t, d, m);
	if (i == -1) {

		// NOT FOUND, ALL TEXT IS BEFORE AND NONE IS AFTER, DONE
		*b = r;
		*a = _T("");
		return;
	}

	// GET LENGTHS
	int rlength, tlength;
	rlength = length(r);
	tlength = length(t);

	// CLIP OUT BEFORE AND AFTER FROM A COPY OF R SO THAT R AND *B BEING THE SAME WON'T MANGLE *A
	string source = r;
	*b = clip(source, 0, i);
	*a = clip(source, i + tlength, rlength - tlength - i);
}

string replace(read r, read t1, read t2, matching m)
{
	// takes text, find and replace tags, and matching
	// makes a single pass down the text, replacing whole instances of the find text with the replacement text
	// returns a string

	// IF THE TEXT OR THE FIND TEXT IS BLANK, OR IF THE FIND TEXT IS NOT FOUND, RETURN THE TEXT UNCHANGED
	string top, left, bottom;
	top = r;
	if (isblank(r) || isblank(t1) || !has(r, t1, m)) return(top);

	// LOOP WHILE TOP HAS FIND
	while (has(top, t1, m)) {

		// F IS IN TOP
		split(top, t1, &left, &top, Forward, m);
		bottom += left + t2;
	}

	// F IS NOT IN TOP
	bottom += top;

	// RETURN BOTTOM TEXT
	return(bottom);
}

string clip(read r, int startindex, int characters)
{
	// takes text, a starting index, and a number of characters to copy or -1 for all
	// clips out that text, not reading outside of r
	// returns a string

	// GET THE LENGTH AND ELIMINATE SPECIAL CASES
	string s;
	int n = length(r);
	if (n == 0 || characters == 0) { return(s); }            // NO CHARACTERS TO CLIP OR NONE REQUESTED
	if (startindex < 0 || startindex > n - 1) { return(s); } // START INDEX OUTSIDE OF R

	// ADJUST LOCAL COPY OF CHARACTERS
	if (characters < 0 || characters > n - startindex) characters = n - startindex;

	// COPY THE TEXT INTO THE STRING, CROP IT, AND RETURN IT
	s = r;
	s = s.Mid(startindex, characters);
	return(s);
}

string on(read r, read t, direction d, matching m)
{
	// takes text and tag, and direction and matching
	// confirms the text starts or ends with the tag, inserting it if necessary
	// returns a string

	string s = r;
	if (d == Forward) { if (!starts(s, t, m)) s = t + s; } // CONFIRM THE TEXT STARTS WITH THE TAG
	else              { if (!trails(s, t, m)) s = s + t; } // CONFIRM THE TEXT ENDS WITH THE TAG
	return(s);
}

string off(read r, read t, direction d, matching m)
{
	// takes text and tag, and direction and matching
	// confirms the text does not start or end with the tag, removing multiple instances of it if necessary
	// returns a string

	string s = r;
	if (d == Forward) { while(starts(s, t, m)) s = clip(s, length(t), -1); }            // REMOVE THE TAG FROM THE START OF THE STRING
	else              { while(trails(s, t, m)) s = clip(s, 0, length(s) - length(t)); } // REMOVE THE TAG FROM THE END OF THE STRING
	return(s);
}

string trim(read r, read t1, read t2, read t3)
{
	// takes text and tags
	// removes the tags from the start and end of the text
	// returns a string

	// COPY THE TEXT INTO A STRING
	string s = r;

	// REMOVE THE TAGS FROM THE START OF THE STRING UNTIL GONE
	while (1) {

		if      (starts(s, t1)) s = clip(s, length(t1), -1);
		else if (starts(s, t2)) s = clip(s, length(t2), -1);
		else if (starts(s, t3)) s = clip(s, length(t3), -1);
		else                       break;
	}

	// REMOVE THE TAGS FROM THE END OF THE STRING UNTIL GONE
	while (1) {

		if      (trails(s, t1)) s = clip(s, 0, length(s) - length(t1));
		else if (trails(s, t2)) s = clip(s, 0, length(s) - length(t2));
		else if (trails(s, t3)) s = clip(s, 0, length(s) - length(t3));
		else                       break;
	}

	// RETURN THE STRING
	return(s);
}

string saynumber(int number, read name)
{
	// takes a number and a name
	// composes text like "14 apples"
	// returns a string

	if      (number == 0) return(make(_T("no "), name, _T("s")));                               // ZERO YIELDS "no [name]s"
	else if (number == 1) return(make(_T("1 "), name));                                         // ONE YIELDS "1 [name]"
	else                  return(make(insertcommas(numerals(number)), _T(" "), name, _T("s"))); // GREATER YIELDS "[number] [name]s"
}

string insertcommas(read r)
{
	// takes text
	// inserts commas between groups of three characters
	// returns a string

	// MAKE STRINGS
	string s, left, bottom;
	s = r;

	// MOVE DOWN COMMAS AND GROUPS OF 3 CHARACTERS
	while (length(s) > 3) {

		left = clip(s, length(s) - 3, 3);
		s = clip(s, 0, length(s) - 3);
		bottom = "," + left + bottom;
	}

	// MOVE DOWN THE LEADING GROUP OF UP TO 3 CHARACTERS AND RETURN THE STRING
	bottom = s + bottom;
	return(bottom);
}

string saytime(DWORD time)
{
	// takes a number of milliseconds
	// composes text to describe how long that is
	// returns a string

	// RETURN EXPLINATION FOR LESS THAN A SECOND
	if (time < 1000) return("less than a second");

	// CALCULATE THE HOUR, MINUTE, AND SECOND NUMBERS
	int hour, minute, second;
	hour = time / 3600000;
	minute = (time / 60000) - (hour * 60);
	second = (time / 1000) - (hour * 3600) - (minute * 60);

	// COMPOSE THE TEXT TO DISPLAY AND RETURN IT
	string s;
	if (hour) s += saynumber(hour, _T("hour"));
	if (hour || minute) s += " " + saynumber(minute, _T("minute"));
	s += _T(" ") + saynumber(second, _T("second"));
	return(trim(s, _T(" ")));
}
