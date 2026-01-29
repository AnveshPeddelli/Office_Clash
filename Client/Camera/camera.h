#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	void Update();
	void Focus(XMFLOAT3 target);
	void Clear();

	XMMATRIX GetView() const;
	XMMATRIX GetProjection(float aspect) const;

private:
	float yaw = 0;
	float pitch = 0;
	float distance = 80;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 pos = { 0,0,-5 };
};