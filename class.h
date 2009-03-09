
// Get information about a single file, or list the contents of a folder
class finditem {
public:

	// Members
	HANDLE handle;        // Search handle
	WIN32_FIND_DATA info; // Information about what we found this time
	string search;        // Query path

	// Takes a path to a file or folder, and false to get information about it, or true to list its contents
	finditem(read path, bool list = true) {

		// Set values to start the search
		handle = INVALID_HANDLE_VALUE;
		ZeroMemory(&info, sizeof(info));
		search = path;
		if (list) search += "\\*.*";

		// We're not going to use this in a loop, run the single search now
		if (!list) result();
	}

	// Clean up contents when this object goes out of scope
	~finditem() { close(); }

	// Loop calling this method to get results until it returns false
	bool result() {

		// Start the search
		if (handle == INVALID_HANDLE_VALUE) {
			handle = FindFirstFile(search, &info);
			if (handle == INVALID_HANDLE_VALUE) return false; // Not found or other error

			// Skip over "." and ".." at the start
			if (info.cFileName != string(".") && info.cFileName != string("..")) return true;
		}

		// Get the next file or folder in the list
		while (FindNextFile(handle, &info)) {

			// Skip over "." and ".." at the start
			if (info.cFileName != string(".") && info.cFileName != string("..")) return true;
		}

		// Done listing the files
		close();
		return false;
	}

	// True if this object found
	bool found() { return handle != INVALID_HANDLE_VALUE; } // A file or folder
	bool folder() { return (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0; } // A folder

	// Close the search we started
	void close() {
		if (handle != INVALID_HANDLE_VALUE) {
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}
};

// Map sections of a file into memory
class mapitem {
public:

	// Members
	HANDLE file, map, view;
	big size, i, s; // File size, and index and size that clip within it

	// Initialize member variables
	mapitem() {
		file = map = view = NULL; // Mark handles not used yet
		size = i = s = 0;
	}

	// Close handles when this object goes out of scope
	~mapitem() {
		if (view && view != INVALID_HANDLE_VALUE) UnmapViewOfFile(view);
		if (map  && map  != INVALID_HANDLE_VALUE) CloseHandle(map);
		if (file && file != INVALID_HANDLE_VALUE) CloseHandle(file);
	}

	// Methods
	bool open(read path);
	bool set();

	// True if we're on the last block
	bool done() {
		return i + s == size;
	}
};

// Wrap a registry key to close it
class registryitem {
public:

	// The handle to the registry key
	HKEY key;

	// Open a registry key and store its handle in this object
	bool Open(HKEY root, read path);
	void Close() { if (key) RegCloseKey(key); key = NULL; }

	// Make a new local object, and delete it when it goes out of scope
	registryitem() { key = NULL; }
	~registryitem() { Close(); }
};

// Make a local sectionitem object to stay in the critical section while you're in a function
class sectionitem {
public:
	sectionitem();
	~sectionitem();
};

// A rectangular size in the window
class sizeitem {
private:

	// Coordinates from client area origin, and width and height dimensions
	int mx, my, mw, mh;

public:

	// Get the dimensions
	int x() { return mx; }
	int y() { return my; }
	int w() { return mw; }
	int h() { return mh; }

	// Set the dimensions
	void x(int nx) { if (nx < 0) nx = 0; mx = nx; } // Replace a negative value with 0
	void y(int ny) { if (ny < 0) ny = 0; my = ny; }
	void w(int nw) { if (nw < 0) nw = 0; mw = nw; }
	void h(int nh) { if (nh < 0) nh = 0; mh = nh; }
	void set(int nx, int ny, int nw, int nh) { x(nx); y(ny); w(nw); h(nh); } // All at once
	void clear() { x(0); y(0); w(0); h(0); } // Clear all to 0

	// Adjust the dimensions
	void addx(int nx) { x(x() + nx); }
	void addy(int ny) { y(y() + ny); }
	void addw(int nw) { w(w() + nw); }
	void addh(int nh) { h(h() + nh); }

	// Right and bottom
	int r() { return x() + w(); }
	int b() { return y() + h(); }
	void r(int nr) { w(nr - x()); }
	void b(int nb) { h(nb - y()); }

	// New and convert in and out
	sizeitem() { clear(); }
	sizeitem(POINT p) { set(p); }
	sizeitem(RECT  r) { set(r); }
	sizeitem(SIZE  s) { set(s); }
	void set(POINT p) { x(p.x);    y(p.y);   w(0);             h(0);              }
	void set(RECT  r) { x(r.left); y(r.top); w(r.right - x()); h(r.bottom - y()); }
	void set(SIZE  s) { x(0);      y(0);     w(s.cx);          h(s.cy);           }
	POINT point() { POINT p; p.x = x(); p.y = y(); return p; }
	RECT rectangle() { RECT r; r.left = x(); r.top = y(); r.right = x() + w(); r.bottom = y() + h(); return r; }
	SIZE size() { SIZE s; s.cx = w(); s.cy = h(); return s; }

	// Make the size n pixels smaller around the edges
	void inside(int n = 1) { addx(n); addy(n); addw(-(2 * n)); addh(-(2 * n)); }

	// Determine if this size holds any pixels
	bool is() { return w() > 0 && h() > 0; }
};

// Hold a device context with information about how to put it away
enum deviceopen {
	DeviceNone,   // We haven't obtained the device context yet
	DeviceUse,    // The system provided a device context for the program to use
	DeviceCreate, // The program created a default display device context
	DeviceGet,    // The program asked the system for the window's device context
	DevicePaint,  // The program got the device context in response to a paint message
};
class deviceitem {
public:
~deviceitem();

	// Methods
	void OpenUse(HDC newdevice);
	void OpenCreate();
	void OpenGet(HWND newwindow);
	void OpenPaint(HWND newwindow);
	void Font(HFONT newfont);
	void Background(int newbackground);
	void FontColor(COLORREF newcolor);
	void BackgroundColor(COLORREF newcolor);

	// The device context
	deviceopen open;
	HDC device;
	HWND window;
	PAINTSTRUCT paint;

	// Painting tools to put back
	HFONT font;
	bool replacebackground, replacefontcolor, replacebackgroundcolor;
	COLORREF fontcolor, backgroundcolor;
	int background;

	// New
	deviceitem() {
		open = DeviceNone;
		device = NULL;
		window = NULL;
		font = NULL;
		replacebackground = replacefontcolor = replacebackgroundcolor = false;
	}
};

// Hold a color and brush
class brushitem {
public:

	// The color and brush
	COLORREF color;
	HBRUSH brush;

	// New
	brushitem() {
		color = 0;
		brush = NULL;
	}
};

// Information the window displays
class displayitem {
public:

	// Information the window shows
	string banner;  // Banner text, like "start" or "running"
	bool edittasks; // true if the tasks box is editable
	bool enableclear, enabletask, enablestart, enablestop, enablereset; // true if the buttons are enabled
	string status;  // Lines of status text
	string errors;  // Lines of error text

	// New
	displayitem() {
		edittasks = false;
		enableclear = enabletask = enablestart = enablestop = enablereset = false;
	}
};

// The program's global handle object
class handleitem {
public:

	// Program handles
	HINSTANCE instance; // Program instance handle
	CRITICAL_SECTION section; // Critical section for the job object

	// Windows and menus
	HWND window; // The main window on the screen
	HWND tasks, status, errors; // Child window edit controls
	HWND clear, task, start, stop, reset; // Child window buttons
	HMENU menu; // The Tasks menu

	// Painting tools
	brushitem white, black, blue, lightblue, yellow, lightyellow, green, lightgreen, red, lightred, middle;
	HFONT font, arial;

	// Our record of the information the window is displaying now
	displayitem display;
};

// The program's current job
enum jobstage {JobStageBefore, JobStageRunning, JobStageDone};
class jobitem {
public:

	// Status
	jobstage stage;        // Stage of the job
	bool     stop;         // True to stop early
	DWORD    time;         // The tick when the job started, or how long it took to finish
	string   tasks;        // Lines of text from the tasks box the job still has to do
	string   task;         // What the job is currently doing
	string   errors;       // Text list of errors
	int      error;        // Total number of errors
	int      folder;       // Number of folders done
	int      foldererror;  // Number of folder errors
	int      file;         // Number of files done
	int      fileerror;    // Number of file errors
	int      compare;      // Number of file comparisons done
	int      compareerror; // Number of file compare errors
};
