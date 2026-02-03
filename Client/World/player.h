#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Player
{
public:
	Player();

	void Update(float dt);

	void SetPosition(const XMFLOAT3& p);
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	float GetEyeHeight() const;

private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;

	float speed;
	float eyeHeight;
};