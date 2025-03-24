#include <Windows.h>
#include <iostream>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

using namespace std;

void ClearScreen() {
	HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc,hBrush);
	Rectangle(hdc, 0, 0, 1920, 1080);
	DeleteObject(hBrush);
}

int main() {
	ClearScreen();
	int x, y;
	cout << "Enter (x, y):\n";
	cin >> x >> y;
	Fish AFish(x, y);
	AFish.Show();
	system("pause");
	AFish.Hide();
}