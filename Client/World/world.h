#pragma once
#include <vector>
#include <d3d11.h>

struct Vertex
{
	float x, y, z;
	float r, g, b, a;
};

struct WorldMesh
{
	ID3D11Buffer* vertexBuffer = nullptr;
	UINT vertexCount = 0;
};

class World
{
public:
	bool loadObj(ID3D11Device* device, const char* path);
	const WorldMesh& getMesh() const;

private:
	WorldMesh mesh;
};