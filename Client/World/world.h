#pragma once
#define NOMINMAX

#include "../logger.h"
#include "player.h"

#include <vector>
#include <algorithm>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
};

struct WorldMesh
{
	ID3D11Buffer* vertexBuffer = nullptr;
	UINT vertexCount = 0;

	float scale = 1.0f;
	float groundY = 0.0f;
};

class World
{
public:
	bool loadObj(ID3D11Device* device, const char* path);
	const WorldMesh& getMesh() const;
	float GetGroundY() const;
	float GetScale() const;

	const XMFLOAT3 GetCenter() const;

	void Update(float dt);
	Player& GetPlayer();


private:
	WorldMesh mesh;
	XMFLOAT3 minP;
	XMFLOAT3 maxP;

	XMFLOAT3 center;
	Player player;
};