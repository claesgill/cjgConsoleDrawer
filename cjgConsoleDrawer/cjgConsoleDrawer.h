#pragma once
#ifndef CJGCONSOLEDRAWER_H
#define CJGCONSOLEDRAWER_H
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <string>


class cjgConsoleDrawer {
	private:
		int screenWidth;
		int screenHeight;
		int fontWidth;
		int fontHeight;
		CHAR_INFO* consoleBuffer;
		HANDLE windowHandle;
		HANDLE windowHandleRead;
		float fElapsedTime;
		void Low(int x0, int y0, int x1, int y1, short character, short color) {
			int dx = x1 - x0;
			int dy = y1 - y0;

			int yi = 1;
			if (dy < 0) {
				yi = -1;
				dy = -dy;
			}
			int D = 2 * dy - dx;
			int y = y0;

			for (int x = x0; x <= x1; x++) {
				Draw(x, y, character, color);
				if (D > 0) {
					y = y + yi;
					D = D - 2 * dx;
				}
				D = D + 2 * dy;
			}
		}
		void High(int x0, int y0, int x1, int y1, short character, short color) {
			int dx = x1 - x0;
			int dy = y1 - y0;
			int xi = 1;
			if (dx < 0) {
				xi = -1;
				dx = -dx;
			}
			int D = 2 * dx - dy;
			int x = x0;

			for (int y = y0; y <= y1; y++) {
				Draw(x, y, character, color);
				if (D > 0) {
					x = x + xi;
					D = D - 2 * dy;
				}
				D = D + 2 * dx;
			}
		}

	public:
		std::wstring windowTitle;
		void CreateConsoleWindow(int width, int height, int fontW, int fontH) {
			screenWidth  = width;
			screenHeight = height;
			fontWidth    = fontW;
			fontHeight   = fontH;
			SMALL_RECT windowSize = { 0, 0, (SHORT)screenWidth - 1, (SHORT)screenHeight - 1 };
			
			COORD bufferSize = { (SHORT)screenWidth, (SHORT)screenHeight };
			consoleBuffer = new CHAR_INFO[screenWidth * screenHeight];
			memset(consoleBuffer, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

			windowHandle     = GetStdHandle(STD_OUTPUT_HANDLE);
			windowHandleRead = GetStdHandle(STD_INPUT_HANDLE);

			SetConsoleTitle("cjgConsoleDrawer - APP_NAME");

			SetConsoleWindowInfo(windowHandle, TRUE, &windowSize);
			SetConsoleScreenBufferSize(windowHandle, bufferSize);
			SetConsoleActiveScreenBuffer(windowHandle);

			CONSOLE_FONT_INFOEX cfi;
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = fontWidth;
			cfi.dwFontSize.Y = fontHeight;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;

			wcscpy_s(cfi.FaceName, L"Consolas");
			SetCurrentConsoleFontEx(windowHandle, false, &cfi);

		}
		void Update() {
			SMALL_RECT consoleWriteArea = { 0, 0, screenWidth - 1, screenHeight - 1 };
			WriteConsoleOutput(windowHandle, consoleBuffer, { (SHORT)screenWidth, (SHORT)screenHeight }, { 0,0 }, &consoleWriteArea);
		}
		void Draw(int x, int y, short character, short color) {
			if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
				consoleBuffer[x + screenWidth * y].Char.UnicodeChar = character;
				consoleBuffer[x + screenWidth * y].Attributes = color;
			}
		}
		void DrawLine(int x0, int y0, int x1, int y1, short character, short color) {
			if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
				if (x0 > x1) {
					Low(x1, y1, x0, y0, character, color);
				}
				else {
					Low(x0, y0, x1, y1, character, color);
				}
			}
			else {
				if (y0 > y1) {
					High(x1, y1, x0, y0, character, color);
				}
				else {
					High(x0, y0, x1, y1, character, color);
				}
			}
		}
		void DrawRect(int x0, int y0, int width, int height, short character, short color) {
			DrawLine(x0, y0, x0 + width, y0, character, color);
			DrawLine(x0 + width, y0, x0 + width, y0 + height, character, color);
			DrawLine(x0 + width, y0 + height, x0, y0 + height, character, color);
			DrawLine(x0, y0 + height, x0, y0, character, color);
		}
		void DrawTri(int x0, int y0, int x1, int y1, int x2, int y2, short character, short color) {
			DrawLine(x0, y0, x1, y1, character, color);
			DrawLine(x1, y1, x2, y2, character, color);
			DrawLine(x2, y2, x0, y0, character, color);
		}
		void ClearScreen() {
			for (int y = 0; y < screenHeight; y++) {
				for (int x = 0; x < screenWidth; x++) {
					Draw(x, y, 0x2588, 0);
				}
			}
		}
		int ScreenWidth() {
			return screenWidth;
		}
		int ScreenHeight() {
			return screenHeight;
		}
};

#endif // !CJGCONSOLEDRAWER_H
