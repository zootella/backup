
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

// Mix two colors in the given amounts
COLORREF ColorMix(COLORREF color1, int amount1, COLORREF color2, int amount2) {

	// Extract the individual primary color values
	int red1, red2, green1, green2, blue1, blue2;
	red1   = GetRValue(color1);
	red2   = GetRValue(color2);
	green1 = GetGValue(color1);
	green2 = GetGValue(color2);
	blue1  = GetBValue(color1);
	blue2  = GetBValue(color2);

	// Mix each color value using a weighted average, rounds fractions down
	int red, green, blue;
	red = green = blue = 0;
	if (amount1 + amount2) {

		red   = ((red1   * amount1) + (red2   * amount2)) / (amount1 + amount2);
		green = ((green1 * amount1) + (green2 * amount2)) / (amount1 + amount2);
		blue  = ((blue1  * amount1) + (blue2  * amount2)) / (amount1 + amount2);
	}

	// Return the mixed color
	return RGB(red, green, blue);
}

// Make a brushitem from the given color
// Returns a brushitem that must be deleted, or null on error
brushitem CreateBrush(COLORREF color) {

	// Create a brush of the solid color
	brushitem brush;
	brush.color = color;
	brush.brush = CreateSolidBrush(color);
	if (!brush.brush) Report(L"error createsolidbrush");

	// Return the brush color and handle
	return brush;
}

// Takes a font face name and point size
// Creates the font
// Returns a handle to it
HFONT CreateFont(read face, int points) {

	// Create the font
	LOGFONT info;
	ZeroMemory(&info, sizeof(info));
	info.lfHeight         = -points;                      // Point size, minus sign required
	info.lfWidth          = 0;                            // Default width
	info.lfEscapement     = 0;                            // Not rotated
	info.lfOrientation    = 0;
	info.lfWeight         = FW_NORMAL;                    // Normal, not bold
	info.lfItalic         = (byte)false;                  // Not italic
	info.lfUnderline      = (byte)false;                  // Not underlined
	info.lfStrikeOut      = (byte)false;                  // No strikeout
	info.lfCharSet        = ANSI_CHARSET;                 // Use ANSI characters
	info.lfOutPrecision   = OUT_DEFAULT_PRECIS;           // Default size precision
	info.lfClipPrecision  = CLIP_DEFAULT_PRECIS;          // Default clipping behavior
	info.lfQuality        = DEFAULT_QUALITY;              // Don't force antialiasing
	info.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE; // Only used if the font name is unavailable
	lstrcpy(info.lfFaceName, face);                       // Font name
	HFONT font = CreateFontIndirect(&info);
	if (!font) Report(L"error createfontindirect");
	return font;
}

// Repaint the window right now
void PaintMessage(HWND window) {

	// Invalidate the whole client area of the given window, and make it process a paint message right now
	InvalidateRect(window, NULL, false); // false to not wipe the window with the background color
	UpdateWindow(window); // Call the window procedure directly with a paint message right now
}

// Takes a device context, size, and brush
// Fills the rectangle with the color
void PaintFill(deviceitem *device, sizeitem size, HBRUSH brush) {

	// Make sure there are pixels to paint
	if (!size.is()) return;

	// Paint the rectangle
	RECT rectangle = size.rectangle();
	FillRect(device->device, &rectangle, brush); // Returns error if the computer is locked
}

// Paint a 1-pixel wide border inside the given size
void PaintBorder(deviceitem *device, sizeitem size, HBRUSH brush) {

	// Paint the 4 edges of the border
	sizeitem edge;
	edge = size; edge.w(1); PaintFill(device, edge, brush); edge.x(size.r() - 1); PaintFill(device, edge, brush);
	edge = size; edge.h(1); PaintFill(device, edge, brush); edge.y(size.b() - 1); PaintFill(device, edge, brush);
}

// Takes a deviceitem that has a font, text, and a bounding position and size
// Paints the text there
void PaintText(deviceitem *device, read r, sizeitem size) {

	// Paint the text, if the background is opaque, this will cause a flicker
	RECT rectangle = size.rectangle();
	if (!DrawText(device->device, r, -1, &rectangle, DT_NOPREFIX)) Report(L"error drawtext");
}
