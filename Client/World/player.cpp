#include "player.h"
#include "../Input/input.h"


void Player::SetPosition(const XMFLOAT3& p)
{
	position = p;
}

XMFLOAT3 Player::GetPosition() const { return position; };
XMFLOAT3 Player::GetRotation() const { return rotation; };
float Player::GetEyeHeight() const { return eyeHeight; };

Player::Player()
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };

	speed = 5.0f;
	eyeHeight = 1.7f;
}

void Player::Update(float dt)
{
	if (Input::IsKeyDown('W'))
		position.z += speed * dt;

	if (Input::IsKeyDown('S'))
		position.z -= speed * dt;
	
	if (Input::IsKeyDown('A'))
		position.x -= speed * dt;
	
	if (Input::IsKeyDown('D'))
		position.x += speed * dt;

}

