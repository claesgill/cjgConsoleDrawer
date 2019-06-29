#include "cjgConsoleDrawer.h"

int main() {

	// Construct window
	cjgConsoleDrawer console;
	console.CreateConsoleWindow(120, 120, 6, 6);

	int x = 0;
	int y = console.ScreenHeight();
	int i = 0;
	int k = 0;
	while (1) {
		console.ClearScreen();

		// Some checks for moving the line and triangle
		if (y <= console.ScreenHeight() && y != 0) { y--;								}
		if (x <  console.ScreenWidth()  && y == 0) { x++;   y = 0;						}
		if (x >= console.ScreenWidth()  && y == 0) { x = 0; y = console.ScreenHeight(); }

		// Drawing a triangle
		console.DrawTri(x, y, 100, 60, 10, 80, 0x2588, 220); 

		// Drawing a simple moving line
		console.DrawLine(x, y, console.ScreenWidth()-1, console.ScreenHeight()-1, 0x2588, rand() % 255);

		// Drawing a white rectangle as a bounding box
		console.DrawRect(0, 0, console.ScreenWidth()-1, console.ScreenHeight()-1, 0x2588, 255);

		// Update screen
		console.Update();
	}

	return 0;
}