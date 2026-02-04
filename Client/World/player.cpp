#include "player.h"
#include "../Input/input.h"
#include "../logger.h"

void Player::SetPosition(const XMFLOAT3& p)
{
	position = p;
	LOG_D("PlayerPosition: %f, %f, %f\n", position.x, position.y, position.z);
}

XMFLOAT3 Player::GetPosition() const { return position; };
//XMFLOAT3 Player::GetRotation() const { return rotation; };
float Player::GetEyeHeight() const { return eyeHeight; };

Player::Player()
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = {3.0f, 0.0f, 0.0f };

	speed = 5.0f;
	eyeHeight = 0.7f;
}

void Player::Update_Keys(float dt)
{
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMQuaternionRotationRollPitchYaw(0, yaw, 0));
	XMVECTOR right = XMVector3Rotate(XMVectorSet(1, 0, 0, 0), XMQuaternionRotationRollPitchYaw(0, yaw, 0));

	XMFLOAT3 f, r;
	XMStoreFloat3(&f, forward);
	XMStoreFloat3(&r, right);

	if (Input::IsKeyDown('W'))
	{
		position.x += f.x * speed * dt;
		position.z += f.z * speed * dt;
	}

	if (Input::IsKeyDown('S'))
	{
		position.x -= f.x * speed * dt;
		position.z -= f.z * speed * dt;
	}
	
	if (Input::IsKeyDown('A'))
	{
		position.x -= r.x * speed * dt;
		position.z -= r.z * speed * dt;
	}

	if (Input::IsKeyDown('D'))
	{
		position.x += r.x * speed * dt;
		position.z += r.z * speed * dt;
	}

}

void Player::Update_Mouse()
{
	const float sensitivity = 0.002f;

	yaw += Input::GetDeltaX() * sensitivity;
	pitch += Input::GetDeltaY() * sensitivity;

	//Clamp pitch so I can't flip
	if (pitch >  1.5f) pitch =  1.5f;
	if (pitch < -1.5f) pitch = -1.5f;
}

XMFLOAT3 Player::GetRotation() const
{
	return { pitch, yaw, 0 };
}
