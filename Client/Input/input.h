#pragma once
#include <Windows.h>
#include <windowsx.h>

class Input
{
public:
	static void Init(HWND hwnd);

	static void OnMouseMove(LPARAM lparam);
	static void OnMouseDown(WPARAM btn);
	static void OnMouseUp(WPARAM btn);
	static void OnWheel(WPARAM wparam);

	static bool IsMouseDown();
	static float GetDeltaX();
	static float GetDeltaY();
	static float GetWheelDelta();

	static void EndFrame();

private:
	static HWND hwnd;

	static POINT lastPos;
	static float dx, dy;
	static float wheel;
	static bool mouseDown;
};