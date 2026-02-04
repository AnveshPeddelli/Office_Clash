#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Player
{
public:
	Player();

	void Update_Keys(float dt);
	void Update_Mouse();
	XMFLOAT3 GetRotation() const;

	void SetPosition(const XMFLOAT3& p);
	XMFLOAT3 GetPosition() const;
	float GetEyeHeight() const;

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;

	float speed;
	float eyeHeight;

	float yaw = 0;
	float pitch = 0;
};