
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

// Show a message to the user
void Report(read r) {

	// Display the given text in a message box
	if (PROGRAMTEST) MessageBox(Handle.window, r, _T("Report"), MB_OK);
}

// Start a new thread to execute the given function
void BeginThread(LPVOID function) {

	// Create a new thread that runs the function
	DWORD info;
	HANDLE thread = (HANDLE)_beginthreadex(
		(void *)                          NULL,     // Use default security attributes
		(unsigned)                        0,        // Default initial stack size
		(unsigned (__stdcall *) (void *)) function, // Function where the new thread will begin execution
		(void *)                          NULL,     // Pass no parameter to the thread
		(unsigned)                        0,        // Create and start thread
		(unsigned *)                      &info);   // Writes thread identifier, cannot be null for Windows 95
	if (!thread) Report(_T("error beginthreadex"));

	// Tell the system this thread is done with the new thread handle
	if (!CloseHandle(thread)) Report(_T("error closehandle"));
}

// Have the given window display the given text
void WindowTextSet(HWND window, read r) {

	// Set the text to the window
	if (!SetWindowText(window, r)) Report(_T("error setwindowtext"));
}

// Get the text a window is displaying
// Returns blank if no text or any error
string WindowTextGet(HWND window) {

	// If the window handle is null, return blank
	if (!window) return _T("");

	// Find the required buffer size, in bytes
	int size =
		(int)SendMessage(window, WM_GETTEXTLENGTH, 0, 0) // The number of text characters
		+ 1; // Add 1 to have space in the buffer for the null terminator

	// Open a string
	string s;
	write buffer = s.GetBuffer(size);

	// Write the window text into the buffer
	GetWindowText( // Writes all the text and a null terminator
		window,    // Handle to window
		buffer,    // Destination buffer
		size);     // Size of the buffer

	// Close the string and return it
	s.ReleaseBuffer();
	return s;
}

// Add text in a new line at the bottom of an edit control, and scroll to view it
void EditAppend(HWND window, read r) {

	// Add the given text to a new line at the bottom
	string s = WindowTextGet(window); // Get the text that's already there, the user may have edited it
	if (is(s)) s += "\r\n";           // If not blank, start with a newline to make sure r will be on its own line
	s += r;                           // Add the given text
	WindowTextSet(window, s);         // Put the edited text back into the control

	// Scroll down to the bottom
	EditScroll(window);
}

// Scroll to the bottom of an edit control
void EditScroll(HWND window) {

	// Find out how many lines there are, and then scroll down that many
	LRESULT lines = SendMessage(window, EM_GETLINECOUNT, 0, 0);
	SendMessage(window, EM_LINESCROLL, 0, lines);
}

// Make a new edit window
HWND WindowCreateEdit(bool scrollbars, bool capacity) {

	// Prepare the style of the edit control window
	DWORD style =
		WS_CHILD |                       // Required for child windows
		ES_LEFT |                        // Left-align text
		ES_MULTILINE;                    // Hold multiple lines of text
	if (scrollbars) style |=
		WS_VSCROLL | WS_HSCROLL |        // Scroll bars
		ES_AUTOHSCROLL | ES_AUTOVSCROLL; // Scroll when the user enters text

	// Create the edit window
	HWND window = WindowCreate(_T("EDIT"), NULL, style, 0, Handle.window, NULL);

	// Have it use Tahoma, not the default system font
	SendMessage( 
		(HWND)window,        // Send the message to this window
		WM_SETFONT,          // Message to send
		(WPARAM)Handle.font, // Handle to font
		0);                  // Don't tell the control to immediately redraw itself

	// Expand its text capacity
	if (capacity) SendMessage(window, EM_LIMITTEXT, 0, 0);

	// Return the handle to the edit window we made
	return window;
}

// Make a new button
HWND WindowCreateButton(read r) {

	// Prepare the style of the button window
	DWORD style =
		WS_CHILD |     // Required for child windows
		BS_PUSHBUTTON; // Have the button send the window a message when the user clicks it

	// Create the edit window
	HWND window = WindowCreate(_T("BUTTON"), NULL, style, 0, Handle.window, NULL);

	// Title it
	WindowTextSet(window, r);

	// Have it use Tahoma, not the default system font
	SendMessage( 
		(HWND)window,        // Send the message to this window
		WM_SETFONT,          // Message to send
		(WPARAM)Handle.font, // Handle to font
		0);                  // Don't tell the control to immediately redraw itself

	// Return the handle to the button window we made
	return window;
}

// Make a new window
HWND WindowCreate(read name, read text, DWORD style, int size, HWND parent, HMENU menu) {

	// Create the window
	HWND window = CreateWindow(
		name,                   // System or registered window class name or class
		text,                   // Window text to show in the title bar, or null for no text
		style,                  // Window style
		size, size, size, size, // Window position and size
		parent,                 // Handle to parent window
		menu,                   // Menu handle or child window identification number
		Handle.instance,        // Program instance handle
		NULL);                  // No parameter
	if (!window) Report(_T("error createwindow"));
	return window;
}

// Move and resize the given window
void WindowSize(HWND window, sizeitem size) {

	// Move the window, false to not send a paint message
	if (!MoveWindow(window, size.x(), size.y(), size.w(), size.h(), false)) Report(_T("error movewindow"));
}

// Make an edit window editable or read only
void WindowEdit(HWND window, boolean edit) {

	// Send the window a messge to make it read only or editable
	SendMessage( 
		(HWND)window,   // Send the message to this window
		EM_SETREADONLY, // Message to send
		(WPARAM)!edit,  // true to set read only
		0);             // Not used, must be 0
}

// Takes a menu resource name and an index, like 0, to clip out the first submenu
// Loads the menu and clips out the submenu at the given index
// Returns the menu, or null on error
HMENU MenuLoad(read name, int index) {

	// Load the menu resource
	HMENU menus = LoadMenu(Handle.instance, name);
	if (!menus) { Report(_T("error loadmenu")); return NULL; }

	// Clip off the submenu at the given index, and return it
	HMENU menu = GetSubMenu(menus, index);
	if (!menu) Report(_T("error getsubmenu"));
	return menu;
}

// Show the given context menu
// Holds here while the user makes a choice
// Returns the choice the user made, or 0 if the user clicked outside or on error
UINT MenuShow(HMENU menu, int x, int y) {

	// Show the context menu and hold execution here until the user chooses from the menu or cancels it
	return TrackPopupMenu(
		menu,            // Handle to the menu to display
		TPM_NONOTIFY |   // Return the chosen menu item without sending messages to the main window
		TPM_RETURNCMD |
		TPM_RIGHTBUTTON, // Let the user click an item with the left or right mouse button
		x,               // Desired place to show it in screen coordinates
		y,
		0,
		Handle.window,
		NULL);
}

// Takes text to display in the dialog box
// Shows the user the browse for folder dialog box
// Returns the path the user chose, blank on cancel or error
string DialogBrowse(read display) {

	// Setup information for the dialog box
	character name[MAX_PATH];
	BROWSEINFO info;
	info.hwndOwner      = Handle.window;        // Handle to parent window for the browse dialog
	info.pidlRoot       = NULL;                 // Browse from the desktop
	info.pszDisplayName = name;                 // Write the name of the chosen folder here
	info.lpszTitle      = display;              // Text to display in the browse dialog
	info.ulFlags        = BIF_RETURNONLYFSDIRS; // Only allow file system folders
	info.lpfn           = NULL;                 // No callback function
	info.lParam         = 0;                    // No program-defined value
	info.iImage         = 0;                    // Will be filled with the icon index in the system image list

	// Show the user the browse for folder system dialog box
	CoInitialize(NULL);
	LPITEMIDLIST list = SHBrowseForFolder(&info); // Returns memory we are responsible for freeing
	if (!list) return _T(""); // The user clicked Cancel or the close X

	// Get the path the user chose
	character buffer[MAX_PATH];
	SHGetPathFromIDList(list, buffer);
	CoTaskMemFree(list); // Free the COM memory the system allocated for us
	return buffer;
}
