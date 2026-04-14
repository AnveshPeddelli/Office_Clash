#include "world.h"
#include "player.h"

#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

namespace
{
	int ParseObjIndex(const std::string& token, size_t vertexCount)
	{
		const size_t slashIndex = token.find('/');
		const std::string value = token.substr(0, slashIndex);
		if (value.empty())
			return 0;

		char* parseEnd = nullptr;
		long rawIndex = std::strtol(value.c_str(), &parseEnd, 10);
		if (parseEnd == value.c_str())
			return 0;

		if (rawIndex < 0)
			rawIndex = static_cast<long>(vertexCount) + rawIndex + 1;

		return static_cast<int>(rawIndex);
	}
}

bool World::loadObj(ID3D11Device* device, const std::filesystem::path& path)
{
	std::ifstream input(path);
	if (!input)
	{
		LOG_E("Failed to open world mesh: %ls\n", path.c_str());
		return false;
	}

	std::vector<XMFLOAT3> positions;
	std::vector<Vertex> vertices;

	std::string line;
	while (std::getline(input, line))
	{
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream stream(line);
		std::string recordType;
		stream >> recordType;

		if (recordType == "v")
		{
			XMFLOAT3 position{};
			if (stream >> position.x >> position.y >> position.z)
				positions.push_back(position);
			continue;
		}

		if (recordType != "f")
			continue;

		std::vector<int> faceIndices;
		std::string token;
		while (stream >> token)
		{
			const int parsedIndex = ParseObjIndex(token, positions.size());
			if (parsedIndex <= 0 || parsedIndex > static_cast<int>(positions.size()))
			{
				faceIndices.clear();
				break;
			}

			faceIndices.push_back(parsedIndex - 1);
		}

		if (faceIndices.size() < 3)
			continue;

		for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
		{
			const XMFLOAT3& a = positions[faceIndices[0]];
			const XMFLOAT3& b = positions[faceIndices[i]];
			const XMFLOAT3& c = positions[faceIndices[i + 1]];

			vertices.push_back({ a.x, a.y, a.z, 0.7f, 0.7f, 0.7f, 1.0f });
			vertices.push_back({ b.x, b.y, b.z, 0.7f, 0.7f, 0.7f, 1.0f });
			vertices.push_back({ c.x, c.y, c.z, 0.7f, 0.7f, 0.7f, 1.0f });
		}
	}

	if (vertices.empty())
	{
		LOG_E("No triangles were loaded from world mesh: %ls\n", path.c_str());
		return false;
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
	if (largest <= std::numeric_limits<float>::epsilon())
		return false;

	mesh.scale = 10.0f / largest;

	const XMFLOAT3 scaledMin = {
		(minP.x - center.x) * mesh.scale,
		(minP.y - center.y) * mesh.scale,
		(minP.z - center.z) * mesh.scale
	};
	const XMFLOAT3 scaledMax = {
		(maxP.x - center.x) * mesh.scale,
		(maxP.y - center.y) * mesh.scale,
		(maxP.z - center.z) * mesh.scale
	};

	mesh.groundY = scaledMax.y;

	LOG_I("Scale: %f\n", mesh.scale);
	LOG_I("GroundY: %f\n", mesh.groundY);


	//create GPU buffer
	if (mesh.vertexBuffer)
	{
		mesh.vertexBuffer->Release();
		mesh.vertexBuffer = nullptr;
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = UINT(vertices.size() * sizeof(Vertex));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices.data();

	device->CreateBuffer(&bd, &data, &mesh.vertexBuffer);
	mesh.vertexCount = (UINT)vertices.size();

	// Spawn in the transformed world space, a short distance in front of the mesh.
	Player& player = GetPlayer();
	const float depth = scaledMax.z - scaledMin.z;
	const float spawnDistance = std::max(5.0f, depth * 0.75f);
	const float eyeY = scaledMin.y + (scaledMax.y - scaledMin.y) * 0.6f;
	player.SetPosition({ 0.0f, eyeY - player.GetEyeHeight(), scaledMin.z - spawnDistance });

	LOG_I("Player spawned at: %f, %f, %f\n", 0.0f, eyeY - player.GetEyeHeight(), scaledMin.z - spawnDistance);

	return true;
}

void World::Shutdown()
{
	if (mesh.vertexBuffer)
	{
		mesh.vertexBuffer->Release();
		mesh.vertexBuffer = nullptr;
	}

	mesh.vertexCount = 0;
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
