#include "graphics.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//Window callback
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Graphics::initWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"DirectXWindowsClass";

	RegisterClass(&wc);

	hwnd = CreateWindowEx(0, wc.lpszClassName, L"DirectX Client", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nCmdShow);
	return true;
}

bool Graphics::initDirectX()
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		nullptr,
		&context
	);

	if (FAILED(hr))
		return false;

	ID3D11Texture2D* backBuffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
	backBuffer->Release();

	context->OMSetRenderTargets(1, &renderTarget, nullptr);

	return true;
}

bool Graphics::initPipeline()
{
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(L"triangle.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
	if (FAILED(hr))
		return false;

	hr = D3DCompileFromFile(L"triangle.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, &errorBlob);
	if (FAILED(hr))
		return false;

	//creating shaders
	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);

	//input layout (matches vertex struct)
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

	//create vertex buffer (triangle geometry)
	Vertex triangle[] = {
		{  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 },
		{  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 },
		{ -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }
	};


	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(triangle);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = triangle;

	device->CreateBuffer(&bd, &data, &vertexBuffer);


	//Cleanup shader blobs
	vsBlob->Release();
	psBlob->Release();
	errorBlob->Release();

	return true;
}

bool Graphics::isRunning()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

void Graphics::renderFrame()
{
	float clearColor[4] = { 0.1f, 0.1f, 0.25f, 1.0f };
	context->ClearRenderTargetView(renderTarget, clearColor);

	//activating the shader pipeline
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	context->Draw(3, 0);

	swapChain->Present(1, 0);
}

void Graphics::shutdown()
{
	if (renderTarget) renderTarget->Release();
	if (swapChain) swapChain->Release();
	if (context) context->Release();
	if (device) device->Release();
	if (hwnd) DestroyWindow(hwnd);
}