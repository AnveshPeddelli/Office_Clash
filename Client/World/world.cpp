#include "world.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

	//create GPU buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = UINT(vertices.size() * sizeof(Vertex));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	device->CreateBuffer(&bd, &data, &mesh.vertexBuffer);
	mesh.vertexCount = (UINT)vertices.size();

	return true;
}

const WorldMesh& World::getMesh() const
{
	return mesh;
}