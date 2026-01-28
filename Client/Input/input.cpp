#include "input.h"

HWND Input::hwnd = nullptr;
POINT Input::lastPos = {};
float Input::dx = 0;
float Input::dy = 0;
float Input::wheel = 0;
bool Input::mouseDown = false;

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
}

