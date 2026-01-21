#pragma once
#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")



class Graphics
{
public:
	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
	};

public:
	bool initWindow(HINSTANCE hInstance, int nCmdShow);
	bool initDirectX();
	bool initPipeline();
	bool isRunning();
	void renderFrame();
	void shutdown();

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

	float posx = 0.0f;
	float posy = 0.0f;
	float camx = 0.0f;
	float camy = 0.0f;
};

