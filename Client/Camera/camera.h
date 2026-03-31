#pragma once
#include <DirectXMath.h>
#include "../World/player.h"

using namespace DirectX;

class Camera
{
public:
	void Update();
	void Focus(XMFLOAT3 target);
	void Clear();
	void AttachToPlayer(Player& p);

	XMMATRIX GetView() const;
	XMMATRIX GetProjection(float aspect) const;
	

private:
	float yaw = 0;
	float pitch = 0;
	float distance = 0;

	DirectX::XMMATRIX view;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 pos = { 0,0,-5 };
};