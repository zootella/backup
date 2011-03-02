
// Functions in job control.cpp
void JobReset();
void JobStart();
bool JobContinue();
void JobStop();
void JobDone();

// Functions in job get.cpp
string JobTasks();
displayitem JobDisplay();

// Functions in job set.cpp
string JobTask(read r);
void JobFileCount();
void JobFileError(read r);
void JobFolderCount();
void JobFolderError(read r);
void JobCompareCount();
void JobCompareError(read r);
void JobError(read r);

// Functions in platform disk.cpp
bool DiskFolder(read path, bool create, bool write);
bool DiskFolderCheck(read path, bool create);
bool DiskDeleteFolder(read path);
bool DiskDeleteFile(read path);
bool DiskMakeFolder(read path);
bool DiskCopyFile(read source, read destination);
bool DiskIsFolder(read path);
bool DiskIsFile(read path);
bool DiskSameFile(read path1, finditem *f2);
bool DiskCompareFile(read path1, read path2);

// Functions in platform number.cpp
big Combine(DWORD high, DWORD low);
DWORD High(big b);
DWORD Low(big b);

// Functions in platform paint.cpp
COLORREF ColorMix(COLORREF color1, int amount1, COLORREF color2, int amount2);
brushitem CreateBrush(COLORREF color);
HFONT CreateFont(read face, int points);
void PaintMessage(HWND window);
void PaintFill(deviceitem *device, sizeitem size, HBRUSH brush);
void PaintBorder(deviceitem *device, sizeitem size, HBRUSH brush);
void PaintText(deviceitem *device, read r, sizeitem size);

// Functions in platform registry.cpp
string RegistryRead(HKEY root, read path, read name);
void RegistryWrite(HKEY root, read path, read name, read value);

// Functions in platform string.cpp
string make(read r1 = L"", read r2 = L"", read r3 = L"", read r4 = L"", read r5 = L"", read r6 = L"", read r7 = L"", read r8 = L"", read r9 = L"");
string upper(read r);
string lower(read r);
int number(read r);
string numerals(int number);
bool is(read r);
bool isblank(read r);
bool same(read r1, read r2, matching m = Different);
int compare(read r1, read r2, matching m = Different);
bool starts(read r, read t, matching m = Different);
bool trails(read r, read t, matching m = Different);
bool has(read r, read t, matching m = Different);
int find(read r, read t, direction d = Forward, matching m = Different);
string parse(read r, read t1, read t2, matching m = Different);
string before(read r, read t, direction d = Forward, matching m = Different);
string after(read r, read t, direction d = Forward, matching m = Different);
void split(read r, read t, string *b, string *a, direction d = Forward, matching m = Different);
string replace(read r, read t1, read t2, matching m = Different);
string clip(read r, int startindex, int characters = -1);
string on(read r, read t, direction d = Forward, matching m = Different);
string off(read r, read t, direction d = Forward, matching m = Different);
string trim(read r, read t1 = L"", read t2 = L"", read t3 = L"");
string saynumber(int number, read name);
string insertcommas(read r);
string saytime(DWORD time);

// Functions in platform window.cpp
void Report(read r);
void BeginThread(LPVOID function);
void WindowTextSet(HWND window, read r);
string WindowTextGet(HWND window);
void EditAppend(HWND window, read r);
void EditScroll(HWND window);
HWND WindowCreateEdit(bool scrollbars, bool capacity);
HWND WindowCreateButton(read r);
HWND WindowCreate(read name, read text, DWORD style, int size, HWND parent, HMENU menu);
void WindowSize(HWND window, sizeitem size);
void WindowEdit(HWND window, boolean edit);
HMENU MenuLoad(read name, int index);
UINT MenuShow(HMENU menu, int x, int y);
string DialogBrowse(read display);

// Functions in task big.cpp
DWORD WINAPI Tasks();
void TaskDelete(read path);
void TaskCopy(read source, read destination);
void TaskCompare(read source, read destination);
void TaskUpdate(read source, read destination, bool compare);

// Functions in task small.cpp
void TaskDeleteFolder(read path);
void TaskDeleteFile(read path);
void TaskCopyFolder(read source, read destination, bool compare);
void TaskMakeFolder(read path);
void TaskCopyFile(read source, read destination, bool compare);
void TaskCompareFolder(read source, read destination);
void TaskCompareFile(read source, read destination);
void TaskUpdateClear(read source, read destination);
void TaskUpdateFill(read source, read destination, bool compare);

// Functions in user button.cpp
void ButtonClear();
void ButtonDelete();
void ButtonCopy();
void ButtonCompare();
void ButtonUpdate();
void ButtonUpdateCompare();

// Functions in user paint.cpp
void PaintCreate();
void Paint();

// Functions in user update.cpp
void WindowUpdate();

// Functions in user window.cpp
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, PSTR command, int show);
LRESULT CALLBACK MainWinProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

// Functions in (test).cpp
void Test();
