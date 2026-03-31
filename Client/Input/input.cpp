#include "input.h"
#include <iostream>

HWND Input::hwnd = nullptr;
POINT Input::lastPos = {};
float Input::dx = 0;
float Input::dy = 0;
float Input::wheel = 0;
bool Input::mouseDown = false;

bool Input::keys[256] = {};

void Input::Init(HWND h)
{
	hwnd = h;
	GetCursorPos(&lastPos);
	ScreenToClient(hwnd, &lastPos);
}

void Input::OnMouseMove(LPARAM l)
{
	POINT p;
	p.x = GET_X_LPARAM(l);
	p.y = GET_Y_LPARAM(l);

	dx += float(p.x - lastPos.x);
	dy += float(p.y - lastPos.y);

	lastPos = p;
}

void Input::OnMouseDown(WPARAM)
{
	mouseDown = true;
}

void Input::OnMouseUp(WPARAM)
{
	mouseDown = false;
}

void Input::OnWheel(WPARAM w)
{
	wheel += GET_WHEEL_DELTA_WPARAM(w) / 120.0f;
}

bool Input::IsMouseDown()
{
	return mouseDown;
}

float Input::GetDeltaX()
{
	return dx;
}

float Input::GetDeltaY()
{
	return dy;
}

float Input::GetWheelDelta()
{
	return wheel;
}

void Input::EndFrame()
{
	dx = dy = wheel = 0;

	RECT rect;
	GetClientRect(hwnd, &rect);

	POINT center;
	center.x = (rect.right - rect.left) / 2;
	center.y = (rect.bottom - rect.top) / 2;

	ClientToScreen(hwnd, &center);
	SetCursorPos(center.x, center.y);

	lastPos.x = (rect.right - rect.left) / 2;
	lastPos.y = (rect.bottom - rect.top) / 2;
}

void Input::KeyDown(WPARAM key)
{
	if (key < 256)
		keys[key] = true;
}

void Input::KeyUp(WPARAM key)
{
	if (key < 256)
		keys[key] = false;
}

bool Input::IsKeyDown(int key)
{
	if (key < 256)
		return keys[key];

	return false;
}