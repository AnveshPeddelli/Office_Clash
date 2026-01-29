#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../World/world.h"
#include <DirectXMath.h>
#include "../Camera/camera.h"

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;


class Graphics
{
public:
	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
	};
	struct CameraCB
	{
		XMFLOAT4X4 world;
		XMFLOAT4X4 view;
		XMFLOAT4X4 projection;
	};

public:
	bool initWindow(HINSTANCE hInstance, int nCmdShow);
	bool initDirectX();
	bool initPipeline();
	bool isRunning();
	void renderFrame();
	void Clear();
	void shutdown();
	void FocusOnWorld();

private:
	HWND hwnd = nullptr;
	bool running = true;

	//DirectX Objects
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTarget = nullptr;

	//Shader Objects
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* cameraBuffer = nullptr;

	ID3D11Texture2D* depthBuffer = nullptr;
	ID3D11DepthStencilView* depthView = nullptr;

	Camera camera;

	//World Obj
	World world;

};

