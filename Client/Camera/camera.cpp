#include "camera.h"
#include "../Input/input.h"
#include <algorithm>

void Camera::Update()
{
	if (Input::IsMouseDown())
	{
		yaw += Input::GetDeltaX() * 0.005f;
		pitch += Input::GetDeltaY() * 0.005f;
	}

	distance -= Input::GetWheelDelta() * 3.0f;

	pitch = std::clamp(pitch, -1.5f, 1.5f);
	distance = std::clamp(distance, 5.0f, 500.0f);

	

	pos.x = target.x + distance * cosf(pitch) * sinf(yaw);
	pos.y = target.y + distance * sinf(pitch);
	pos.z = target.z + distance * cosf(pitch) * cosf(yaw);
}

void Camera::Focus(XMFLOAT3 t)
{
	target = t;
}

XMMATRIX Camera::GetView() const
{
	//return XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMVectorSet(0, 1, 0, 0));
	return view;
}

XMMATRIX Camera::GetProjection(float aspect) const
{
	return XMMatrixPerspectiveFovLH(XM_PIDIV4, aspect, 0.1f, 5000.0f);
}

void Camera::AttachToPlayer(Player& p)
{
	XMFLOAT3 pos = p.GetPosition();
	XMFLOAT3 rot = p.GetRotation();
	float eyeHeight = p.GetEyeHeight();

	XMVECTOR eye = XMLoadFloat3(&pos) + XMVectorSet(0, eyeHeight, 0, 0);
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMQuaternionRotationRollPitchYaw(rot.x, rot.y, 0));

	view = XMMatrixLookAtLH(eye, eye + forward, XMVectorSet(0, 1, 0, 0));
}
