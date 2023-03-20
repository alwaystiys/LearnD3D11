#include<learnd3d11/renderer.h>

Renderer::Renderer()
{
	m_vertexBuffer = 0;
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}

Renderer::Renderer(const Renderer&)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	result = Init(device, hwnd);
	if (!result)
		return false;

	return true;
}

void Renderer::alertError(HRESULT hResult, ID3DBlob* shaderCompileErrorsBlob) {
	const char* errorString = NULL;
	if (hResult == HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND))
		errorString = "Could not compile shader; file not found";
	else if (shaderCompileErrorsBlob) {
		errorString = (const char*)shaderCompileErrorsBlob->GetBufferPointer();
		shaderCompileErrorsBlob->Release();
		shaderCompileErrorsBlob = 0;
	}
	MessageBoxA(0, errorString, "Shader Compiler Error", MB_ICONERROR | MB_OK);
}

bool Renderer::Init(ID3D11Device* device, HWND hwnd)
{
	LPCWSTR filePath = L"../../res/shaders/4.1.color.hlsl";
	LPCSTR vs_entty_point = "vs_main";
	LPCSTR ps_entty_point = "ps_main";
	LPCSTR vs_target = "vs_5_0";
	LPCSTR ps_target = "ps_5_0";

	// Create Vertex Shader
	HRESULT hResult = 0;
	ID3DBlob* vsBlob = 0;
	ID3DBlob* psBlob = 0;
	ID3DBlob* shaderCompileErrorsBlob = 0;
	
	hResult = D3DCompileFromFile(filePath, nullptr, nullptr, vs_entty_point, vs_target, 0, 0, &vsBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult)){
		alertError(hResult, shaderCompileErrorsBlob);
		return false;
	}
	hResult = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(hResult))
		return false;

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hResult = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_layout);
	if (FAILED(hResult))
		return false;
	vsBlob->Release();
	vsBlob = 0;

	// Create Pixel Shader
	hResult = D3DCompileFromFile(filePath, nullptr, nullptr, ps_entty_point, ps_target, 0, 0, &psBlob, &shaderCompileErrorsBlob);
	if (FAILED(hResult)){
		alertError(hResult, shaderCompileErrorsBlob);
		return false;
	}
	hResult = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader);
	if (FAILED(hResult))
		return false;
	psBlob->Release();
	psBlob = 0;

	// Create Vertex Buffer
	float vertexData[] = { // x, y, r, g, b, a
		0.0f,  0.5f, 0.f, 1.f, 0.f, 1.f,
		0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f
	};
	m_stride = 6 * sizeof(float);
	m_numVerts = sizeof(vertexData) / m_stride;
	m_offset = 0;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = sizeof(vertexData);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertexData;

	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &m_vertexBuffer);
	if (FAILED(hResult))
		return false;

	return true;
}

bool Renderer::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_stride, &m_offset);

	deviceContext->Draw(m_numVerts, 0);

	return true;
}

void Renderer::Shutdown()
{
	if (m_layout){
		m_layout->Release();
		m_layout = 0;
	}
	if (m_pixelShader){
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	if (m_vertexShader){
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	if (m_vertexBuffer){
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}