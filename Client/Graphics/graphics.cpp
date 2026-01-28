#include "graphics.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "../Input/input.h"


//Window callback
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	case WM_MOUSEMOVE:
		Input::OnMouseMove(lParam);
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		Input::OnMouseDown(wParam);
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		Input::OnMouseUp(wParam);
		break;

	case WM_MOUSEWHEEL:
		Input::OnWheel(wParam);
		break;
		
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
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
	Input::Init(hwnd);
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

	//context->OMSetRenderTargets(1, &renderTarget, nullptr);

	//Depth Buffer Creation
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 1280;
	depthDesc.Height = 720;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
	device->CreateDepthStencilView(depthBuffer, nullptr, &depthView);
	


	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 1280.0f;
	viewport.Height = 720.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
	context->OMSetRenderTargets(1, &renderTarget, depthView);


	return true;
}

bool Graphics::initPipeline()
{

	//Compile Shaders
	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(L"C:/Users/p.anvesh/source/repos/Git/Office_Clash/Client/triangle.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
			MessageBoxA(0, (char*)errorBlob->GetBufferPointer(), "Shader Error", 0);
		return false;
	}
	hr = D3DCompileFromFile(L"triangle.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
			MessageBoxA(0, (char*)errorBlob->GetBufferPointer(), "Shader Error", 0);
		return false;
	}

	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);

	//Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = { 
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

	vsBlob->Release();
	psBlob->Release();

	////Create STATIC triangle (centered at origin)
	//Vertex triangle[3] = {
	//	{ 0.0f,  0.5f, 0.0f, 1, 0, 0, 1},
	//	{ 0.5f, -0.5f, 0.0f, 0, 1, 0, 1},
	//	{-0.0f, -0.5f, 0.0f, 0, 0, 1, 1}
	//};
	
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CameraCB);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	device->CreateBuffer(&cbd, nullptr, &cameraBuffer);

	//Loading World Obj
	world.loadObj(device, "C:/Users/p.anvesh/source/repos/Git/Office_Clash/BlenderObjs/World/world.obj");


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
	//Input camera update
	if (Input::IsMouseDown())
	{
		yaw += Input::GetDeltaX() * 0.005f;
		pitch += Input::GetDeltaY() * 0.005f;
	}

	//Zoom (Mouse wheel)
	distance -= Input::GetWheelDelta() * 5.0f;

	//clamp values
	pitch = max(-1.5f, min(1.5f, pitch));
	distance = max(5.0f, min(500.0f, distance));

	printf("Wheel: %f\n", Input::GetWheelDelta());


	float clearColor[4] = { 0.1f, 0.1f, 0.25f, 1.0f };
	context->ClearRenderTargetView(renderTarget, clearColor);
	context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	/*float speed = 0.01f;
	if (GetAsyncKeyState('A') & 0x8000) yaw -= speed;
	if (GetAsyncKeyState('D') & 0x8000) yaw += speed;
	if (GetAsyncKeyState('W') & 0x8000) pitch += speed;
	if (GetAsyncKeyState('S') & 0x8000) pitch -= speed;*/

	//clamp pitch
	pitch = max(-1.5f, min(1.5f, pitch));

	//Spherical to Cartesian
	camPos.x = distance * cosf(pitch) * sinf(yaw);
	camPos.y = distance * sinf(pitch);
	camPos.z = distance * cosf(pitch) * cosf(yaw);

	// WORLD (scale huge OBJ down)
	XMMATRIX worldMat = XMMatrixScaling(0.05f, 0.05f, 0.05f);

	// VIEW
	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat3(&camPos),
		XMLoadFloat3(&camTarget),
		XMLoadFloat3(&camUp)
	);

	// PROJECTION
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		1280.0f / 720.f,
		0.1f,
		5000.0f
	);

	// Upload to GPU
	CameraCB cb;
	XMStoreFloat4x4(&cb.world, DirectX::XMMatrixTranspose(worldMat));
	XMStoreFloat4x4(&cb.view, DirectX::XMMatrixTranspose(view));
	XMStoreFloat4x4(&cb.projection, XMMatrixTranspose(proj));

	context->UpdateSubresource(cameraBuffer, 0, nullptr, &cb, 0, 0);
	context->VSSetConstantBuffers(0, 1, &cameraBuffer);
	



	//activating the shader pipeline
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	const WorldMesh& mesh = world.getMesh();

	context->IASetVertexBuffers(0, 1, &mesh.vertexBuffer, &stride, &offset);
	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	////TEMP CPU TRANSFORM (untill matrices)
	//Vertex transformed[3] = {
	//	{  0.0f - camx,  0.5f - camy, 0.0f, 1, 0, 0, 1 },
	//	{  0.5f - camx, -0.5f - camy, 0.0f, 0, 1, 0, 1 },
	//	{ -0.5f - camx, -0.5f - camy, 0.0f, 0, 0, 1, 1 }
	//};
	//
	////context->UpdateSubresource(vertexBuffer, 0, nullptr, transformed, 0, 0);

	context->Draw(mesh.vertexCount, 0);

	swapChain->Present(1, 0);


	//Reset per-frame deltas
	Input::EndFrame();
}

void Graphics::shutdown()
{
	if (renderTarget) renderTarget->Release();
	if (swapChain) swapChain->Release();
	if (context) context->Release();
	if (device) device->Release();
	if (hwnd) DestroyWindow(hwnd);
}