#include "world.h"
#include "player.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

#include <iostream>

bool World::loadObj(ID3D11Device* device, const char* path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded);

	if (!scene || !scene->HasMeshes())
		return false;

	aiMesh* m = scene->mMeshes[0];

	std::vector<Vertex> vertices;
	vertices.reserve(m->mNumFaces * 3);

	for (unsigned i = 0; i < m->mNumFaces; i++)
	{
		aiFace& face = m->mFaces[i];

		for (unsigned j = 0; j < face.mNumIndices; j++)
		{
			unsigned idx = face.mIndices[j];
			aiVector3D v = m->mVertices[idx];

			vertices.push_back({ v.x, v.y, v.z, 0.7f, 0.7f, 0.7f, 1.0f });
		}
	}

	this->minP = { FLT_MAX, FLT_MAX, FLT_MAX };
	this->maxP = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (auto& v : vertices)
	{
		minP.x = std::min(minP.x, v.x);
		minP.y = std::min(minP.y, v.y);
		minP.z = std::min(minP.z, v.z);
		
		maxP.x = std::max(maxP.x, v.x);
		maxP.y = std::max(maxP.y, v.y);
		maxP.z = std::max(maxP.z, v.z);
	}

	center.x = (minP.x + maxP.x) * 0.5f;
	center.y = (minP.y + maxP.y) * 0.5f;
	center.z = (minP.z + maxP.z) * 0.5f;

	LOG_I("xmaxP: %f, %f, %f \n", maxP.x, maxP.y, maxP.z);
	LOG_I("xminP: %f, %f, %f \n", minP.x, minP.y, minP.z);
	LOG_I("center: %f, %f, %f \n", center.x, center.y, center.z);

	XMFLOAT3 size;
	size.x = maxP.x - minP.x;
	size.y = maxP.y - minP.y;
	size.z = maxP.z - minP.z;

	float largest = std::max(size.x, std::max(size.y, size.z));
	mesh.scale = 10.0f / largest;

	mesh.groundY = (maxP.y - center.y) * mesh.scale;

	LOG_I("Scale: %f\n", mesh.scale);
	LOG_I("GroundY: %f\n", mesh.groundY);


	//create GPU buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = UINT(vertices.size() * sizeof(Vertex));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	device->CreateBuffer(&bd, &data, &mesh.vertexBuffer);
	mesh.vertexCount = (UINT)vertices.size();

	//Spawn Player on top of stl
	Player& player = GetPlayer();
	player.SetPosition({ center.x, mesh.groundY + player.GetEyeHeight(), -center.z+10});

	LOG_I("Player spawned at Y: %f\n", mesh.groundY + player.GetEyeHeight());

	return true;
}

const WorldMesh& World::getMesh() const
{
	return mesh;
}

const XMFLOAT3 World::GetCenter() const
{
	return center;
}

void World::Update(float dt)
{
	player.Update_Mouse();
	player.Update_Keys(dt);
}

Player& World::GetPlayer()
{
	return player;
}

float World::GetGroundY() const
{
	return mesh.groundY;
}

float World::GetScale() const
{
	return mesh.scale;
}