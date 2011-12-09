
// bug, close it while it's copying, and the last file is full size but corrupt
// bug, unlock workstation and child windows don't paint entirely
// new feature, cancel in the middle of a compare
// new feature, close while running, have it stay on the screen until done
// new feature, quick compare task just looks at size and date
// new feature, use copyfileex to cancel in the middle
// removed feature, mark folders to skip
// removed feature, safe to delete flag

// Program build and name
#define PROGRAMTEST false
#define PROGRAMTITLE L"Backup 2011 December 9"

// Program settings
#define PULSE 100
#define STOPERRORS 1000
#define VIEWSIZE 67108864

// Registry locations
#define REGISTRYKEY  HKEY_CURRENT_USER
#define REGISTRYPATH L"Software\\Rocky Mountain Tools\\Backup"
#define REGISTRYNAME L"tasks"

// Timer
#define TIMER_PULSE 1

// Number types
typedef unsigned __int64 big;

// Text
#define length  lstrlenW // Rename function
typedef CString string;  // string is a CString object
typedef LPCWSTR read;    // read is a pointer to wide characters and a null terminator that will only be read

// Text options
enum direction {Forward,   Reverse};  // Default forward and the start, or reverse and the end
enum matching  {Different, Matching}; // Default case sensitive, or case insensitive matching
